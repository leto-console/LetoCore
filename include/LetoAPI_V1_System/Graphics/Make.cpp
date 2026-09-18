#include "Make.hpp"

#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <utility>
#include <Graphics/IScreen.hpp>

// ====================================================================================================

static void DrawBitmap(LetoScreen_V1* screen, int32_t x, int32_t y, const LetoBitmap_V1* bitmap, LetoColor_V1 color, bool inverse)
{
    if (!screen || !bitmap) return;

    IScreen& impl_screen = *IScreen::FromHandle(screen);
    const BitmapData& data = *BitmapData::FromHandle(bitmap);

    // Ранний выход если полностью за экраном
    if (x + data.width <= 0 || y + data.height <= 0 || 
        x >= impl_screen.Width() || y >= impl_screen.Height())
        return;

    for (int _x = 0; _x < data.width; ++_x)
    {
        for (int _y = 0; _y < data.height; ++_y)
        {
            if (data.GetPixel(_x, _y) != inverse)
                impl_screen.PixelSet(x + _x, y + _y, color);
        }
    }
}

static void DrawPixel(LetoScreen_V1* screen, int32_t x, int32_t y, LetoColor_V1 color)
{
    if (!screen) return;
    IScreen& impl_screen = *IScreen::FromHandle(screen);

    impl_screen.PixelSet({x, y}, color);
}

// Вспомогательная функция для обмена значений
static inline void SwapInt(int32_t& a, int32_t& b)
{
    int32_t temp = a;
    a = b;
    b = temp;
}

// Алгоритм Брезенхема для линии с поддержкой толщины
static void DrawLine(LetoScreen_V1* screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t thickness, LetoColor_V1 color)
{
    if (!screen) return;
    IScreen& impl_screen = *IScreen::FromHandle(screen);

    // Если толщина <= 1, используем классический алгоритм Брезенхема
    if (thickness <= 1)
    {
        int32_t dx = std::abs(x2 - x1);
        int32_t dy = std::abs(y2 - y1);
        int32_t sx = (x1 < x2) ? 1 : -1;
        int32_t sy = (y1 < y2) ? 1 : -1;
        int32_t err = (dx > dy ? dx : -dy) / 2;

        while (true)
        {
            impl_screen.PixelSet(x1, y1, color);
            
            if (x1 == x2 && y1 == y2) break;
            
            int32_t e2 = err;
            if (e2 > -dx) { err -= dy; x1 += sx; }
            if (e2 < dy) { err += dx; y1 += sy; }
        }
    }
    else
    {
        // Для толстых линий: рисуем несколько параллельных линий
        int32_t dx = x2 - x1;
        int32_t dy = y2 - y1;
        int32_t len = std::max(std::abs(dx), std::abs(dy));
        
        if (len == 0) return;
        
        // Нормализованный перпендикуляр
        float perp_x = -dy / static_cast<float>(len);
        float perp_y = dx / static_cast<float>(len);
        
        int32_t half_thickness = thickness / 2;
        
        // Рисуем несколько линий со смещением
        for (int32_t t = -half_thickness; t <= half_thickness; ++t)
        {
            int32_t offset_x = static_cast<int32_t>(perp_x * t);
            int32_t offset_y = static_cast<int32_t>(perp_y * t);
            
            // Используем Брезенхема для каждой параллельной линии
            int32_t lx1 = x1 + offset_x;
            int32_t ly1 = y1 + offset_y;
            int32_t lx2 = x2 + offset_x;
            int32_t ly2 = y2 + offset_y;
            
            int32_t ldx = std::abs(lx2 - lx1);
            int32_t ldy = std::abs(ly2 - ly1);
            int32_t lsx = (lx1 < lx2) ? 1 : -1;
            int32_t lsy = (ly1 < ly2) ? 1 : -1;
            int32_t lerr = (ldx > ldy ? ldx : -ldy) / 2;
            
            int32_t cx = lx1, cy = ly1;
            while (true)
            {
                impl_screen.PixelSet(cx, cy, color);
                
                if (cx == lx2 && cy == ly2) break;
                
                int32_t e2 = lerr;
                if (e2 > -ldx) { lerr -= ldy; cx += lsx; }
                if (e2 < ldy) { lerr += ldx; cy += lsy; }
            }
        }
    }
}

