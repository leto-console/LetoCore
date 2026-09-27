/*
 * Сгенерировано fontmap2c.py — НЕ РЕДАКТИРОВАТЬ ВРУЧНУЮ.
 * Источник: base_6x6_rus.bmp (92x36 px, 24 bpp, BI_RGB, bottom-up)
 * Сетка: 6 горизонтальных линий x 17 вертикальных -> 5 строк x 16 столбцов
 * Внутренние размеры ячеек: width=[4, 5, 6] px, height=[6] px
 * Глифов: 66 из 80 ячеек (пропущено пустых: 14 — залитых сплошь 14), байт битмапов: 288
 * Пустые (чистые и залитые сплошь) ячейки исключены --skip-empty: индекс
 * в массиве НЕ равен позиции кода; пропущенные ячейки с причинами
 * перечислены в .cpp, а имена битмапов несут hex row/col своей позиции.
 * Формат: page-major (SSD1306-native): pages=(height+7)/8,
 *         bitmap[x*pages + y/8], бит (1 << (y%8)); чёрный пиксель = 0, белый = 1.
 *         То есть 0x00 = столбец целиком чёрный, 0xFF = целиком белый.
 *         GetPixel()==true здесь означает «белый»: такую маску рисуют
 *         инверсно (пиксель ставится там, где бит сброшен).
 *
 * ЛОГИКА page-major УПАКОВКИ (почему именно так):
 *   1) высота режется на страницы по 8 пикселей: pages = (height + 7) / 8;
 *      height=8  -> pages=1 (1 байт на столбец), height=16 -> pages=2
 *      (2 байта на столбец: младший — строки y=0..7, старший — y=8..15),
 *      height=6  -> pages=1 (используются младшие 6 бит, старшие 2 = 1 — белый);
 *   2) внутри страницы бит 0 — САМЫЙ ВЕРХНИЙ пиксель (y = page*8 + 0),
 *      бит 7 — самый нижний (y = page*8 + 7), т.е. биты идут сверху вниз;
 *   3) байты упорядочены по столбцам: data[x * pages + page]. Для SSD1306
 *      это родной порядок обхода (страница -> колонка), поэтому буфер можно
 *      отдавать в окно RAM дисплея без перестановки байт;
 *   4) полярность: чёрный пиксель = 0, белый = 1 — байт 0x00 значит «столбец целиком
 *      чёрный», 0xFF — «целиком белый» (в т.ч. биты страницы
 *      за пределами height). Это AND-пробойка чёрного в белом
 *      экране (buffer &= glyph); для GlyphData::GetPixel(), где
 *      true значит «рисовать пиксель», нужна инверсная проверка.
 *      Пиксели цвета рамки в битмап не попадают, а размер глифа всегда
 *      берётся СТРОГО по сетке (даже для пустых ячеек — тогда их буфер
 *      целиком из 0xFF).
 */

#include <stdint.h>
#include <cstddef>

#include "base_6x6_rus.h"

// ----------------------------------------------------------------------
// Приватные битмапы глифов: static — не видны за пределами этого файла.
// Имя: __glyph_<prefix>_<hex row><hex col>__, row/col — позиция в сетке,
// 0-based. Отдельные битмапы не объявлены в .h: это детали реализации.
// ----------------------------------------------------------------------

// --skip-empty: пустые ячейки исключены, битмапы для них не создаются
// (blank — нет ни одного чёрного пикселя; solid n/m — залита сплошь,
//  n/m — число чёрных пикселей; позиции — чтобы не путать индекс
//  массива с кодом символа):
//   row=4 col=3, 7, 13, 15 (solid 24/24)
//   row=4 col=2, 4..6, 8, 10..12, 14 (solid 30/30)
//   row=4 col=9 (solid 36/36)

