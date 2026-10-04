/*
 * Сгенерировано fontmap2c.py — НЕ РЕДАКТИРОВАТЬ ВРУЧНУЮ.
 * Источник: base_6x6_ascii.bmp (96x43 px, 24 bpp, BI_RGB, bottom-up)
 * Сетка: 7 горизонтальных линий x 17 вертикальных -> 6 строк x 16 столбцов
 * Внутренние размеры ячеек: width=[1, 2, 3, 4, 5, 6] px, height=[6] px
 * Глифов: 96, байт битмапов: 374
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

#include "base_6x6_ascii.h"

// ----------------------------------------------------------------------
// Приватные битмапы глифов: static — не видны за пределами этого файла.
// Имя: __glyph_<prefix>_<hex row><hex col>__, row/col — позиция в сетке,
// 0-based. Отдельные битмапы не объявлены в .h: это детали реализации.
// ----------------------------------------------------------------------

/* glyph [row=0 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_00__[] = {
	0xC0, 0xC0, 0xC0, 0xC0,
};

/* glyph [row=0 col=1] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_01__[] = {
	0xC0, 0xEF, 0xC0,
};

/* glyph [row=0 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_02__[] = {
	0xC3, 0xC0, 0xC3, 0xC0,
};

/* glyph [row=0 col=3] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_03__[] = {
	0xCA, 0xFF, 0xCA, 0xFF, 0xCA,
};

/* glyph [row=0 col=4] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_04__[] = {
	0xC0, 0xC0, 0xC0, 0xC0,
};

/* glyph [row=0 col=5] 6x6 px, pages=1, 6 bytes */
static const uint8_t __glyph_base_6x6_ascii_05__[] = {
	0xE3, 0xD3, 0xC8, 0xC4, 0xF2, 0xF1,
};

/* glyph [row=0 col=6] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_06__[] = {
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
};

/* glyph [row=0 col=7] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_07__[] = {
	0xC0, 0xC3, 0xC0,
};

/* glyph [row=0 col=8] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_08__[] = {
	0xDE, 0xE1,
};

/* glyph [row=0 col=9] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_09__[] = {
	0xE1, 0xDE,
};

/* glyph [row=0 col=10] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_0a__[] = {
	0xCA, 0xC7, 0xCA,
};

/* glyph [row=0 col=11] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_0b__[] = {
	0xC4, 0xC4, 0xDF, 0xC4, 0xC4,
};

/* glyph [row=0 col=12] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_0c__[] = {
	0xD0, 0xF0,
};

/* glyph [row=0 col=13] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_0d__[] = {
	0xC4, 0xC4, 0xC4, 0xC4,
};

/* glyph [row=0 col=14] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_0e__[] = {
	0xF0, 0xF0,
};

/* glyph [row=0 col=15] 6x6 px, pages=1, 6 bytes */
static const uint8_t __glyph_base_6x6_ascii_0f__[] = {
	0xE0, 0xD0, 0xC8, 0xC4, 0xC2, 0xC1,
};

