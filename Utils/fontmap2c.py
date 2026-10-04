#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
fontmap2c.py — конвертер BMP-карты шрифта (bitmap font sheet) в пару C/C++
файлов для LetoCore-структуры:

    struct GlyphData {
        const uint8_t width;
        const uint8_t height;
        const uint8_t* bitmap;
    };

Выход (всегда UTF-8 без BOM, имя и каталог — из output_name):

    .h:   extern const GlyphData __glyph_<prefix>__[N];
    .cpp: const GlyphData __glyph_<prefix>__[N] = {...};
          и private static-битмапы __glyph_<prefix>_<hex row><hex col>__
          (row/col — позиция ячейки в сетке, 0-based)

Число глифов N известно генератору, поэтому оно записано в размер массива, а
отдельная переменная «count» не эмится: длина берётся из типа массива.

Вход: ТОЛЬКО несжатый BMP (BI_RGB / BI_BITFIELDS, сырые пиксели). Карта ровно
в 3 цветах, допусков и настраиваемых порогов нет:

    * чёрный   (luma <= 25,  напр. #000000, #010101) -> активный пиксель глифа
    * белый    (luma >= 230, напр. #FFFFFF, #FEFEFE) -> фон
    * всё остальное                                 -> рамка/сетка; её цвет
      определяется автоматически (самый частый пиксель этого класса)

Битмап — page-major (SSD1306-native, как читает GlyphData::GetPixel):
pages = (height + 7) // 8, столбец x страницы p лежит в buf[x * pages + p]
битом (1 << (y % 8)), бит 0 — верхний пиксель страницы. По умолчанию чёрный = 0,
белый = 1 (0x00 = столбец целиком чёрный, 0xFF = целиком белый); прежняя
полярность (чёрный = 1) — флаг --bit-polarity high.

Размер глифа берётся СТРОГО из расстояния между линиями рамки (с учётом их
толщины): содержимое ячеек не обрезается и не сдвигается, а пустая ячейка даёт
глиф того же размера, целиком белый. Линии столбцов ищутся отдельно в полосе
каждого ряда, поэтому разные ряды карты могут иметь разную ширину ячеек; ряд, в
полосе которого не видно всех общих границ, остаётся на общей сетке. Любое
расхождение ряда с общей сеткой печатается как [warn].

--skip-empty исключает пустые ячейки из вывода совсем (экономия flash: у русских
шрифтов после "Я"/"ъ" идёт залитый хвост таблицы). Пустой считается ячейка без
единого чёрного пикселя ИЛИ залитая сплошь (порог --solid-coverage, правило
отключается ключом --keep-solid). Тогда индекс элемента в массиве больше НЕ равен
позиции кода: пропущенные ячейки с причинами перечислены комментарием в .cpp.

Использование:
    python3 fontmap2c.py <image.bmp> <output_name> [опции]
    python3 fontmap2c.py ru.bmp ru_font --skip-empty --preview
    python3 fontmap2c.py font.bmp font --min-line-run 8 --min-coverage 0.9
"""

from __future__ import annotations

import argparse
import os
import re
import struct
import sys

# --- классификация пикселей -------------------------------------------------
INK = 0      # чёрный  (активный пиксель глифа)
PAPER = 1    # белый   (фон)
FRAME = 2    # рамка / сетка (любой другой цвет)

# Границы классов яркости (0..255, ITU-R BT.601) фиксированы: карта индексируется
# вручную и значения точные, при этом #010101 остаётся чёрным, #FEFEFE — белым.
INK_LUMA_MAX = 25      # luma <= 25 (~10%)  -> INK
PAPER_LUMA_MIN = 230   # luma >= 230 (~90%) -> PAPER

# Полярность бита в выходном битмапе (--bit-polarity): каким значением бита
# записан ЧЁРНЫЙ пиксель. Все прочие пиксели ячейки — белые, цвета рамки и
# «хвост» страницы за пределами height — получают противоположное значение.
INK_BIT_LOW = 0     # 0 = чёрный, 1 = белый  (по умолчанию: байт 0x00 = чёрный столбец)
INK_BIT_HIGH = 1    # 1 = чёрный, 0 = белый  (прежний SSD1306-стиль)
BIT_POLARITY = {'low': INK_BIT_LOW, 'high': INK_BIT_HIGH}


def polarity_note(ink_bit):
    """Короткое описание полярности — для баннера и комментариев в .h/.cpp."""
    if ink_bit == INK_BIT_HIGH:
        return 'чёрный пиксель = 1, белый = 0'
    return 'чёрный пиксель = 0, белый = 1'


class FontmapError(Exception):
    """Ошибка, текст которой печатается в stderr с возвратом кода 1."""


# --- мелочь -----------------------------------------------------------------
def luma(r, g, b):
    """Яркость ITU-R BT.601 в целых числах (0..255)."""
    return (r * 299 + g * 587 + b * 114) // 1000


def sanitize_ident(text, fallback='fontmap'):
    """Валидное C-имя: только [A-Za-z0-9_], цифра не может быть первой."""
    out = re.sub(r'[^0-9A-Za-z_]+', '_', str(text).strip())
    out = re.sub(r'_+', '_', out).strip('_')
    if not out:
        out = fallback
    if out[0].isdigit():
        out = '_' + out
    return out


# --- чтение BMP -------------------------------------------------------------
def bmp_header_info(path):
    """
    Разбор BITMAPFILEHEADER + BITMAPINFOHEADER: нужен чтобы отказать сжатым BMP
    и чтобы явно определить ориентацию по знаку biHeight (top-down / bottom-up).
    """
    with open(path, 'rb') as f:
        head = f.read(64)
    if len(head) < 54 or head[:2] != b'BM':
        raise FontmapError('файл %s не является BMP: нет подписи "BM"' % path)
    pix_off = struct.unpack_from('<I', head, 10)[0]
    hdr_size = struct.unpack_from('<I', head, 14)[0]
    width = struct.unpack_from('<i', head, 18)[0]
    height = struct.unpack_from('<i', head, 22)[0]
    planes, bpp = struct.unpack_from('<HH', head, 26)
    comp = struct.unpack_from('<I', head, 30)[0] if hdr_size >= 40 else 0
    if hdr_size < 12:
        raise FontmapError('неподдерживаемый заголовок BMP (size=%d) в %s' % (hdr_size, path))
    if comp not in (0, 3):
        raise FontmapError(
            'BMP %s сжат (biCompression=%d). Поддержаны только несжатые сырые '
            'пиксели: BI_RGB(0) / BI_BITFIELDS(3).' % (path, comp))
    return {
        'path': path,
        'hdr_size': hdr_size,
        'pix_off': pix_off,
        'width': width,
        'raw_height': height,
        'height': abs(height),
        'top_down': height < 0,
        'bpp': bpp,
        'planes': planes,
        'comp': {0: 'BI_RGB', 3: 'BI_BITFIELDS'}[comp],
    }


def load_bmp_with_pillow(path, info):
    """
    Основной читатель. Pillow сам приводит bottom-up BMP к нормальному порядку
    строк, поэтому rows[y][x] всегда читается сверху вниз (y=0 — верхняя строка).
    """
    from PIL import Image
    with Image.open(path) as im:
        if getattr(im, 'format', None) != 'BMP':
            raise FontmapError('Pillow декодировал %s как %r, а не как BMP'
                               % (path, im.format))
        if im.size != (info['width'], info['height']):
            raise FontmapError('несоответствие размера BMP в %s' % path)
        rgb = im.convert('RGB')
        w, h = rgb.size
        raw = rgb.tobytes('raw', 'RGB')     # top-down, stride = w*3 (getdata deprecated)
    flat = [tuple(raw[o:o + 3]) for o in range(0, w * h * 3, 3)]
    return [flat[y * w:(y + 1) * w] for y in range(h)]


def load_bmp_raw(path, info):
    """
    Запасной читатель (без Pillow): несжатые BMP 1/4/8/24/32 bpp, палитра или
    direct color, ориентация определяется знаком biHeight вручную.
    """
    with open(path, 'rb') as f:
        data = f.read()
    w, h, bpp = info['width'], info['height'], info['bpp']
    if bpp not in (1, 4, 8, 24, 32):
        raise FontmapError('BMP с %d bpp не поддерживает запасной читатель '
                           '(установите Pillow: pip install pillow)' % bpp)
    hdr_end = 14 + info['hdr_size']
    palette = []
    if bpp <= 8:
        count = 1 << bpp
        if info['hdr_size'] >= 40:
            count = struct.unpack_from('<I', data, 32)[0] or count
        for i in range(count):
            off = hdr_end + i * 4
            if off + 4 > len(data):
                raise FontmapError('BMP %s: палитра выходит за пределы файла' % path)
            b, g, r, _a = struct.unpack_from('<BBBB', data, off)
            palette.append((r, g, b))
    row_bytes = ((w * bpp + 31) // 32) * 4
    src = info['pix_off'] or hdr_end
    if src + row_bytes * h > len(data):
        raise FontmapError('BMP %s: данных пикселей не хватает на %dx%d @ %d bpp'
                           % (path, w, h, bpp))

    def mono(idx):
        if palette and idx < len(palette):
            return palette[idx]
        return ((255 - idx * 255),) * 3

    rows = []
    for y in range(h):
        file_row = y if info['top_down'] else (h - 1 - y)   # bottom-up: строка 0 = низ
        raw = data[src + file_row * row_bytes: src + (file_row + 1) * row_bytes]
        if bpp == 24:
            line = [(raw[x * 3 + 2], raw[x * 3 + 1], raw[x * 3]) for x in range(w)]
        elif bpp == 32:
            line = [(raw[x * 4 + 2], raw[x * 4 + 1], raw[x * 4]) for x in range(w)]
        elif bpp == 8:
            line = [mono(raw[x]) for x in range(w)]
        elif bpp == 4:
            line = [mono((raw[x >> 1] >> 4 if not (x & 1) else raw[x >> 1] & 0x0F))
                    for x in range(w)]
        else:  # bpp == 1
            line = [mono((raw[x >> 3] >> (7 - (x & 7))) & 1) for x in range(w)]
        rows.append(line)
    return rows


def load_font_bitmap(path, verbose=False):
    """-> (width, height, rows, info); ориентация нормализована (rows[0] = верх)."""
    ext = os.path.splitext(path)[1].lower()
    if ext != '.bmp':
        raise FontmapError('принимается только .bmp (несжатые сырые пиксели), получено %r' % ext)
    if not os.path.isfile(path):
        raise FontmapError('файл не найден: %s' % path)
    info = bmp_header_info(path)
    if info['width'] <= 0:
        raise FontmapError('некорректная ширина BMP: %d' % info['width'])
    reader = 'pillow'
    try:
        rows = load_bmp_with_pillow(path, info)
    except ImportError:
        rows = load_bmp_raw(path, info)
        reader = 'raw'
    if verbose:
        sys.stderr.write('[bmp] %s %dx%d %dbpp %s %s (читатель: %s)\n' % (
            os.path.basename(path), info['width'], info['height'], info['bpp'], info['comp'],
            'top-down' if info['top_down'] else 'bottom-up', reader))
    return info['width'], info['height'], rows, info


# --- бинаризация карты (3 фиксированных класса) ------------------------------
def build_masks(rows, W, H):
    """
    Каждый пиксель однозначно относится к одному из 3 классов (без допусков и
    dithering): luma <= INK_LUMA_MAX -> INK, luma >= PAPER_LUMA_MIN -> PAPER,
    всё остальное -> FRAME. Возвращает (mask, frame_pixel_count, hist), где hist —
    счётчик цветов рамки: по нему определяется цвет линий.
    """
    mask = []
    frame_count = 0
    hist = {}
    for y in range(H):
        line_in = rows[y]
        line_out = bytearray(W)
        for x in range(W):
            c = line_in[x]
            v = luma(*c)
            if v <= INK_LUMA_MAX:
                kind = INK
            elif v >= PAPER_LUMA_MIN:
                kind = PAPER
            else:
                kind = FRAME
                frame_count += 1
                hist[c] = hist.get(c, 0) + 1
            line_out[x] = kind
        mask.append(line_out)
    return mask, frame_count, hist


def detect_frame_color(hist):
    """
    Авто-детект цвета рамки: самый частый «не чёрный/не белый» цвет. Он покрывает
    линии целиком, в отличие от одиночных шумных/сглаженных пикселей.
    """
    if not hist:
        return None
    return sorted(hist.items(), key=lambda kv: -kv[1])[0][0]


# --- поиск линий рамки ------------------------------------------------------
def longest_true_run(seq):
    """Длина максимальной непрерывной последовательности ненулевых значений."""
    best = cur = 0
    for v in seq:
        cur = cur + 1 if v else 0
        if cur > best:
            best = cur
    return best


class Line(object):
    """Одна линия рамки: отрезок [start..end] включительно, thickness = end-start+1."""

    __slots__ = ('start', 'end')

    def __init__(self, start, end):
        self.start = start
        self.end = end

    @property
    def thickness(self):
        return self.end - self.start + 1

    def __repr__(self):
        return 'Line(%d..%d, %dpx)' % (self.start, self.end, self.thickness)


def find_frame_lines(mask, W, H, orientation, min_run, min_coverage, band=None):
    """
    orientation: 'v' — вертикальные линии (X-координаты => границы столбцов),
                 'h' — горизонтальные линии (Y-координаты => границы строк).
    Линия = непрерывная последовательность пикселей цвета рамки длиной >= min_run,
    при условии, что её покрытие по изображению >= min_coverage (доли 0..1).
    Соседние столбцы/строки-кандидаты склеиваются в одну толстую линию.
    band=(y0, y1) — искать вертикальные линии только в полосе строк: и длина, и
    покрытие меряются по высоте полосы. Так ряд карты видит СВОЮ сетку столбцов
    (граница узкого глифа занимает одну полосу и по всей карте не набирает
    покрытие). Для 'h' полоса не применяется.
    """
    if orientation == 'v':
        y_lo, y_hi = band if band else (0, H)
        span = W
        other = y_hi - y_lo
        if band:                 # полоса короче порога: требуем линию по всей полосе
            min_run = max(1, min(min_run, other))
    else:
        y_lo, y_hi, span, other = 0, H, H, W
    candidates = []
    for i in range(span):
        if orientation == 'v':
            col = [1 if mask[y][i] == FRAME else 0 for y in range(y_lo, y_hi)]
        else:
            col = [1 if mask[i][x] == FRAME else 0 for x in range(other)]
        if longest_true_run(col) < min_run:
            continue
        if sum(col) < min_coverage * other:
            continue
        candidates.append(i)
    lines = []
    for i in candidates:
        if lines and i == lines[-1].end + 1:
            lines[-1].end = i              # склейка: толщина линии > 1px
        else:
            lines.append(Line(i, i))
    return lines


# --- сетка ячеек и упаковка -------------------------------------------------
class Cell(object):
    """Ячейка сетки: внутренняя область (без рамки) + упакованный битмап."""

    __slots__ = ('row', 'col', 'x0', 'y0', 'width', 'height', 'pages', 'bits',
                 'ink', 'empty', 'kept', 'skip_reason')

    def __init__(self, row, col, x0, y0, width, height):
        self.row = row
        self.col = col
        self.x0 = x0                 # левый пиксель содержимого (в координатах BMP)
        self.y0 = y0                 # верхний пиксель содержимого
        self.width = width           # строго по сетке, НЕ по контенту
        self.height = height         # строго по сетке
        self.pages = (height + 7) // 8
        self.bits = bytearray(width * self.pages)
        self.ink = 0                 # число «чёрных» (INK) пикселей в ячейке
        self.empty = True            # ink == 0
        self.kept = True             # False -> не попадает в выходной массив (--skip-empty)
        self.skip_reason = None      # 'blank' | 'solid 36/36' | None


def inner_intervals(lines):
    """Внутренние отрезки между соседними линиями: [(start, end_inclusive, size)]."""
    out = []
    for a, b in zip(lines, lines[1:]):
        start = a.end + 1            # сразу после левой линии (учтена её толщина)
        end = b.start - 1            # сразу перед правой линией
        if end >= start:
            out.append((start, end, end - start + 1))
    return out


def pack_page_major(cell, mask, ink_bit=INK_BIT_LOW):
    """
    Столбец x страницы p пишется в bits[x * pages + p] битом (1 << (y % 8)).
    Чёрный пиксель — значение ink_bit (--bit-polarity), остальные пиксели и
    биты страницы за пределами height — противоположное значение. Параллельно
    считается число ink-пикселей: по нему --skip-empty распознаёт залитые сплошь
    служебные ячейки.
    """
    ink = 0
    for x in range(cell.width):
        col_base = x * cell.pages
        for y in range(cell.height):
            if mask[cell.y0 + y][cell.x0 + x] == INK:
                cell.bits[col_base + (y >> 3)] |= (1 << (y & 7))
                ink += 1
    cell.ink = ink
    cell.empty = (ink == 0)
    if ink_bit == INK_BIT_LOW:      # активно-низкая маска: 0 = чёрный, 1 = белый
        for i in range(len(cell.bits)):
            cell.bits[i] ^= 0xFF
    return cell


def row_line_sets(mask, W, H, hlines, opts):
    """
    Вертикальные линии в полосе КАЖДОГО ряда: список списков Line, параллельный
    рядам из inner_intervals(hlines). Без этого граница узкого глифа, нарисованная
    только в одном ряду, не набирает покрытия по всей карте и теряется: ячейка
    становится шире, а пиксели её рамки уходят в битмап как «белые» (0xFF).
    """
    sets = []
    for y0, _y1, h in inner_intervals(hlines):
        sets.append(find_frame_lines(mask, W, H, 'v', opts.min_line_run,
                                     opts.min_coverage, (y0, y0 + h)))
    return sets


def is_global_refinement(band_lines, global_lines):
    """
    True, если полоса ряда видит ВСЕ общие границы столбцов (возможно, плюс свои).
    Иначе полоса не «тоньше» общей сетки, а потеряны разделители (сглаживание,
    заливка цветом рамки) — такой ряд оставляем на общей сетке, чтобы вместо
    нескольких глифов не получить один во всю ширину карты.
    """
    for g in global_lines:
        if not any(min(g.end, b.end) >= max(g.start, b.start) for b in band_lines):
            return False
    return True


def extract_cells(mask, vlines, hlines, band_vlines=None, verbose=False,
                  ink_bit=INK_BIT_LOW):
    """
    Плоский список глифов в порядке ячеек на карте: слева-направо, сверху-вниз.
    Границы столбцов ряда берутся из линий его полосы (band_vlines), поэтому ряды
    могут иметь разную ширину ячеек; если полоса ряда не видит хотя бы одну общую
    границу либо своих линий нет вовсе, используется общая сетка vlines.
    """
    xs = inner_intervals(vlines)
    ys = inner_intervals(hlines)
    if not xs or not ys:
        raise FontmapError('между линиями рамки нет ни одной ячейки: карта слишком '
                           'маленькая либо линии идут вплотную друг к другу')
    cells = []
    for r, (y0, _y1, ch) in enumerate(ys):
        row_xs = xs
        if band_vlines is not None:
            own = inner_intervals(band_vlines[r])
            if own and is_global_refinement(band_vlines[r], vlines):
                row_xs = own
            elif own and own != xs:
                sys.stderr.write('[warn] row %d: полоса ряда видит не все общие '
                                 'границы столбцов, взята общая сетка\n' % r)
        for c, (x0, _x1, cw) in enumerate(row_xs):
            cells.append(pack_page_major(Cell(r, c, x0, y0, cw, ch), mask, ink_bit))
    heights = sorted({c.height for c in cells})
    widths = sorted({c.width for c in cells})
    if len(heights) > 1:
        sys.stderr.write('[warn] высота ячеек неодинакова: %s\n' % heights)
    if verbose:
        sys.stderr.write('[grid] %d строк x %d столбцов = %d ячеек, '
                         'inner width=%s, inner height=%s\n'
                         % (len(ys), len(xs), len(cells), widths, heights))
    global_w = [t[2] for t in xs]
    for r in sorted({c.row for c in cells}):
        row_w = [c.width for c in cells if c.row == r]
        if row_w != global_w:
            sys.stderr.write('[warn] row %d: своя сетка столбцов width=%s, '
                             'общая width=%s\n' % (r, row_w, global_w))
    return cells


def cell_tag(c):
    """Метка ячейки для --preview: EMPTY / SOLID n/m / used + приписка о пропуске."""
    total = c.width * c.height
    if c.skip_reason == 'blank':
        base = 'EMPTY'
    elif c.skip_reason:
        base = 'SOLID %s' % c.skip_reason[len('solid '):]
    elif c.empty:
        base = 'EMPTY'
    elif total and c.ink == total:
        base = 'SOLID %d/%d' % (c.ink, total)
    else:
        base = 'used'
    return base if c.kept else '%s (skipped by --skip-empty)' % base


def ascii_preview(cells, ink_bit=INK_BIT_LOW):
    """
    ASCII-отрисовка всех глифов (отладка сетки/упаковки): --preview.
    '#' печатается там, где в битмапе записан чёрный пиксель, то есть где бит
    равен ink_bit (полярность --bit-polarity).
    """
    out = []
    for c in cells:
        out.append('--- [%d:%d] %dx%d pages=%d %s' % (
            c.row, c.col, c.width, c.height, c.pages, cell_tag(c)))
        for y in range(c.height):
            out.append('   ' + ''.join(
                '#' if ((c.bits[x * c.pages + (y >> 3)] >> (y & 7)) & 1) == ink_bit
                else '.' for x in range(c.width)))
        out.append('   ' + ' '.join('%02X' % b for b in c.bits))
    return '\n'.join(out)


# --- имена ------------------------------------------------------------------
def hex_digits_for(count):
    """Сколько hex-цифр нужно, чтобы занумеровать `count` значений (0-based)."""
    return max(1, len('%x' % max(0, count - 1)))


class Names(object):
    """Генератор имён: __glyph_<prefix>_<hex row><hex col>__ + публичный массив
    GlyphData с явным (известным на этапе генерации) числом элементов."""

    def __init__(self, prefix, rows, cols):
        self.prefix = sanitize_ident(prefix)
        self.dr = hex_digits_for(rows)
        self.dc = hex_digits_for(cols)

    def array(self):
        return '__glyph_%s__' % self.prefix

    def bitmap(self, row, col):
        token = ('%0*x' % (self.dr, row)) + ('%0*x' % (self.dc, col))
        return '__glyph_%s_%s__' % (self.prefix, token)


# --- генерация C/C++ --------------------------------------------------------
BYTES_PER_LINE = 12
TAB = '\t'


def format_bitmap(bits):
    """Строки с байтами массива: по BYTES_PER_LINE значений в строке."""
    hexes = ['0x%02X' % b for b in bits]
    out = []
    for i in range(0, len(hexes), BYTES_PER_LINE):
        out.append(TAB + ', '.join(hexes[i:i + BYTES_PER_LINE]) + ',')
    return out


def skipped_positions(cells):
    """
    Компактный список ячеек, исключённых --skip-empty, с причиной:
    ['row=4 col=6..15 (solid 36/36)', 'row=0 col=0 (blank)'].
    """
    groups = {}
    for c in cells:
        if not c.kept:
            groups.setdefault((c.row, c.skip_reason or 'blank'), []).append(c.col)
    out = []
    for key in sorted(groups):
        r, reason = key
        cs = sorted(groups[key])
        runs, a, b = [], cs[0], cs[0]
        for v in cs[1:]:
            if v == b + 1:
                b = v
            else:
                runs.append((a, b))
                a = b = v
        runs.append((a, b))
        out.append('row=%d col=%s (%s)' % (r, ', '.join(
            str(x) if x == y else '%d..%d' % (x, y) for x, y in runs), reason))
    return out


def skip_stats(cells):
    """Счётчики пропусков по причинам: {'blank': чистые, 'solid': залитые сплошь}."""
    st = {'blank': 0, 'solid': 0}
    for c in cells:
        if c.kept or not c.skip_reason:
            continue
        st['solid' if c.skip_reason.startswith('solid') else 'blank'] += 1
    return st


def skip_note(st, skipped):
    """'чистых 3, залитых 14' — для баннера/документации."""
    if not skipped:
        return ''
    parts = []
    if st['blank']:
        parts.append('чистых %d' % st['blank'])
    if st['solid']:
        parts.append('залитых сплошь %d' % st['solid'])
    return ', '.join(parts) if parts else str(skipped)


def banner_lines(src_path, info, cells, vlines, hlines, ink_bit=INK_BIT_LOW):
    xs = sorted({c.width for c in cells})
    ys = sorted({c.height for c in cells})
    rows = max((c.row for c in cells), default=-1) + 1
    cols = max((c.col for c in cells), default=-1) + 1
    kept = [c for c in cells if c.kept]
    skipped = len(cells) - len(kept)
    st = skip_stats(cells)
    total = sum(len(c.bits) for c in kept)
    head = ['Сгенерировано fontmap2c.py — НЕ РЕДАКТИРОВАТЬ ВРУЧНУЮ.',
            'Источник: %s (%dx%d px, %d bpp, %s, %s)' % (
                src_path, info['width'], info['height'], info['bpp'], info['comp'],
                'top-down' if info['top_down'] else 'bottom-up'),
            'Сетка: %d горизонтальных линий x %d вертикальных -> %d строк x %d столбцов'
            % (len(hlines), len(vlines), rows, cols),
            'Внутренние размеры ячеек: width=%s px, height=%s px' % (xs, ys)]
    if skipped:
        head.append('Глифов: %d из %d ячеек (пропущено пустых: %d — %s), байт битмапов: %d'
                    % (len(kept), len(cells), skipped, skip_note(st, skipped), total))
        head.append('Пустые (чистые и залитые сплошь) ячейки исключены --skip-empty: индекс')
        head.append('в массиве НЕ равен позиции кода; пропущенные ячейки с причинами')
        head.append('перечислены в .cpp, а имена битмапов несут hex row/col своей позиции.')
    else:
        head.append('Глифов: %d, байт битмапов: %d' % (len(cells), total))
    head += [
        'Формат: page-major (SSD1306-native): pages=(height+7)/8,',
        '        bitmap[x*pages + y/8], бит (1 << (y%%8)); %s.' % polarity_note(ink_bit),
    ]
    if ink_bit == INK_BIT_LOW:
        head += ['        То есть 0x00 = столбец целиком чёрный, 0xFF = целиком белый.',
                 '        GetPixel()==true здесь означает «белый»: такую маску рисуют',
                 '        инверсно (пиксель ставится там, где бит сброшен).']
    else:
        head.append('        То есть 0x00 = столбец целиком белый, 0xFF = целиком чёрный.')
    return head


def ru_plural(n, forms):
    """forms = (1 элемент, 2 элемента, 5 элементов) — русская плюрализация."""
    n10, n100 = n % 10, n % 100
    if n10 == 1 and n100 != 11:
        return forms[0]
    if 2 <= n10 <= 4 and not 12 <= n100 <= 14:
        return forms[1]
    return forms[2]


def gen_header(names, banner, glyph_count, skipped=None, skip_note=''):
    L = ['/*']
    for s in banner:
        L.append(' * %s' % s)
    L.append(' */')
    L.append('')
    L.append('#pragma once')
    L.append('')
    L.append('#include <stdint.h>')
    L.append('#include <cstddef>')
    L.append('')
    L.append('#include "Graphics/GlyphData.hpp"')
    L.append('')
    L.append('/**')
    L.append(' * @brief Массив глифов карты шрифта: ровно %d %s.'
             % (glyph_count, ru_plural(glyph_count,
                                       ('элемент', 'элемента', 'элементов'))))
    L.append(' *')
    L.append(' * Размер массива явный — он известен генератору, поэтому отдельная')
    L.append(' * переменная количества глифов не эмится: длина берётся из типа,')
    L.append(' * std::size(%s) == %d (нужен <iterator>) либо' % (names.array(), glyph_count))
    L.append(' * sizeof(%s) / sizeof(%s[0]) == %d.'
             % (names.array(), names.array(), glyph_count))
    L.append(' *')
    if skipped:
        L.append(' * Порядок элементов: ТОЛЬКО осмысленные ячейки карты, слева-направо и')
        L.append(' * сверху-вниз (row-major). Пустые (без чернил и залитые сплошь) ячейки')
        L.append(' * исключены --skip-empty (пропущено %s), поэтому %d — это НЕ '
                 'число' % (skip_note, glyph_count))
        L.append(' * ячеек карты, а индекс элемента НЕ равен номеру кода в таблице:')
        L.append(' * соответствие «код -> индекс» строит потребитель (позиции и причины')
        L.append(' * пропусков перечислены в .cpp).')
    else:
        L.append(' * Порядок элементов строго соответствует порядку ячеек на карте:')
        L.append(' * слева-направо, сверху-вниз (row-major). Никаких предположений о кодировке')
        L.append(' * не делается — соответствие «глиф -> код символа» задаёт потребитель.')
    L.append(' */')
    L.append('extern const GlyphData %s[%d];' % (names.array(), glyph_count))
    L.append('')
    return '\n'.join(L)


def gen_source(names, header_base, banner, cells, ink_bit=INK_BIT_LOW):
    kept = [c for c in cells if c.kept]
    skipped = skipped_positions(cells)
    L = ['/*']
    for s in banner:
        L.append(' * %s' % s)
    L.append(' *')
    L.append(' * ЛОГИКА page-major УПАКОВКИ (почему именно так):')
    L.append(' *   1) высота режется на страницы по 8 пикселей: pages = (height + 7) / 8;')
    L.append(' *      height=8  -> pages=1 (1 байт на столбец), height=16 -> pages=2')
    L.append(' *      (2 байта на столбец: младший — строки y=0..7, старший — y=8..15),')
    L.append(' *      height=6  -> pages=1 (используются младшие 6 бит, старшие 2 = %s);'
             % ('1 — белый' if ink_bit == INK_BIT_LOW else '0'))
    L.append(' *   2) внутри страницы бит 0 — САМЫЙ ВЕРХНИЙ пиксель (y = page*8 + 0),')
    L.append(' *      бит 7 — самый нижний (y = page*8 + 7), т.е. биты идут сверху вниз;')
    L.append(' *   3) байты упорядочены по столбцам: data[x * pages + page]. Для SSD1306')
    L.append(' *      это родной порядок обхода (страница -> колонка), поэтому буфер можно')
    L.append(' *      отдавать в окно RAM дисплея без перестановки байт;')
    if ink_bit == INK_BIT_LOW:
        L.append(' *   4) полярность: %s — байт 0x00 значит «столбец целиком'
                 % polarity_note(ink_bit))
        L.append(' *      чёрный», 0xFF — «целиком белый» (в т.ч. биты страницы')
        L.append(' *      за пределами height). Это AND-пробойка чёрного в белом')
        L.append(' *      экране (buffer &= glyph); для GlyphData::GetPixel(), где')
        L.append(' *      true значит «рисовать пиксель», нужна инверсная проверка.')
    else:
        L.append(' *   4) полярность: %s — байт 0x00 значит «столбец пустой»,'
                 % polarity_note(ink_bit))
        L.append(' *      0xFF — «столбец целиком чёрный» (прежний SSD1306-формат).')
    L.append(' *      Пиксели цвета рамки в битмап не попадают, а размер глифа всегда')
    L.append(' *      берётся СТРОГО по сетке (даже для пустых ячеек — тогда их буфер')
    L.append(' *      целиком %s).' % ('из 0xFF' if ink_bit == INK_BIT_LOW else 'из нулей'))
    L.append(' */')
    L.append('')
    L.append('#include <stdint.h>')
    L.append('#include <cstddef>')
    L.append('')
    L.append('#include "%s"' % header_base)
    L.append('')
    L.append('// ----------------------------------------------------------------------')
    L.append('// Приватные битмапы глифов: static — не видны за пределами этого файла.')
    L.append('// Имя: __glyph_<prefix>_<hex row><hex col>__, row/col — позиция в сетке,')
    L.append('// 0-based. Отдельные битмапы не объявлены в .h: это детали реализации.')
    L.append('// ----------------------------------------------------------------------')
    L.append('')
    if skipped:
        L.append('// --skip-empty: пустые ячейки исключены, битмапы для них не создаются')
        L.append('// (blank — нет ни одного чёрного пикселя; solid n/m — залита сплошь,')
        L.append('//  n/m — число чёрных пикселей; позиции — чтобы не путать индекс')
        L.append('//  массива с кодом символа):')
        for s in skipped:
            L.append('//   %s' % s)
        L.append('')
    for c in kept:
        name = names.bitmap(c.row, c.col)
        L.append('/* glyph [row=%d col=%d] %dx%d px, pages=%d, %d bytes%s */'
                 % (c.row, c.col, c.width, c.height, c.pages, len(c.bits),
                    ', EMPTY' if c.empty else ''))
        L.append('static const uint8_t %s[] = {' % name)
        L.extend(format_bitmap(c.bits))
        L.append('};')
        L.append('')
    L.append('// ----------------------------------------------------------------------')
    L.append('// Публичный массив глифов: порядок = %s карты слева-направо, сверху-вниз.'
             % ('непустые ячейки' if skipped else 'ячейки'))
    L.append('// width/height — размеры строго по сетке рамки, а не по контенту символа.')
    L.append('// ----------------------------------------------------------------------')
    L.append('// Размер массива явный и совпадает с extern в "%s": число глифов' % header_base)
    L.append('// известно генератору, поэтому отдельная переменная count не нужна —')
    L.append('// потребитель берёт размер из типа: std::size(%s) или' % names.array())
    L.append('// sizeof(%s) / sizeof(%s[0]) == %d.' % (names.array(), names.array(), len(kept)))
    L.append('const GlyphData %s[%d] = {' % (names.array(), len(kept)))
    width_field = max((len(names.bitmap(c.row, c.col)) for c in kept), default=0)
    for i, c in enumerate(kept):
        name = names.bitmap(c.row, c.col)
        entry = '{ %d, %d, %s },' % (c.width, c.height, name)
        note = '/* [%d] row=%d col=%d, pages=%d, %d bytes%s */' % (
            i, c.row, c.col, c.pages, len(c.bits), ', empty' if c.empty else '')
        pad = ' ' * (width_field - len(name) + 4)
        L.append(TAB + entry + pad + note)
    L.append('};')
    L.append('')
    return '\n'.join(L)


# --- конвейер ---------------------------------------------------------------
def build_grid(W, H, rows, opts):
    """
    -> (mask, vlines, hlines, frame_rgb, band_vlines). Полный разбор карты:
    классификация по 3 цветам, авто-детект цвета рамки, поиск линий (общий и по
    полосе каждого ряда), границы столбцов/строк.
    """
    mask, frame_count, hist = build_masks(rows, W, H)
    frame_rgb = detect_frame_color(hist)
    if frame_rgb is None:
        raise FontmapError(
            'в %s не найден ни один пиксель третьего цвета: рамка/сетка отсутствует. '
            'Карта должна быть трёхцветной: чёрный (luma <= %d), белый (luma >= %d) и '
            'любой другой цвет на линиях сетки.'
            % (opts.image, INK_LUMA_MAX, PAPER_LUMA_MIN))

    vlines = find_frame_lines(mask, W, H, 'v', opts.min_line_run, opts.min_coverage)
    hlines = find_frame_lines(mask, W, H, 'h', opts.min_line_run, opts.min_coverage)
    if len(vlines) < 2 or len(hlines) < 2:
        raise FontmapError(
            'сетка не распознана: найдено %d вертикальных и %d горизонтальных линий '
            'рамки (нужно минимум по 2). Цвет рамки: #%02X%02X%02X, всего пикселей '
            'рамки: %d. Ослабьте пороги: --min-line-run %d --min-coverage %.2f.'
            % (len(vlines), len(hlines), frame_rgb[0], frame_rgb[1], frame_rgb[2],
               frame_count, opts.min_line_run, opts.min_coverage))
    return mask, vlines, hlines, frame_rgb, row_line_sets(mask, W, H, hlines, opts)


def apply_skip_empty(cells, opts):
    """
    --skip-empty: в вывод не попадают «чистые» ячейки (ни одного ink-пикселя) и
    ячейки, залитые сплошь — служебная подложка/заполнение хвоста таблицы.
    Порог «сплошь» задаёт --solid-coverage (1.0 = только идеально залитые),
    правило заливки целиком отключается ключом --keep-solid.
    """
    if not getattr(opts, 'skip_empty', False):
        return list(cells)
    cov = getattr(opts, 'solid_coverage', 1.0)
    keep_solid = getattr(opts, 'keep_solid', False)
    for c in cells:
        total = c.width * c.height
        c.skip_reason = None
        if c.ink == 0:
            c.skip_reason = 'blank'
        elif not keep_solid and total and c.ink >= cov * total - 1e-9:
            c.skip_reason = 'solid %d/%d' % (c.ink, total)
        c.kept = c.skip_reason is None
    kept = [c for c in cells if c.kept]
    if not kept:
        raise FontmapError('все %d ячеек карты пусты или залиты: с --skip-empty писать '
                           'нечего. Убери флаг, верни залитые ячейки ключом --keep-solid '
                           'либо подправь --solid-coverage.'
                           % len(cells))
    return kept


def run_pipeline(opts):
    """
    Чтение BMP -> разбор сетки -> глифы в page-major -> тексты .h/.cpp.
    Возвращает dict с итогом работы (нужен main и тестам).
    """
    W, H, rows, info = load_font_bitmap(opts.image, verbose=opts.verbose)
    mask, vlines, hlines, frame_rgb, band_vlines = build_grid(W, H, rows, opts)
    ink_bit = BIT_POLARITY[opts.bit_polarity]
    cells = extract_cells(mask, vlines, hlines, band_vlines,
                          verbose=opts.verbose, ink_bit=ink_bit)
    kept = apply_skip_empty(cells, opts)
    skipped = skipped_positions(cells)
    n_skipped = len(cells) - len(kept)
    st = skip_stats(cells)
    if n_skipped:
        sys.stderr.write('[skip] --skip-empty: ячеек %d, глифов записано %d, пропущено '
                         'пустых %d (%s)\n'
                         % (len(cells), len(kept), n_skipped, skip_note(st, n_skipped)))

    rows_n = max(c.row for c in cells) + 1
    cols_n = max(c.col for c in cells) + 1
    base = sanitize_ident(os.path.splitext(os.path.basename(opts.output_name))[0],
                          fallback='fontmap')
    header_base = base + '.h'
    names = Names(base, rows_n, cols_n)
    banner = banner_lines(opts.image, info, cells, vlines, hlines, ink_bit)
    header_text = gen_header(names, banner, len(kept), skipped, skip_note(st, n_skipped))
    source_text = gen_source(names, header_base, banner, cells, ink_bit)
    return {
        'info': info, 'W': W, 'H': H, 'mask': mask, 'rows': rows,
        'vlines': vlines, 'hlines': hlines, 'frame_rgb': frame_rgb,
        'band_vlines': band_vlines,
        'cells': cells, 'kept': kept, 'skipped': skipped, 'names': names,
        'ink_bit': ink_bit,
        'out_dir': os.path.dirname(opts.output_name) or '.', 'header_base': header_base,
        'header_text': header_text, 'source_text': source_text,
    }


def write_outputs(res):
    """
    Пишет <base>.h и <base>.cpp в UTF-8 БЕЗ BOM (как в закоммиченных файлах
    шрифтов LetoCore). Возвращает кортеж путей.
    """
    h_path = os.path.join(res['out_dir'], res['header_base'])
    c_path = os.path.join(res['out_dir'], os.path.splitext(res['header_base'])[0] + '.cpp')
    if res['out_dir'] and not os.path.isdir(res['out_dir']):
        os.makedirs(res['out_dir'])
    for path, text in ((h_path, res['header_text']), (c_path, res['source_text'])):
        with open(path, 'w', encoding='utf-8', newline='\n') as f:
            f.write(text)
    return h_path, c_path


# --- CLI --------------------------------------------------------------------
def build_arg_parser():
    ap = argparse.ArgumentParser(
        prog='fontmap2c.py',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description='Конвертация BMP-карты шрифта в .h/.cpp со структурой GlyphData '
                    '(page-major / SSD1306-native).',
        epilog='примеры:\n'
               '  python3 fontmap2c.py font.bmp myfont\n'
               '  python3 fontmap2c.py font.bmp myfont --preview --verbose\n'
               '  python3 fontmap2c.py ru.bmp ru_font --skip-empty   # без пустых ячеек\n'
               '  # хвост таблицы залит чёрным (сплошь) — такие ячейки тоже пустые:\n'
               '  python3 fontmap2c.py font.bmp ru_font --skip-empty --solid-coverage 0.95\n'
               '  # в шрифте есть реальный символ-квадрат: не считать заливку пустотой\n'
               '  python3 fontmap2c.py font.bmp ru_font --skip-empty --keep-solid\n'
               '  # прежняя полярность байтов (1 = чёрный пиксель, 0 = белый):\n'
               '  python3 fontmap2c.py font.bmp ru_font --bit-polarity high\n')
    ap.add_argument('image', nargs='?', help='входной несжатый BMP (.bmp) с картой глифов')
    ap.add_argument('output_name', nargs='?',
                    help='имя output_name.h / output_name.cpp (можно с путём, '
                         'например build/font)')
    ap.add_argument('--min-line-run', type=int, default=5, metavar='N',
                    help='минимальная длина непрерывного отрезка рамки для линии '
                         '(default: 5)')
    ap.add_argument('--min-coverage', type=float, default=0.5, metavar='F',
                    help='минимальное покрытие стороны изображения рамкой, 0..1 '
                         '(default: 0.5). Для вертикальных линий покрытие меряется '
                         'по высоте полосы своего ряда, поэтому ряд может иметь '
                         'свою сетку столбцов. Чем выше порог — тем строже: если '
                         'ячейки залиты цветом рамки, низкий порог склеивает такую '
                         'область с линиями в одну толстую (проверь --preview)')
    ap.add_argument('--skip-empty', action='store_true',
                    help='полностью пропускать пустые ячейки: для них не создаются '
                         'static-битмапы и они не попадают в публичный массив '
                         '(экономия flash; индекс элемента тогда НЕ равен коду). '
                         'Пустой считается ячейка без единого чёрного пикселя, а также '
                         'залитая сплошь (служебная подложка/подкрашенные хвосты '
                         'таблицы) — см. --solid-coverage/--keep-solid')
    ap.add_argument('--solid-coverage', type=float, default=1.0, metavar='F',
                    help='при какой доле чёрных пикселей ячейка считается залитой '
                         'сплошь и пропускается вместе с --skip-empty, 0..1 '
                         '(default: 1.0 = только идеально залитые; 0.95 допускает '
                         'сглаживание/шум)')
    ap.add_argument('--keep-solid', action='store_true',
                    help='не считать залитые сплошь ячейки пустыми — оставить только '
                         'фильтр «ни одного чёрного пикселя» (нужно, если в шрифте есть '
                         'реальный символ-квадрат)')
    ap.add_argument('--bit-polarity', choices=('low', 'high'), default='low',
                    help='каким значением бита записан ЧЁРНЫЙ пиксель глифа. '
                         'low (по умолчанию): 0 = чёрный, 1 = белый/прозрачно — байт '
                         '0x00 = полностью чёрный столбец, пустая ячейка = 0xFF, '
                         'удобно как AND-маска «пробойки» чёрного; high: прежний формат '
                         '1 = чёрный, 0 = белый (пустая ячейка = 0x00) — как ожидает '
                         'потребитель вида «GetPixel() == true -> рисовать пиксель»')
    ap.add_argument('--preview', action='store_true',
                    help='ASCII-отрисовка всех глифов в stdout')
    ap.add_argument('--quiet', action='store_true', help='не печатать сводку')
    ap.add_argument('--verbose', action='store_true', help='подробный лог разбора')
    return ap


def summarize(opts, res, written):
    if opts.quiet:
        return
    cells = res['cells']
    rows_n = max(c.row for c in cells) + 1
    cols_n = max(c.col for c in cells) + 1
    fr = res['frame_rgb']
    heights = sorted({c.height for c in cells})
    widths = sorted({c.width for c in cells})
    empty = sum(1 for c in cells if c.empty)
    solid = sum(1 for c in cells if c.width * c.height and c.ink == c.width * c.height)
    kept = res['kept']
    total_bytes = sum(len(c.bits) for c in kept)
    n_skipped = len(cells) - len(kept)
    why = skip_note(skip_stats(cells), n_skipped)
    note = ''
    if n_skipped:
        note = ' (записано %d, пропущено пустых %d: %s)' % (len(kept), n_skipped, why)
    sys.stdout.write(
        '[ok] %s: %dx%d px, рамка #%02X%02X%02X, линий h=%d v=%d -> сетка %dx%d = %d '
        'глифов (%dx%d px), pages=%s, пустых=%d, залитых=%d%s, байт=%d%s\n'
        % (opts.image, res['W'], res['H'], fr[0], fr[1], fr[2],
           len(res['hlines']), len(res['vlines']), rows_n, cols_n,
           len(cells), min(widths), max(heights),
           sorted({c.pages for c in cells}), empty, solid, note, total_bytes,
           ' -> ' + ', '.join(written)))
    if opts.verbose:
        sys.stdout.write('     v-lines: %s\n' % ', '.join(str(l) for l in res['vlines']))
        sys.stdout.write('     h-lines: %s\n' % ', '.join(str(l) for l in res['hlines']))
        sys.stdout.write('     names:   %s[%d], примеры битмапов: %s, %s\n' % (
            res['names'].array(), len(res['kept']),
            res['names'].bitmap(0, 0), res['names'].bitmap(rows_n - 1, cols_n - 1)))
        if res['skipped']:
            sys.stdout.write('     skipped: %s\n' % '; '.join(res['skipped']))


def main(argv=None):
    ap = build_arg_parser()
    opts = ap.parse_args(argv)
    if not opts.image or not opts.output_name:
        ap.error('нужны аргументы <image.bmp> и <output_name> (см. --help)')
    if not 0.0 < opts.solid_coverage <= 1.0:
        ap.error('--solid-coverage должен быть в интервале (0, 1]')
    if not opts.skip_empty and (opts.keep_solid or opts.solid_coverage != 1.0):
        ap.error('--keep-solid и --solid-coverage работают только вместе с --skip-empty')
    try:
        res = run_pipeline(opts)
        written = [os.path.relpath(p) for p in write_outputs(res)]
    except FontmapError as exc:
        sys.stderr.write('ERROR: %s\n' % exc)
        return 1
    summarize(opts, res, written)
    if opts.preview:
        sys.stdout.write(ascii_preview(res['cells'], res['ink_bit']) + '\n')
    return 0


if __name__ == '__main__':
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        sys.exit(130)