// Оптимизированная версия DrawRect
static void DrawRect(LetoScreen_V1* screen, int32_t x1, int32_t y1, int32_t width, int32_t height, int32_t thickness, LetoColor_V1 color)
{
    if (!screen || width <= 0 || height <= 0) return;
    IScreen& impl_screen = *IScreen::FromHandle(screen);

    int32_t x2 = x1 + width - 1;
    int32_t y2 = y1 + height - 1;

    if (thickness == 0)
    {
        // Заполненный прямоугольник - рисуем построчно
        for (int32_t y = y1; y <= y2; ++y)
        {
            for (int32_t x = x1; x <= x2; ++x)
            {
                impl_screen.PixelSet(x, y, color);
            }
        }
    }
    else
    {
        // Контур - рисуем 4 линии
        // Верхняя и нижняя границы
        for (int32_t x = x1; x <= x2; ++x)
        {
            impl_screen.PixelSet(x, y1, color);
            impl_screen.PixelSet(x, y2, color);
        }
        // Левая и правая границы (без углов, чтобы не дублировать)
        for (int32_t y = y1 + 1; y < y2; ++y)
        {
            impl_screen.PixelSet(x1, y, color);
            impl_screen.PixelSet(x2, y, color);
        }
    }
}

//#define _min(a,b) ((a)<(b)?(a):(b))
//#define _max(a,b) ((a)>(b)?(a):(b))

static void DrawRoundRect(LetoScreen_V1* screen, int32_t x, int32_t y, int32_t width, int32_t height, int32_t radius, int32_t thickness, LetoColor_V1 color)
{
    if (!screen || width <= 0 || height <= 0) return;
    
    IScreen& impl_screen = *IScreen::FromHandle(screen);
    
    // Корректировка радиуса
    int32_t max_r = std::min(width / 2, height / 2);
    if (radius > max_r) radius = max_r;
    if (radius < 0) radius = 0;
    
    int32_t x2 = x + width - 1;
    int32_t y2 = y + height - 1;
    
    if (thickness == 0)
    {
        // ЗАПОЛНЕННЫЙ скругленный прямоугольник
        
        // Центральная часть (прямоугольник без углов)
        for (int32_t cy = y + radius; cy <= y2 - radius; ++cy)
        {
            for (int32_t cx = x; cx <= x2; ++cx)
            {
                impl_screen.PixelSet(cx, cy, color);
            }
        }
        
        // Боковые части (верх и низ без углов)
        for (int32_t cy = y; cy < y + radius; ++cy)
        {
            for (int32_t cx = x + radius; cx <= x2 - radius; ++cx)
            {
                impl_screen.PixelSet(cx, cy, color);
                impl_screen.PixelSet(cx, y2 - (cy - y), color);
            }
        }
        
        // Углы (четверти кругов)
        // Используем тот же принцип, что и в заполненном эллипсе, но для 4 углов
        int r_sq = radius * radius;
        for (int32_t dy = 0; dy <= radius; ++dy)
        {
            // Находим максимальный dx для текущего dy
            int32_t dx_max = static_cast<int32_t>(std::sqrt(static_cast<float>(r_sq - dy * dy)) + 0.5f);
            
            for (int32_t dx = 0; dx <= dx_max; ++dx)
            {
                // Верхний левый
                impl_screen.PixelSet(x + radius - dx, y + radius - dy, color);
                // Верхний правый
                impl_screen.PixelSet(x2 - radius + dx, y + radius - dy, color);
                // Нижний левый
                impl_screen.PixelSet(x + radius - dx, y2 - radius + dy, color);
                // Нижний правый
                impl_screen.PixelSet(x2 - radius + dx, y2 - radius + dy, color);
            }
        }
    }
    else
    {
        // КОНТУР скругленного прямоугольника
        
        // Прямые линии
        // Верх и низ
        for (int32_t cx = x + radius; cx <= x2 - radius; ++cx)
        {
            impl_screen.PixelSet(cx, y, color);
            impl_screen.PixelSet(cx, y2, color);
        }
        // Лево и право
        for (int32_t cy = y + radius; cy <= y2 - radius; ++cy)
        {
            impl_screen.PixelSet(x, cy, color);
            impl_screen.PixelSet(x2, cy, color);
        }
        
        // Более надежный способ для маленьких радиусов - увеличенный шаг тригонометрии
        // 360 градусов / (2 * PI * R) ~ точек нужно. Возьмем с запасом.
        int steps = (radius * 8 > 32) ? radius * 8 : 32; 
        for (int i = 0; i <= steps; ++i)
        {
            float angle = 0.5f * 3.14159265f * i / steps; // 0 to PI/2
            int32_t dx = static_cast<int32_t>(radius * std::cos(angle) + 0.5f);
            int32_t dy = static_cast<int32_t>(radius * std::sin(angle) + 0.5f);
            
            // TL
            impl_screen.PixelSet(x + radius - dx, y + radius - dy, color);
            // TR
            impl_screen.PixelSet(x2 - radius + dx, y + radius - dy, color);
            // BL
            impl_screen.PixelSet(x + radius - dx, y2 - radius + dy, color);
            // BR
            impl_screen.PixelSet(x2 - radius + dx, y2 - radius + dy, color);
        }
    }
}