/* glyph [row=1 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_10__[] = {
	0xDE, 0xE9, 0xE5, 0xDE,
};

/* glyph [row=1 col=1] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_11__[] = {
	0xC4, 0xC2, 0xFF,
};

/* glyph [row=1 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_12__[] = {
	0xE2, 0xF1, 0xE9, 0xE6,
};

/* glyph [row=1 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_13__[] = {
	0xD2, 0xE1, 0xE5, 0xDA,
};

/* glyph [row=1 col=4] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_14__[] = {
	0xC7, 0xC4, 0xC4, 0xFF,
};

/* glyph [row=1 col=5] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_15__[] = {
	0xD7, 0xE5, 0xE5, 0xD9,
};

/* glyph [row=1 col=6] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_16__[] = {
	0xDE, 0xE5, 0xE5, 0xD8,
};

/* glyph [row=1 col=7] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_17__[] = {
	0xC1, 0xF9, 0xC5, 0xC3,
};

/* glyph [row=1 col=8] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_18__[] = {
	0xDA, 0xE5, 0xE5, 0xDA,
};

/* glyph [row=1 col=9] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_19__[] = {
	0xC6, 0xE9, 0xE9, 0xDE,
};

/* glyph [row=1 col=10] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_1a__[] = {
	0xF6, 0xF6,
};

/* glyph [row=1 col=11] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_1b__[] = {
	0xD6, 0xF6,
};

/* glyph [row=1 col=12] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_1c__[] = {
	0xC8, 0xD4, 0xE2, 0xC0,
};

/* glyph [row=1 col=13] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_1d__[] = {
	0xD4, 0xD4, 0xD4, 0xD4,
};

/* glyph [row=1 col=14] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_1e__[] = {
	0xE2, 0xD4, 0xC8,
};

/* glyph [row=1 col=15] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_1f__[] = {
	0xC2, 0xE9, 0xC5, 0xC2,
};

/* glyph [row=2 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_20__[] = {
	0xC0, 0xC0, 0xC0, 0xC0,
};

/* glyph [row=2 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_21__[] = {
	0xFE, 0xC9, 0xC9, 0xFE,
};

/* glyph [row=2 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_22__[] = {
	0xFF, 0xE5, 0xE5, 0xDA,
};

/* glyph [row=2 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_23__[] = {
	0xDE, 0xE1, 0xE1, 0xE1,
};

/* glyph [row=2 col=4] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_24__[] = {
	0xFF, 0xE1, 0xE1, 0xDE,
};

/* glyph [row=2 col=5] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_25__[] = {
	0xDE, 0xE9, 0xE9, 0xE6,
};

/* glyph [row=2 col=6] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_26__[] = {
	0xFF, 0xC5, 0xC5, 0xC1,
};

/* glyph [row=2 col=7] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_27__[] = {
	0xDE, 0xE1, 0xE9, 0xDA,
};

/* glyph [row=2 col=8] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_28__[] = {
	0xFF, 0xC4, 0xC4, 0xFF,
};

/* glyph [row=2 col=9] 1x6 px, pages=1, 1 bytes, EMPTY */
static const uint8_t __glyph_base_6x6_ascii_29__[] = {
	0xFF,
};

/* glyph [row=2 col=10] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_2a__[] = {
	0xD0, 0xE0, 0xE0, 0xDF,
};

/* glyph [row=2 col=11] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_2b__[] = {
	0xFF, 0xC4, 0xCA, 0xF1,
};

/* glyph [row=2 col=12] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_2c__[] = {
	0xFF, 0xE0, 0xE0, 0xE0,
};

/* glyph [row=2 col=13] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_2d__[] = {
	0xFF, 0xC6, 0xD8, 0xC6, 0xFF,
};

/* glyph [row=2 col=14] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_2e__[] = {
	0xFF, 0xC4, 0xC8, 0xFF,
};

/* glyph [row=2 col=15] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_2f__[] = {
	0xDE, 0xE1, 0xE1, 0xDE,
};

/* glyph [row=3 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_30__[] = {
	0xFF, 0xC9, 0xC9, 0xC6,
};

/* glyph [row=3 col=1] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_31__[] = {
	0xDE, 0xE1, 0xE1, 0xFE, 0xE0,
};

/* glyph [row=3 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_32__[] = {
	0xFF, 0xC9, 0xD9, 0xE6,
};

/* glyph [row=3 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_33__[] = {
	0xD2, 0xE5, 0xE9, 0xD2,
};

/* glyph [row=3 col=4] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_34__[] = {
	0xC1, 0xC1, 0xFF, 0xC1, 0xC1,
};

/* glyph [row=3 col=5] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_35__[] = {
	0xDF, 0xE0, 0xE0, 0xDF,
};

/* glyph [row=3 col=6] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_36__[] = {
	0xC7, 0xC8, 0xF0, 0xC8, 0xC7,
};

/* glyph [row=3 col=7] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_37__[] = {
	0xFF, 0xD0, 0xCC, 0xD0, 0xFF,
};

/* glyph [row=3 col=8] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_38__[] = {
	0xF1, 0xCA, 0xC4, 0xCA, 0xF1,
};

/* glyph [row=3 col=9] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_39__[] = {
	0xC3, 0xC4, 0xF8, 0xC4, 0xC3,
};

/* glyph [row=3 col=10] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_3a__[] = {
	0xF1, 0xE9, 0xE5, 0xE3,
};

/* glyph [row=3 col=11] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_3b__[] = {
	0xFF, 0xE1,
};

/* glyph [row=3 col=12] 6x6 px, pages=1, 6 bytes */
static const uint8_t __glyph_base_6x6_ascii_3c__[] = {
	0xC1, 0xC2, 0xC4, 0xC8, 0xD0, 0xE0,
};