/* glyph [row=0 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_00__[] = {
	0xFE, 0xC9, 0xC9, 0xFE,
};

/* glyph [row=0 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_01__[] = {
	0xFF, 0xE5, 0xE5, 0xFD,
};

/* glyph [row=0 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_02__[] = {
	0xFF, 0xE5, 0xE5, 0xDA,
};

/* glyph [row=0 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_03__[] = {
	0xFE, 0xC1, 0xC1, 0xC1,
};

/* glyph [row=0 col=4] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_04__[] = {
	0xF0, 0xDE, 0xD1, 0xFF,
};

/* glyph [row=0 col=5] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_05__[] = {
	0xDE, 0xE5, 0xE5, 0xE5,
};

/* glyph [row=0 col=6] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_06__[] = {
	0xF7, 0xC8, 0xFF, 0xC8, 0xF7,
};

/* glyph [row=0 col=7] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_07__[] = {
	0xE5, 0xE5, 0xE5, 0xDA,
};

/* glyph [row=0 col=8] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_08__[] = {
	0xFF, 0xC8, 0xC4, 0xFF,
};

/* glyph [row=0 col=9] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_09__[] = {
	0xFC, 0xD1, 0xC9, 0xFC,
};

/* glyph [row=0 col=10] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_0a__[] = {
	0xFF, 0xC4, 0xCA, 0xF1,
};

/* glyph [row=0 col=11] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_0b__[] = {
	0xE0, 0xDE, 0xC1, 0xFE,
};

/* glyph [row=0 col=12] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_0c__[] = {
	0xFF, 0xC6, 0xD8, 0xC6, 0xFF,
};

/* glyph [row=0 col=13] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_0d__[] = {
	0xFF, 0xC4, 0xC4, 0xFF,
};

/* glyph [row=0 col=14] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_0e__[] = {
	0xDE, 0xE1, 0xE1, 0xDE,
};

/* glyph [row=0 col=15] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_0f__[] = {
	0xFF, 0xC1, 0xC1, 0xFF,
};

/* glyph [row=1 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_10__[] = {
	0xFF, 0xC9, 0xC9, 0xC6,
};

/* glyph [row=1 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_11__[] = {
	0xDE, 0xE1, 0xE1, 0xE1,
};

/* glyph [row=1 col=2] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_12__[] = {
	0xC1, 0xC1, 0xFF, 0xC1, 0xC1,
};

/* glyph [row=1 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_13__[] = {
	0xD3, 0xE4, 0xE4, 0xDF,
};

/* glyph [row=1 col=4] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_14__[] = {
	0xC6, 0xC9, 0xFF, 0xC9, 0xC6,
};

/* glyph [row=1 col=5] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_15__[] = {
	0xF1, 0xCA, 0xC4, 0xCA, 0xF1,
};

/* glyph [row=1 col=6] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_16__[] = {
	0xDF, 0xD0, 0xD0, 0xDF, 0xF0,
};

/* glyph [row=1 col=7] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_17__[] = {
	0xCF, 0xC8, 0xC8, 0xFF,
};

/* glyph [row=1 col=8] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_18__[] = {
	0xFF, 0xE0, 0xFF, 0xE0, 0xFF,
};

/* glyph [row=1 col=9] 6x6 px, pages=1, 6 bytes */
static const uint8_t __glyph_base_6x6_rus_19__[] = {
	0xFF, 0xE0, 0xFF, 0xE0, 0xFF, 0xE0,
};

