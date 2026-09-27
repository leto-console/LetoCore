#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
fontmap2c_test.py — тесты конвертера fontmap2c.py.

Строит синтетические BMP-карты (чёрный глиф / белый фон / цветная рамка), гоняет
их через run_pipeline и проверяет сетку, page-major-упаковку, полярность битов,
имена символов и запись .h/.cpp.

Запуск:  python3 fontmap2c_test.py        (код 0 — всё прошло, 1 — есть FAIL)
"""

import os
import struct
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from fontmap2c import (FRAME, FontmapError, INK_BIT_HIGH, INK_BIT_LOW,
                       ascii_preview, bmp_header_info, build_arg_parser,
                       load_bmp_raw, load_bmp_with_pillow, ru_plural,
                       run_pipeline, write_outputs)


BLACK = (0x00, 0x00, 0x00)
WHITE = (0xFF, 0xFF, 0xFF)
BLUE = (0x44, 0x88, 0xCC)
GRAY = (0x80, 0x80, 0x80)        # «серая» рамка: тоже FRAME (средняя яркость)
NEAR_BLACK = (0x01, 0x01, 0x01)  # фактический чёрный в base_6x6_*.bmp
NEAR_WHITE = (0xFE, 0xFE, 0xFE)


def write_test_bmp(path, rows, top_down=False):
    """Пишет несжатый 24bpp BMP; rows[0] — ВЕРХНЯЯ строка изображения."""
    h = len(rows)
    w = len(rows[0])
    row_bytes = ((w * 3 + 3) // 4) * 4
    chunks = []
    for y in range(h):
        line = bytearray()
        for r, g, b in rows[y]:
            line += bytes((b, g, r))
        line += bytes(row_bytes - len(line))          # паддинг строки до 4 байт
        chunks.append(line)
    if not top_down:
        chunks.reverse()                              # bottom-up: низ файла = верх
    pix = b''.join(chunks)
    hdr = 14 + 40
    with open(path, 'wb') as f:
        f.write(b'BM' + struct.pack('<IHHI', hdr + len(pix), 0, 0, hdr))
        f.write(struct.pack('<IiiHHIIiiII', 40, w, -h if top_down else h,
                            1, 24, 0, len(pix), 2835, 2835, 0, 0))
        f.write(pix)


def make_font_map(cols, cell_w, cell_h, rows_count=1, frame=1, ink=None):
    """
    Синтетическая карта: rows_count строк по cols ячеек cell_w x cell_h,
    толщина рамки `frame` px. ink(row, col, x, y) -> bool задает черные пиксели.
    """
    w = frame * (cols + 1) + cell_w * cols
    h = frame * (rows_count + 1) + cell_h * rows_count
    grid = [[BLUE for _ in range(w)] for _ in range(h)]
    for r in range(rows_count):
        for c in range(cols):
            x0 = frame + c * (cell_w + frame)
            y0 = frame + r * (cell_h + frame)
            for y in range(cell_h):
                for x in range(cell_w):
                    grid[y0 + y][x0 + x] = (BLACK if (ink and ink(r, c, x, y)) else WHITE)
    return grid


class Tester(object):
    def __init__(self):
        self.failed = 0
        self.tmp = os.path.join(tempfile.gettempdir(), 'fontmap2c_selftest')
        if not os.path.isdir(self.tmp):
            os.makedirs(self.tmp)

    def ok(self, name, cond, extra=''):
        sys.stdout.write('[%s] %s%s\n' % ('ok' if cond else 'FAIL', name,
                                          '' if cond or not extra else '  <- ' + str(extra)))
        if not cond:
            self.failed += 1

    def pipeline(self, image, output_name, *extra_args):
        opts = build_arg_parser().parse_args([image, output_name, '--quiet']
                                             + list(extra_args))
        return run_pipeline(opts)


def run_tests():
    t = Tester()
    p = lambda name: os.path.join(t.tmp, name)

    # 1) высота = 8  -> pages = 1, по 1 байту на столбец
    ink8 = lambda r, c, x, y: c == 0 and ((x == 0 and y == 0) or (x == 1 and y == 7))
    bmp8 = p('map_h8.bmp')
    write_test_bmp(bmp8, make_font_map(2, 5, 8, 1, 1, ink8))
    res = t.pipeline(bmp8, p('out_h8'))
    cells = res['cells']
    t.ok('h=8: pages=1', [c.pages for c in cells] == [1, 1], [c.pages for c in cells])
    t.ok('h=8: размер буфера = width*pages = 5',
         [len(c.bits) for c in cells] == [5, 5], [len(c.bits) for c in cells])
    t.ok('h=8: чёрный обнуляет бит -> [0] бит0=0 (верх), [1] бит7=0 (низ), фон 0xFF',
         list(cells[0].bits) == [0xFE, 0x7F, 0xFF, 0xFF, 0xFF], list(cells[0].bits))
    t.ok('h=8: размер глифа строго по сетке (5x8)',
         (cells[0].width, cells[0].height) == (5, 8),
         (cells[0].width, cells[0].height))
    t.ok('h=8: имена массивов',
         res['names'].bitmap(0, 0) == '__glyph_out_h8_00__'
         and res['names'].array() == '__glyph_out_h8__', res['names'].bitmap(0, 0))
    t.ok('h=8: .h только про публичный API (нет extern битмапов, размер явный)',
         '__glyph_out_h8_00__' not in res['header_text']
         and 'extern const GlyphData __glyph_out_h8__[2];' in res['header_text']
         and '_count' not in res['header_text'])
    t.ok('h=8: .cpp: static-битмап + публичный массив явного размера, без count',
         'static const uint8_t __glyph_out_h8_00__[] = {' in res['source_text']
         and '{ 5, 8, __glyph_out_h8_00__ },' in res['source_text']
         and 'const GlyphData __glyph_out_h8__[2] = {' in res['source_text']
         and '_count' not in res['source_text'])

    t.ok('ru_plural: формы рус числительного',
         [ru_plural(n, ('элемент', 'элемента', 'элементов'))
          for n in (1, 2, 5, 11, 21, 96)]
         == ['элемент', 'элемента', 'элементов', 'элементов', 'элемент', 'элементов'],
         [ru_plural(n, ('элемент', 'элемента', 'элементов'))
          for n in (1, 2, 5, 11, 21, 96)])

    # 2) высота = 16 -> pages = 2, по 2 байта на столбец
    ink16 = lambda r, c, x, y: c == 0 and ((x == 0 and y == 8)
                                           or (x == 1 and y == 15)
                                           or (x == 2 and y == 0))
    bmp16 = p('map_h16.bmp')
    write_test_bmp(bmp16, make_font_map(2, 3, 16, 1, 1, ink16))
    c16 = t.pipeline(bmp16, p('out_h16'))['cells'][0]
    t.ok('h=16: pages=2', c16.pages == 2, c16.pages)
    t.ok('h=16: размер буфера = width*pages = 6', len(c16.bits) == 6, len(c16.bits))
    t.ok('h=16: чёрные пиксели обнуляют биты: [1]=0xFE, [3]=0x7F, [4]=0xFE',
         list(c16.bits) == [0xFF, 0xFE, 0xFF, 0x7F, 0xFE, 0xFF], list(c16.bits))

    # 3) толстая рамка (3 px): внутренние размеры не включают рамку
    bmp3 = p('map_thick.bmp')
    write_test_bmp(bmp3, make_font_map(2, 6, 8, 2, frame=3,
                                       ink=lambda r, c, x, y: y == 0))
    res3 = t.pipeline(bmp3, p('out_thick'))
    t.ok('frame=3px: ширина глифа = 6 (не 8 и не 12)',
         {c.width for c in res3['cells']} == {6}, {c.width for c in res3['cells']})
    t.ok('frame=3px: высота глифа = 8',
         {c.height for c in res3['cells']} == {8}, {c.height for c in res3['cells']})
    t.ok('frame=3px: 2 строки x 2 столбца = 4 глифа',
         len(res3['cells']) == 4, len(res3['cells']))
    t.ok('frame=3px: в байтах обнулён только bit0 (верхний ряд чёрный)',
         all(set(c.bits) == {0xFE} for c in res3['cells']),
         [list(c.bits) for c in res3['cells']])

    # 3b) полярность бита (--bit-polarity): по умолчанию 0 = чёрный, high — прежний формат
    res_hi = t.pipeline(bmp3, p('out_thick_hi'), '--bit-polarity', 'high')
    t.ok('--bit-polarity high: прежний формат, bit0 = 1 на чёрном ряду',
         all(set(c.bits) == {0x01} for c in res_hi['cells']),
         [list(c.bits) for c in res_hi['cells']])
    t.ok('low и high дают взаимно дополнительные байты',
         all([b ^ 0xFF for b in hi.bits] == list(lo.bits)
             for lo, hi in zip(res3['cells'], res_hi['cells'])),
         [list(res3['cells'][0].bits), list(res_hi['cells'][0].bits)])
    t.ok('ink/empty (фильтры --skip-empty) от полярности не зависят',
         [(c.ink, c.empty) for c in res3['cells']]
         == [(c.ink, c.empty) for c in res_hi['cells']],
         [(c.ink, c.empty) for c in res3['cells']])
    t.ok('preview печатает # на чёрных пикселях при обеих полярностях',
         ascii_preview(res3['cells'], INK_BIT_LOW).count('#')
         == ascii_preview(res_hi['cells'], INK_BIT_HIGH).count('#') == 4 * 6,
         ascii_preview(res3['cells'], INK_BIT_LOW).count('#'))
    t.ok('в .cpp описана активно-низкая полярность (low)',
         'чёрный пиксель = 0' in res3['source_text']
         and 'чёрный пиксель = 1' not in res3['source_text'],
         [l.strip() for l in res3['source_text'].splitlines() if 'пиксель = ' in l][:2])
    t.ok('в .cpp при high описана прежняя полярность',
         'чёрный пиксель = 1, белый = 0' in res_hi['source_text'],
         [l.strip() for l in res_hi['source_text'].splitlines() if 'пиксель = ' in l][:2])

    bmp_sol6 = p('map_solid6.bmp')
    write_test_bmp(bmp_sol6, make_font_map(1, 6, 6, 1, 1,
                                           ink=lambda r, c, x, y: True))
    csol = t.pipeline(bmp_sol6, p('out_solid6'))['cells'][0]
    t.ok('сплошь чёрная ячейка 6x6 -> 0xC0 (биты 0..5 = чёрный, хвост страницы = белый)',
         list(csol.bits) == [0xC0] * 6, list(csol.bits))
    t.ok('сплошь чёрная ячейка: ink=36, empty=False (счёт по фактическим чёрным)',
         csol.ink == 36 and csol.empty is False, (csol.ink, csol.empty))
    res_sol6_hi = t.pipeline(bmp_sol6, p('out_solid6_hi'), '--bit-polarity', 'high')
    t.ok('--bit-polarity high: сплошь чёрная ячейка 6x6 -> 0x3F',
         list(res_sol6_hi['cells'][0].bits) == [0x3F] * 6,
         list(res_sol6_hi['cells'][0].bits))

    # 4) ориентация: bottom-up и top-down дают идентичный результат
    grid = make_font_map(3, 5, 12, 2, 1, lambda r, c, x, y: (x + y + c) % 4 == 0)
    bu, td = p('map_bu.bmp'), p('map_td.bmp')
    write_test_bmp(bu, grid, top_down=False)
    write_test_bmp(td, grid, top_down=True)
    r_bu = t.pipeline(bu, p('same'))
    r_td = t.pipeline(td, p('same'))
    t.ok('top-down == bottom-up (битмапы)',
         [c.bits for c in r_bu['cells']] == [c.bits for c in r_td['cells']])
    mark = '#include "same.h"'      # шапка отличается: в неё попадает путь к исходному BMP
    t.ok('top-down == bottom-up (тело .cpp)',
         r_bu['source_text'][r_bu['source_text'].index(mark):]
         == r_td['source_text'][r_td['source_text'].index(mark):])
    t.ok('ориентация определена из заголовка',
         r_bu['info']['top_down'] is False and r_td['info']['top_down'] is True)

    # 5) пустые ячейки: ширина по сетке, буфер целиком белый, глиф присутствует
    bmp5 = p('map_empty_cell.bmp')
    write_test_bmp(bmp5, make_font_map(2, 7, 8, 1, 1,
                                       ink=lambda r, c, x, y: c == 0 and y == 0))
    res5 = t.pipeline(bmp5, p('out_empty'))
    t.ok('пустая ячейка: width=7 по сетке, bits=0xFF (белый), empty=True',
         (res5['cells'][1].width, list(res5['cells'][1].bits), res5['cells'][1].empty)
         == (7, [0xFF] * 7, True),
         (res5['cells'][1].width, list(res5['cells'][1].bits)))
    t.ok('пустая ячейка попала в публичный массив',
         '{ 7, 8, __glyph_out_empty_01__ },' in res5['source_text'])

    # 5b) --skip-empty: пустые ячейки не создают битмапов и не попадают в массив
    res5b = t.pipeline(bmp5, p('out_skip'), '--skip-empty')
    t.ok('--skip-empty: cells по-сетке 2, записан 1 непустой',
         len(res5b['cells']) == 2 and [c.col for c in res5b['kept']] == [0],
         [c.col for c in res5b['kept']])
    t.ok('--skip-empty: битмап пустой ячейки не создан и его нет в массиве',
         '__glyph_out_skip_01__' not in res5b['source_text']
         and '{ 7, 8, __glyph_out_skip_00__ },' in res5b['source_text'])
    t.ok('--skip-empty: пропущенные позиции документированы в .cpp',
         'row=0 col=1' in res5b['source_text']
         and '--skip-empty' in res5b['source_text']
         and 'пропущено пустых: 1' in res5b['header_text'])
    t.ok('--skip-empty: .h предупреждает, что индекс != код',
         'НЕ равен номеру кода' in res5b['header_text']
         and 'НЕ равен номеру кода' not in res5['header_text'])
    bmp_all = p('map_all_empty.bmp')
    write_test_bmp(bmp_all, make_font_map(2, 5, 8, 1, 1, None))
    try:
        t.pipeline(bmp_all, p('out_skip_all'), '--skip-empty')
        t.ok('--skip-empty при полностью пустой карте -> FontmapError', False)
    except FontmapError as exc:
        t.ok('--skip-empty при полностью пустой карте -> FontmapError',
             'писать нечего' in str(exc), str(exc))

    # 5c) «русская таблица»: 5 строк x 16 столбцов, в 5-й строке только 2 символа
    def ink_ru(r, c, x, y):
        return not (r == 4 and c > 1) and (x == 0 or y == 15)
    bmp_ru = p('map_ru_partial.bmp')
    write_test_bmp(bmp_ru, make_font_map(16, 6, 16, 5, 1, ink_ru))
    res_ru = t.pipeline(bmp_ru, p('out_ru'))
    res_sk = t.pipeline(bmp_ru, p('out_ru_skip'), '--skip-empty')
    t.ok('русская таблица 5x16: 80 ячеек, 14 пустых (белых)',
         len(res_ru['cells']) == 80
         and sum(1 for c in res_ru['cells'] if c.empty) == 14,
         (len(res_ru['cells']), sum(1 for c in res_ru['cells'] if c.empty)))
    t.ok('--skip-empty: 80 ячеек -> 66 глифов, пустые строки 5 не созданы',
         len(res_sk['kept']) == 66
         and res_sk['source_text'].count('static const uint8_t') == 66
         and res_ru['source_text'].count('static const uint8_t') == 80
         and '__glyph_out_ru_skip_41__' in res_sk['source_text']
         and '__glyph_out_ru_skip_42__' not in res_sk['source_text'],
         (len(res_sk['kept']), res_sk['source_text'].count('static const uint8_t')))
    t.ok('--skip-empty: экономия байт = 14*12 = 168',
         sum(len(c.bits) for c in res_ru['cells']) - sum(len(c.bits) for c in res_sk['kept'])
         == 14 * 12, sum(len(c.bits) for c in res_sk['kept']))
    t.ok('--skip-empty: индекс [0] и hex-имена сохраняют позицию сетки',
         '/* [0] row=0 col=0' in res_sk['source_text']
         and '/* [65] row=4 col=1' in res_sk['source_text'],
         [ln for ln in res_sk['source_text'].splitlines() if 'row=4 col=1' in ln])
    t.ok('--skip-empty: .h сообщает число пропущенных ячеек (14), а не число диапазонов',
         'пропущено чистых 14' in res_sk['header_text']
         and 'пропущено 1)' not in res_sk['header_text'],
         [ln for ln in res_sk['header_text'].splitlines() if 'пропущено' in ln])

    # 5d) --skip-empty и залитая сплошь ячейка (= заливка хвоста таблицы чёрным)
    def ink_solid(r, c, x, y):
        if c == 0:
            return True                       # залита сплошь 6x16 = 96 px
        if c == 1:
            return y != 0                     # почти залита: 90/96
        return x == 0 and y == 5              # обычный глиф
    bmp_sol = p('map_solid.bmp')
    write_test_bmp(bmp_sol, make_font_map(3, 6, 16, 1, 1, ink_solid))
    res_sol = t.pipeline(bmp_sol, p('out_solid'), '--skip-empty')
    t.ok('solid: залитая сплошь ячейка пропущена, обычная — нет',
         [c.col for c in res_sol['kept']] == [1, 2],
         [(c.col, c.ink, c.skip_reason) for c in res_sol['cells']])
    t.ok('solid: причина пропуска — solid 96/96',
         'row=0 col=0 (solid 96/96)' in res_sol['source_text'],
         res_sol['skipped'])
    t.ok('solid: в баннере разбивка «залитых сплошь 1»',
         'залитых сплошь 1' in res_sol['header_text'],
         [ln for ln in res_sol['header_text'].splitlines() if 'пропущено' in ln])
    res_ks = t.pipeline(bmp_sol, p('out_keepsolid'), '--skip-empty', '--keep-solid')
    t.ok('solid: --keep-solid оставляет залитую ячейку глифом',
         [c.col for c in res_ks['kept']] == [0, 1, 2],
         [c.col for c in res_ks['kept']])
    res_cv = t.pipeline(bmp_sol, p('out_cov'), '--skip-empty', '--solid-coverage', '0.9')
    t.ok('solid: --solid-coverage 0.9 пропускает и почти залитую (90/96)',
         [c.col for c in res_cv['kept']] == [2]
         and '(solid 96/96)' in res_cv['source_text']
         and '(solid 90/96)' in res_cv['source_text'],
         [(c.col, c.ink, c.skip_reason) for c in res_cv['cells']])
    pv_sol = ascii_preview(res_sol['cells'])
    pv_keep = ascii_preview(res_ks['cells'])
    pv_blank = ascii_preview(t.pipeline(bmp5, p('out_skip2'), '--skip-empty')['cells'])
    t.ok('solid: --preview различает EMPTY / SOLID / used',
         'SOLID 96/96 (skipped by --skip-empty)' in pv_sol
         and 'EMPTY (skipped by --skip-empty)' in pv_blank
         and 'pages=2 SOLID 96/96' in pv_keep,
         [ln for ln in pv_keep.splitlines() if ln.startswith('--- ')])

    # 5e) узкий глиф в одном ряду: границы столбцов ищутся в полосе своего ряда
    grid_nw = make_font_map(4, 6, 8, 3, 1, lambda r, c, x, y: c in (1, 2) and x == 0)
    x0n, y0n = 1 + 2 * (6 + 1), 1 + 1 * (8 + 1)          # ячейка [row=1 col=2]
    for yy in range(8):
        grid_nw[y0n + yy][x0n + 5] = BLUE      # своя правая граница узкого глифа
    bmp_nw = p('map_narrow_cell.bmp')
    write_test_bmp(bmp_nw, grid_nw)
    res_nw = t.pipeline(bmp_nw, p('out_narrow'))
    by_row = dict((r, [c.width for c in res_nw['cells'] if c.row == r])
                  for r in range(3))
    t.ok('row 1: ширина глифа взята из своей границы ряда (5), другие ряды — 6',
         by_row == {0: [6, 6, 6, 6], 1: [6, 6, 5, 6], 2: [6, 6, 6, 6]}, by_row)
    frame_in = [(c.row, c.col) for c in res_nw['cells']
                if any(res_nw['mask'][c.y0 + y][c.x0 + x] == FRAME
                       for y in range(c.height) for x in range(c.width))]
    t.ok('внутри боксов не осталось пикселей рамки (нет фантомных 0xFF)',
         frame_in == [], frame_in)
    t.ok('узкий глиф: битмап на байт короче соседей',
         [len(c.bits) for c in res_nw['cells'] if c.row == 1] == [6, 6, 5, 6],
         [len(c.bits) for c in res_nw['cells'] if c.row == 1])
    t.ok('в .cpp width у каждого глифа свой',
         '{ 5, 8, __glyph_out_narrow_12__ },' in res_nw['source_text']
         and '{ 6, 8, __glyph_out_narrow_02__ },' in res_nw['source_text'],
         [ln.strip() for ln in res_nw['source_text'].splitlines()
          if 'glyph_out_narrow_12__ },' in ln])

    # 5f) ряд потерял внутренние разделители -> остаётся общая сетка, а не один
    #     глиф на всю ширину карты
    grid_ls = make_font_map(4, 6, 8, 3, 1, lambda r, c, x, y: c == 1 and x == 0)
    for yy in range(8):
        for xx in (7, 14, 21):
            grid_ls[y0n + yy][xx] = WHITE            # стёрты общие границы столбцов
    bmp_ls = p('map_lost_lines.bmp')
    write_test_bmp(bmp_ls, grid_ls)
    res_ls = t.pipeline(bmp_ls, p('out_lost'))
    t.ok('ряд без части общих границ: 12 ячеек по 6 px, а не 3 по 27',
         len(res_ls['cells']) == 12
         and {c.width for c in res_ls['cells']} == {6},
         sorted({c.width for c in res_ls['cells']}))

    # 6) запасной читатель (без Pillow) совпадает с побайтовым эталоном
    raw_rows = load_bmp_raw(bu, bmp_header_info(bu))
    t.ok('raw-читатель: bottom-up разобран верно', raw_rows == grid)
    t.ok('raw-читатель: top-down разобран так же',
         load_bmp_raw(td, bmp_header_info(td)) == grid)
    t.ok('pillow-читатель совпадает с raw',
         load_bmp_with_pillow(bu, bmp_header_info(bu)) == grid)

    # 7) ошибки: нет рамки / файл не BMP
    nb = p('no_frame.bmp')
    write_test_bmp(nb, make_font_map(2, 5, 8, 1, frame=0,
                                     ink=lambda r, c, x, y: x == 0))
    try:
        t.pipeline(nb, p('out_noframe'))
        t.ok('нет рамки -> FontmapError', False)
    except FontmapError as exc:
        t.ok('нет рамки -> FontmapError', 'не найден' in str(exc), str(exc))
    try:
        t.pipeline(__file__, p('out_bad'))
        t.ok('не .bmp -> FontmapError', False)
    except FontmapError as exc:
        t.ok('не .bmp -> FontmapError', '.bmp' in str(exc), str(exc))
    # 7b) фиксированная 3-цветная модель: near-black / near-white / серая рамка
    tc_grid = [[GRAY] * 8 for _ in range(10)]
    for y in range(1, 9):
        for x in range(1, 7):
            tc_grid[y][x] = NEAR_BLACK if x == 1 else NEAR_WHITE
    tc = p('map_three_color.bmp')
    write_test_bmp(tc, tc_grid)
    res_tc = t.pipeline(tc, p('out_three'))
    c_tc = res_tc['cells'][0]
    t.ok('3-цветная модель: #010101=INK, #FEFEFE=PAPER, #808080=FRAME',
         res_tc['frame_rgb'] == GRAY and len(res_tc['cells']) == 1
         and (c_tc.width, c_tc.height, c_tc.ink) == (6, 8, 8),
         (res_tc['frame_rgb'], len(res_tc['cells']),
          (c_tc.width, c_tc.height, c_tc.ink)))

    # 8) hex-имена: >16 строк / >16 столбцов -> 2 цифры на координату
    bmp8 = p('map_wide.bmp')
    write_test_bmp(bmp8, make_font_map(20, 4, 8, 18, 1,
                                       ink=lambda r, c, x, y: x == 0 and y == 0))
    res8 = t.pipeline(bmp8, p('out_wide'))
    t.ok('18 строк x 20 столбцов = 360 глифов',
         len(res8['cells']) == 360, len(res8['cells']))
    t.ok('hex-имена: 00->0000, (17,19)->1113',
         res8['names'].bitmap(0, 0) == '__glyph_out_wide_0000__'
         and res8['names'].bitmap(17, 19) == '__glyph_out_wide_1113__',
         res8['names'].bitmap(17, 19))

    # 9) имя массива — из basename output_name; цвет рамки определяется автоматически
    res9 = t.pipeline(bu, p('byname'))
    t.ok('имя битмапа строится из имени выхода',
         res9['names'].bitmap(0, 0) == '__glyph_byname_00__', res9['names'].bitmap(0, 0))
    t.ok('имя публичного массива == __glyph_<имя выхода>__',
         res9['names'].array() == '__glyph_byname__', res9['names'].array())
    t.ok('цвет рамки определён автоматически', res9['frame_rgb'] == BLUE,
         res9['frame_rgb'])

    # 10) запись файлов: UTF-8 без BOM и структура .h/.cpp
    h_path, c_path = write_outputs(r_bu)
    with open(h_path, 'rb') as f:
        head = f.read(3)
    t.ok('.h пишется в UTF-8 без BOM', head != b'\xef\xbb\xbf', head)
    with open(h_path, 'r', encoding='utf-8') as f:
        h_text = f.read()
    t.ok('.h содержит #pragma once и extern-массив явного размера',
         '#pragma once' in h_text
         and 'extern const GlyphData __glyph_same__[6];' in h_text
         and '_count' not in h_text)
    with open(c_path, 'rb') as f:
        t.ok('.cpp пишется в UTF-8 без BOM', f.read(3) != b'\xef\xbb\xbf')
    with open(c_path, 'r', encoding='utf-8') as f:
        c_text = f.read()
    t.ok('.cpp включает свой .h и определяет массив тем же размером',
         '#include "same.h"' in c_text
         and 'const GlyphData __glyph_same__[6] = {' in c_text
         and '_count' not in c_text)
    t.ok('.cpp документирует page-major упаковку',
         'page-major' in c_text and 'data[x * pages + page]' in c_text)
    t.ok('.h подключает Graphics/GlyphData.hpp',
         '#include "Graphics/GlyphData.hpp"' in h_text,
         [l for l in h_text.splitlines() if l.startswith('#include')])
    t.ok('.cpp не дублирует include GlyphData (берёт из своего .h)',
         '#include "Graphics/GlyphData.hpp"' not in c_text,
         [l for l in c_text.splitlines() if l.startswith('#include')])

    sys.stdout.write('\n%s\n' % ('TESTS: ALL PASSED' if t.failed == 0
                                 else 'TESTS: %d FAILED' % t.failed))
    if t.failed:
        sys.stdout.write('промежуточные файлы: %s\n' % t.tmp)
    return 1 if t.failed else 0

if __name__ == '__main__':
    sys.exit(run_tests())