/* glyph [row=3 col=13] 2x6 px, pages=1, 2 bytes */
static const uint8_t __glyph_base_6x6_ascii_3d__[] = {
	0xE1, 0xFF,
};

/* glyph [row=3 col=14] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_3e__[] = {
	0xC4, 0xC2, 0xC1, 0xC2, 0xC4,
};

/* glyph [row=3 col=15] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_3f__[] = {
	0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
};

/* glyph [row=4 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_40__[] = {
	0xC0, 0xC1, 0xC2, 0xC0,
};

/* glyph [row=4 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_41__[] = {
	0xD0, 0xEA, 0xEA, 0xFC,
};

/* glyph [row=4 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_42__[] = {
	0xFE, 0xE8, 0xE8, 0xD0,
};

/* glyph [row=4 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_43__[] = {
	0xD8, 0xE4, 0xE4, 0xE4,
};

/* glyph [row=4 col=4] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_44__[] = {
	0xD0, 0xE8, 0xE8, 0xFE,
};

/* glyph [row=4 col=5] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_45__[] = {
	0xDC, 0xEA, 0xEA, 0xE4,
};

/* glyph [row=4 col=6] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_46__[] = {
	0xFC, 0xCA, 0xCA,
};

/* glyph [row=4 col=7] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_47__[] = {
	0xE4, 0xEA, 0xEA, 0xDC,
};

/* glyph [row=4 col=8] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_48__[] = {
	0xFE, 0xC8, 0xC8, 0xF0,
};

/* glyph [row=4 col=9] 1x6 px, pages=1, 1 bytes */
static const uint8_t __glyph_base_6x6_ascii_49__[] = {
	0xFA,
};

/* glyph [row=4 col=10] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_4a__[] = {
	0xD0, 0xE0, 0xDA, 0xC0,
};

/* glyph [row=4 col=11] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_4b__[] = {
	0xFE, 0xC8, 0xD4, 0xE2,
};

/* glyph [row=4 col=12] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_4c__[] = {
	0xDE, 0xE0, 0xD0,
};

/* glyph [row=4 col=13] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_4d__[] = {
	0xF8, 0xC6, 0xC8, 0xC6, 0xF8,
};

/* glyph [row=4 col=14] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_4e__[] = {
	0xFE, 0xC2, 0xC2, 0xFC,
};

/* glyph [row=4 col=15] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_4f__[] = {
	0xDC, 0xE2, 0xE2, 0xDC,
};

/* glyph [row=5 col=0] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_50__[] = {
	0xFE, 0xCA, 0xCA, 0xC4,
};

/* glyph [row=5 col=1] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_51__[] = {
	0xC4, 0xCA, 0xCA, 0xFC,
};

/* glyph [row=5 col=2] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_52__[] = {
	0xFE, 0xC4, 0xC2, 0xC4,
};

/* glyph [row=5 col=3] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_53__[] = {
	0xE4, 0xEA, 0xEA, 0xD2,
};

/* glyph [row=5 col=4] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_54__[] = {
	0xC4, 0xDF, 0xE4, 0xD0,
};

/* glyph [row=5 col=5] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_55__[] = {
	0xDE, 0xE0, 0xE0, 0xFE,
};

/* glyph [row=5 col=6] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_56__[] = {
	0xCE, 0xD0, 0xE0, 0xD0, 0xCE,
};

/* glyph [row=5 col=7] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_57__[] = {
	0xDE, 0xE0, 0xD8, 0xE0, 0xDE,
};

/* glyph [row=5 col=8] 5x6 px, pages=1, 5 bytes */
static const uint8_t __glyph_base_6x6_ascii_58__[] = {
	0xE2, 0xD4, 0xC8, 0xD4, 0xE2,
};