static void PlotEllipsePoints(IScreen& screen, int32_t cx, int32_t cy, int32_t x, int32_t y, LetoColor_V1 color)
{
    screen.PixelSet(cx + x, cy + y, color);
    screen.PixelSet(cx - x, cy + y, color);
    screen.PixelSet(cx + x, cy - y, color);
    screen.PixelSet(cx - x, cy - y, color);
}

static void DrawEllipse(LetoScreen_V1* screen, int32_t centerX, int32_t centerY, int32_t width, int32_t height, int32_t thickness, LetoColor_V1 color)
{
    if (!screen || width <= 0 || height <= 0) return;
    
    IScreen& impl_screen = *IScreen::FromHandle(screen);
    
    int32_t a = width / 2;   // полуось по X
    int32_t b = height / 2;  // полуось по Y
    
    float a_f = static_cast<float>(a);
    float b_f = static_cast<float>(b);
    float a_sq = a_f * a_f;
    float b_sq = b_f * b_f;

    if (thickness == 0)
    {
        // Заполненный эллипс (сканирование по Y)
        for (int32_t y = -b; y <= b; ++y)
        {
            float y_f = static_cast<float>(y);
            // x^2 / a^2 + y^2 / b^2 = 1  =>  x = a * sqrt(1 - y^2/b^2)
            float val = 1.0f - (y_f * y_f) / b_sq;
            if (val < 0) val = 0;
            
            int32_t x_extent = static_cast<int32_t>(a_f * std::sqrt(val) + 0.5f);
            
            for (int32_t x = -x_extent; x <= x_extent; ++x)
            {
                impl_screen.PixelSet(centerX + x, centerY + y, color);
            }
        }
    }
    else
    {
        // Контур эллипса - Алгоритм средней точки
        int32_t x = 0;
        int32_t y = b;
        
        int64_t p1 = b_sq - (a_sq * b) + (a_sq / 4); 
        int64_t dx = 0;
        int64_t dy = 2 * a_sq * y;
        
        PlotEllipsePoints(impl_screen, centerX, centerY, x, y, color);
        
        while (dx < dy)
        {
            x++;
            dx += 2 * b_sq;
            
            if (p1 < 0)
            {
                p1 += dx + b_sq;
            }
            else
            {
                y--;
                dy -= 2 * a_sq;
                p1 += dx - dy + b_sq;
            }
            PlotEllipsePoints(impl_screen, centerX, centerY, x, y, color);
        }
        
        int64_t p2 = b_sq * (x + 0.5) * (x + 0.5) + a_sq * (y - 1) * (y - 1) - a_sq * b_sq;
        
        while (y > 0)
        {
            y--;
            dy -= 2 * a_sq;
            
            if (p2 > 0)
            {
                p2 += a_sq - dy;
            }
            else
            {
                x++;
                dx += 2 * b_sq;
                p2 += dx - dy + a_sq;
            }
            PlotEllipsePoints(impl_screen, centerX, centerY, x, y, color);
        }
    }
}