/* glyph [row=1 col=10] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_1a__[] = {
	0xC1, 0xFF, 0xE4, 0xE4, 0xD8,
};

/* glyph [row=1 col=11] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_1b__[] = {
	0xFF, 0xE4, 0xD8, 0xC0, 0xFF,
};

/* glyph [row=1 col=12] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_1c__[] = {
	0xFF, 0xE4, 0xE4, 0xD8,
};

/* glyph [row=1 col=13] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_1d__[] = {
	0xE2, 0xE9, 0xE9, 0xDE,
};

/* glyph [row=1 col=14] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_1e__[] = {
	0xFF, 0xC4, 0xDE, 0xE1, 0xDE,
};

/* glyph [row=1 col=15] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_1f__[] = {
	0xE6, 0xD9, 0xC9, 0xFF,
};

/* glyph [row=2 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_20__[] = {
	0xD2, 0xEA, 0xEA, 0xFC,
};

/* glyph [row=2 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_21__[] = {
	0xFC, 0xEA, 0xEA, 0xD2,
};

/* glyph [row=2 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_22__[] = {
	0xFE, 0xEA, 0xEA, 0xD4,
};

/* glyph [row=2 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_23__[] = {
	0xFC, 0xC2, 0xC2, 0xC2,
};

/* glyph [row=2 col=4] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_24__[] = {
	0xE0, 0xDC, 0xD2, 0xFC,
};

/* glyph [row=2 col=5] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_25__[] = {
	0xDC, 0xEA, 0xEA, 0xE4,
};

/* glyph [row=2 col=6] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_26__[] = {
	0xF6, 0xC8, 0xFE, 0xC8, 0xF6,
};

/* glyph [row=2 col=7] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_27__[] = {
	0xEA, 0xEA, 0xEA, 0xD4,
};

/* glyph [row=2 col=8] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_28__[] = {
	0xFE, 0xC8, 0xC4, 0xFE,
};

/* glyph [row=2 col=9] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_29__[] = {
	0xFC, 0xD1, 0xC9, 0xFC,
};

/* glyph [row=2 col=10] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_2a__[] = {
	0xFE, 0xC8, 0xD4, 0xE2,
};

/* glyph [row=2 col=11] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_2b__[] = {
	0xE0, 0xDC, 0xC2, 0xFC,
};

/* glyph [row=2 col=12] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_2c__[] = {
	0xF8, 0xC6, 0xC8, 0xC6, 0xF8,
};

/* glyph [row=2 col=13] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_2d__[] = {
	0xFE, 0xC8, 0xC8, 0xFE,
};

/* glyph [row=2 col=14] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_2e__[] = {
	0xDC, 0xE2, 0xE2, 0xDC,
};

/* glyph [row=2 col=15] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_2f__[] = {
	0xFE, 0xC2, 0xC2, 0xFC,
};

/* glyph [row=3 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_30__[] = {
	0xFE, 0xCA, 0xCA, 0xC4,
};

/* glyph [row=3 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_31__[] = {
	0xDC, 0xE2, 0xE2, 0xE2,
};

/* glyph [row=3 col=2] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_32__[] = {
	0xC2, 0xC2, 0xFE, 0xC2, 0xC2,
};

/* glyph [row=3 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_33__[] = {
	0xE6, 0xE8, 0xE8, 0xDE,
};

/* glyph [row=3 col=4] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_34__[] = {
	0xCC, 0xD2, 0xFE, 0xD2, 0xCC,
};

/* glyph [row=3 col=5] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_35__[] = {
	0xE2, 0xD4, 0xC8, 0xD4, 0xE2,
};

/* glyph [row=3 col=6] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_36__[] = {
	0xDE, 0xD0, 0xD0, 0xDE, 0xF0,
};

/* glyph [row=3 col=7] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_37__[] = {
	0xDE, 0xD0, 0xD0, 0xFE,
};

/* glyph [row=3 col=8] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_38__[] = {
	0xFE, 0xE0, 0xFE, 0xE0, 0xFE,
};

/* glyph [row=3 col=9] 6x6 px, pages=1, 6 bytes */
static const uint8_t __glyph_base_6x6_rus_39__[] = {
	0xFE, 0xE0, 0xFE, 0xE0, 0xFE, 0xE0,
};