/* glyph [row=5 col=9] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_59__[] = {
	0xE6, 0xE8, 0xE8, 0xDE,
};

/* glyph [row=5 col=10] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_5a__[] = {
	0xF2, 0xEA, 0xEA, 0xE6,
};

/* glyph [row=5 col=11] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_5b__[] = {
	0xC8, 0xF6, 0xE2,
};

/* glyph [row=5 col=12] 1x6 px, pages=1, 1 bytes */
static const uint8_t __glyph_base_6x6_ascii_5c__[] = {
	0xFE,
};

/* glyph [row=5 col=13] 3x6 px, pages=1, 3 bytes */
static const uint8_t __glyph_base_6x6_ascii_5d__[] = {
	0xE2, 0xF6, 0xC8,
};

/* glyph [row=5 col=14] 4x6 px, pages=1, 4 bytes */
static const uint8_t __glyph_base_6x6_ascii_5e__[] = {
	0xCC, 0xC4, 0xC8, 0xCC,
};

/* glyph [row=5 col=15] 6x6 px, pages=1, 6 bytes */
static const uint8_t __glyph_base_6x6_ascii_5f__[] = {
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
};

// ----------------------------------------------------------------------
// Публичный массив глифов: порядок = ячейки карты слева-направо, сверху-вниз.
// width/height — размеры строго по сетке рамки, а не по контенту символа.
// ----------------------------------------------------------------------
// Размер массива явный и совпадает с extern в "base_6x6_ascii.h": число глифов
// известно генератору, поэтому отдельная переменная count не нужна —
// потребитель берёт размер из типа: std::size(__glyph_base_6x6_ascii__) или
// sizeof(__glyph_base_6x6_ascii__) / sizeof(__glyph_base_6x6_ascii__[0]) == 96.
const GlyphData __glyph_base_6x6_ascii__[96] = {
	{ 4, 6, __glyph_base_6x6_ascii_00__ },    /* [0] row=0 col=0, pages=1, 4 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_01__ },    /* [1] row=0 col=1, pages=1, 3 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_02__ },    /* [2] row=0 col=2, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_03__ },    /* [3] row=0 col=3, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_04__ },    /* [4] row=0 col=4, pages=1, 4 bytes */
	{ 6, 6, __glyph_base_6x6_ascii_05__ },    /* [5] row=0 col=5, pages=1, 6 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_06__ },    /* [6] row=0 col=6, pages=1, 5 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_07__ },    /* [7] row=0 col=7, pages=1, 3 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_08__ },    /* [8] row=0 col=8, pages=1, 2 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_09__ },    /* [9] row=0 col=9, pages=1, 2 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_0a__ },    /* [10] row=0 col=10, pages=1, 3 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_0b__ },    /* [11] row=0 col=11, pages=1, 5 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_0c__ },    /* [12] row=0 col=12, pages=1, 2 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_0d__ },    /* [13] row=0 col=13, pages=1, 4 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_0e__ },    /* [14] row=0 col=14, pages=1, 2 bytes */
	{ 6, 6, __glyph_base_6x6_ascii_0f__ },    /* [15] row=0 col=15, pages=1, 6 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_10__ },    /* [16] row=1 col=0, pages=1, 4 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_11__ },    /* [17] row=1 col=1, pages=1, 3 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_12__ },    /* [18] row=1 col=2, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_13__ },    /* [19] row=1 col=3, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_14__ },    /* [20] row=1 col=4, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_15__ },    /* [21] row=1 col=5, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_16__ },    /* [22] row=1 col=6, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_17__ },    /* [23] row=1 col=7, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_18__ },    /* [24] row=1 col=8, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_19__ },    /* [25] row=1 col=9, pages=1, 4 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_1a__ },    /* [26] row=1 col=10, pages=1, 2 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_1b__ },    /* [27] row=1 col=11, pages=1, 2 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_1c__ },    /* [28] row=1 col=12, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_1d__ },    /* [29] row=1 col=13, pages=1, 4 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_1e__ },    /* [30] row=1 col=14, pages=1, 3 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_1f__ },    /* [31] row=1 col=15, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_20__ },    /* [32] row=2 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_21__ },    /* [33] row=2 col=1, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_22__ },    /* [34] row=2 col=2, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_23__ },    /* [35] row=2 col=3, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_24__ },    /* [36] row=2 col=4, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_25__ },    /* [37] row=2 col=5, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_26__ },    /* [38] row=2 col=6, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_27__ },    /* [39] row=2 col=7, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_28__ },    /* [40] row=2 col=8, pages=1, 4 bytes */
	{ 1, 6, __glyph_base_6x6_ascii_29__ },    /* [41] row=2 col=9, pages=1, 1 bytes, empty */
	{ 4, 6, __glyph_base_6x6_ascii_2a__ },    /* [42] row=2 col=10, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_2b__ },    /* [43] row=2 col=11, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_2c__ },    /* [44] row=2 col=12, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_2d__ },    /* [45] row=2 col=13, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_2e__ },    /* [46] row=2 col=14, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_2f__ },    /* [47] row=2 col=15, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_30__ },    /* [48] row=3 col=0, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_31__ },    /* [49] row=3 col=1, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_32__ },    /* [50] row=3 col=2, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_33__ },    /* [51] row=3 col=3, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_34__ },    /* [52] row=3 col=4, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_35__ },    /* [53] row=3 col=5, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_36__ },    /* [54] row=3 col=6, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_37__ },    /* [55] row=3 col=7, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_38__ },    /* [56] row=3 col=8, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_39__ },    /* [57] row=3 col=9, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_3a__ },    /* [58] row=3 col=10, pages=1, 4 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_3b__ },    /* [59] row=3 col=11, pages=1, 2 bytes */
	{ 6, 6, __glyph_base_6x6_ascii_3c__ },    /* [60] row=3 col=12, pages=1, 6 bytes */
	{ 2, 6, __glyph_base_6x6_ascii_3d__ },    /* [61] row=3 col=13, pages=1, 2 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_3e__ },    /* [62] row=3 col=14, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_3f__ },    /* [63] row=3 col=15, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_40__ },    /* [64] row=4 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_41__ },    /* [65] row=4 col=1, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_42__ },    /* [66] row=4 col=2, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_43__ },    /* [67] row=4 col=3, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_44__ },    /* [68] row=4 col=4, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_45__ },    /* [69] row=4 col=5, pages=1, 4 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_46__ },    /* [70] row=4 col=6, pages=1, 3 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_47__ },    /* [71] row=4 col=7, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_48__ },    /* [72] row=4 col=8, pages=1, 4 bytes */
	{ 1, 6, __glyph_base_6x6_ascii_49__ },    /* [73] row=4 col=9, pages=1, 1 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_4a__ },    /* [74] row=4 col=10, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_4b__ },    /* [75] row=4 col=11, pages=1, 4 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_4c__ },    /* [76] row=4 col=12, pages=1, 3 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_4d__ },    /* [77] row=4 col=13, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_4e__ },    /* [78] row=4 col=14, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_4f__ },    /* [79] row=4 col=15, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_50__ },    /* [80] row=5 col=0, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_51__ },    /* [81] row=5 col=1, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_52__ },    /* [82] row=5 col=2, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_53__ },    /* [83] row=5 col=3, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_54__ },    /* [84] row=5 col=4, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_55__ },    /* [85] row=5 col=5, pages=1, 4 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_56__ },    /* [86] row=5 col=6, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_57__ },    /* [87] row=5 col=7, pages=1, 5 bytes */
	{ 5, 6, __glyph_base_6x6_ascii_58__ },    /* [88] row=5 col=8, pages=1, 5 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_59__ },    /* [89] row=5 col=9, pages=1, 4 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_5a__ },    /* [90] row=5 col=10, pages=1, 4 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_5b__ },    /* [91] row=5 col=11, pages=1, 3 bytes */
	{ 1, 6, __glyph_base_6x6_ascii_5c__ },    /* [92] row=5 col=12, pages=1, 1 bytes */
	{ 3, 6, __glyph_base_6x6_ascii_5d__ },    /* [93] row=5 col=13, pages=1, 3 bytes */
	{ 4, 6, __glyph_base_6x6_ascii_5e__ },    /* [94] row=5 col=14, pages=1, 4 bytes */
	{ 6, 6, __glyph_base_6x6_ascii_5f__ },    /* [95] row=5 col=15, pages=1, 6 bytes */
};