// Алгоритм заполнения треугольника (scanline)
static void FillTriangle(IScreen& screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, LetoColor_V1 color)
{
    // Сортируем вершины по Y
    if (y1 > y2) { SwapInt(y1, y2); SwapInt(x1, x2); }
    if (y1 > y3) { SwapInt(y1, y3); SwapInt(x1, x3); }
    if (y2 > y3) { SwapInt(y2, y3); SwapInt(x2, x3); }
    
    if (y1 == y3) return; // вырожденный треугольник
    
    // Вычисляем шаги для интерполяции X
    float dx_left, dx_right;
    int32_t x_left, x_right;
    
    // Верхняя часть треугольника (от y1 до y2)
    if (y2 != y1)
    {
        float slope1 = static_cast<float>(x2 - x1) / (y2 - y1);
        float slope2 = static_cast<float>(x3 - x1) / (y3 - y1);
        
        for (int32_t y = y1; y <= y2; ++y)
        {
            x_left = static_cast<int32_t>(x1 + slope1 * (y - y1) + 0.5f);
            x_right = static_cast<int32_t>(x1 + slope2 * (y - y1) + 0.5f);
            
            if (x_left > x_right) SwapInt(x_left, x_right);
            
            for (int32_t x = x_left; x <= x_right; ++x)
            {
                screen.PixelSet(x, y, color);
            }
        }
    }
    
    // Нижняя часть треугольника (от y2 до y3)
    if (y3 != y2)
    {
        float slope1 = static_cast<float>(x3 - x2) / (y3 - y2);
        float slope2_saved = static_cast<float>(x3 - x1) / (y3 - y1);
        
        for (int32_t y = y2; y <= y3; ++y)
        {
            x_left = static_cast<int32_t>(x2 + slope1 * (y - y2) + 0.5f);
            x_right = static_cast<int32_t>(x1 + slope2_saved * (y - y1) + 0.5f);
            
            if (x_left > x_right) SwapInt(x_left, x_right);
            
            for (int32_t x = x_left; x <= x_right; ++x)
            {
                screen.PixelSet(x, y, color);
            }
        }
    }
}

static void DrawTriangle(LetoScreen_V1* screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t thickness, LetoColor_V1 color)
{
    if (!screen) return;
    IScreen& impl_screen = *IScreen::FromHandle(screen);
    
    if (thickness == 0)
    {
        // Заполненный треугольник
        FillTriangle(impl_screen, x1, y1, x2, y2, x3, y3, color);
    }
    else
    {
        // Контур треугольника
        DrawLine(screen, x1, y1, x2, y2, thickness, color);
        DrawLine(screen, x2, y2, x3, y3, thickness, color);
        DrawLine(screen, x3, y3, x1, y1, thickness, color);
    }
}

// ====================================================================================================

const GraphicsAPI_V1* Make_GraphicsAPI()
{
    static const GraphicsAPI_V1 api
    {
        &DrawBitmap,
        &DrawPixel,
        &DrawLine,
        &DrawRect,
        &DrawRoundRect,
        &DrawEllipse,
        &DrawTriangle
    };
    
    return &api;
}