/* glyph [row=3 col=10] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_3a__[] = {
	0xC2, 0xFE, 0xE4, 0xE4, 0xD8,
};

/* glyph [row=3 col=11] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_3b__[] = {
	0xFE, 0xE4, 0xD8, 0xC0, 0xFE,
};

/* glyph [row=3 col=12] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_3c__[] = {
	0xFE, 0xE4, 0xE4, 0xD8,
};

/* glyph [row=3 col=13] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_3d__[] = {
	0xE2, 0xEA, 0xEA, 0xDC,
};

/* glyph [row=3 col=14] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_rus_3e__[] = {
	0xFE, 0xC8, 0xDC, 0xE2, 0xDC,
};

/* glyph [row=3 col=15] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_3f__[] = {
	0xEC, 0xD2, 0xD2, 0xFE,
};

/* glyph [row=4 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_40__[] = {
	0xDE, 0xE5, 0xE5, 0xE5,
};

/* glyph [row=4 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_rus_41__[] = {
	0xDC, 0xEA, 0xEA, 0xE4,
};

// ----------------------------------------------------------------------
// Публичный массив глифов: порядок = непустые ячейки карты слева-направо, сверху-вниз.
// width/height — размеры строго по сетке рамки, а не по контенту символа.
// ----------------------------------------------------------------------
// Размер массива явный и совпадает с extern в "base_6x6_rus.h": число глифов
// известно генератору, поэтому отдельная переменная count не нужна —
// потребитель берёт размер из типа: std::size(__glyph_base_6x6_rus__) или
// sizeof(__glyph_base_6x6_rus__) / sizeof(__glyph_base_6x6_rus__[0]) == 66.
const GlyphData __glyph_base_6x6_rus__[66] = {
	{ 4, 6, __glyph_base_6x6_rus_00__ },    /* [0] row=0 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_01__ },    /* [1] row=0 col=1, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_02__ },    /* [2] row=0 col=2, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_03__ },    /* [3] row=0 col=3, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_04__ },    /* [4] row=0 col=4, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_05__ },    /* [5] row=0 col=5, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_06__ },    /* [6] row=0 col=6, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_07__ },    /* [7] row=0 col=7, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_08__ },    /* [8] row=0 col=8, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_09__ },    /* [9] row=0 col=9, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_0a__ },    /* [10] row=0 col=10, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_0b__ },    /* [11] row=0 col=11, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_0c__ },    /* [12] row=0 col=12, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_0d__ },    /* [13] row=0 col=13, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_0e__ },    /* [14] row=0 col=14, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_0f__ },    /* [15] row=0 col=15, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_10__ },    /* [16] row=1 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_11__ },    /* [17] row=1 col=1, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_12__ },    /* [18] row=1 col=2, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_13__ },    /* [19] row=1 col=3, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_14__ },    /* [20] row=1 col=4, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_rus_15__ },    /* [21] row=1 col=5, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_rus_16__ },    /* [22] row=1 col=6, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_17__ },    /* [23] row=1 col=7, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_18__ },    /* [24] row=1 col=8, pages=1, 5 bytes */
	{ 6, 6, __glyph_base_6x6_rus_19__ },    /* [25] row=1 col=9, pages=1, 6 bytes */
	{ 5, 6, __glyph_base_6x6_rus_1a__ },    /* [26] row=1 col=10, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_rus_1b__ },    /* [27] row=1 col=11, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_1c__ },    /* [28] row=1 col=12, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_1d__ },    /* [29] row=1 col=13, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_1e__ },    /* [30] row=1 col=14, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_1f__ },    /* [31] row=1 col=15, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_20__ },    /* [32] row=2 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_21__ },    /* [33] row=2 col=1, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_22__ },    /* [34] row=2 col=2, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_23__ },    /* [35] row=2 col=3, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_24__ },    /* [36] row=2 col=4, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_25__ },    /* [37] row=2 col=5, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_26__ },    /* [38] row=2 col=6, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_27__ },    /* [39] row=2 col=7, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_28__ },    /* [40] row=2 col=8, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_29__ },    /* [41] row=2 col=9, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_2a__ },    /* [42] row=2 col=10, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_2b__ },    /* [43] row=2 col=11, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_2c__ },    /* [44] row=2 col=12, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_2d__ },    /* [45] row=2 col=13, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_2e__ },    /* [46] row=2 col=14, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_2f__ },    /* [47] row=2 col=15, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_30__ },    /* [48] row=3 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_31__ },    /* [49] row=3 col=1, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_32__ },    /* [50] row=3 col=2, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_33__ },    /* [51] row=3 col=3, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_34__ },    /* [52] row=3 col=4, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_rus_35__ },    /* [53] row=3 col=5, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_rus_36__ },    /* [54] row=3 col=6, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_37__ },    /* [55] row=3 col=7, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_38__ },    /* [56] row=3 col=8, pages=1, 5 bytes */
	{ 6, 6, __glyph_base_6x6_rus_39__ },    /* [57] row=3 col=9, pages=1, 6 bytes */
	{ 5, 6, __glyph_base_6x6_rus_3a__ },    /* [58] row=3 col=10, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_rus_3b__ },    /* [59] row=3 col=11, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_3c__ },    /* [60] row=3 col=12, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_3d__ },    /* [61] row=3 col=13, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_rus_3e__ },    /* [62] row=3 col=14, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_rus_3f__ },    /* [63] row=3 col=15, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_40__ },    /* [64] row=4 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_rus_41__ },    /* [65] row=4 col=1, pages=1, 4 bytes */
};
