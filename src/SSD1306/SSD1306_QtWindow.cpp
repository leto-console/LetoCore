/*
 * SSD1306_QtWindow.cpp
 *
 *  Created on: Feb 07, 2025
 *      Author: Timur
 */

#ifdef __LINUX__

#include "SSD1306/SSD1306_QtWindow.hpp"

#include <QPen>
#include <QPainter>

/* Функция определения темного режиме (Пока не нужна) */
// #include <QGuiApplication>
// #include <QPalette>
// #include <QStyleHints>
// inline bool isDarkMode() {
// #if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
//   const auto scheme = QGuiApplication::styleHints()->colorScheme();
//   return scheme == Qt::ColorScheme::Dark;
// #else
//   const QPalette defaultPalette;
//   const auto text = defaultPalette.color(QPalette::WindowText);
//   const auto window = defaultPalette.color(QPalette::Window);
//   return text.lightness() > window.lightness();
// #endif // QT_VERSION
// }

SSD1306_QtWindow::SSD1306_QtWindow(QWidget *parent) 
    : QMainWindow(parent), isOpen{ true }
{
    this->setMinimumHeight(SSD1306_Height * SSD1306_POINT_WIDTH);
    this->setMinimumWidth(SSD1306_Width * SSD1306_POINT_WIDTH);
    this->setMaximumHeight(SSD1306_Height * SSD1306_POINT_WIDTH);
    this->setMaximumWidth(SSD1306_Width * SSD1306_POINT_WIDTH);
    this->show();

    enablePen = QPen(QColor(255, 255, 255, 255), SSD1306_POINT_WIDTH);
    disablePen = QPen(QColor(0, 0, 0, 255), SSD1306_POINT_WIDTH);
}

SSD1306_QtWindow::~SSD1306_QtWindow()
{
}

bool SSD1306_QtWindow::IsOpen()
{
    return isOpen;
}

bool SSD1306_QtWindow::Init()
{
	return true;
}

bool SSD1306_QtWindow::Loop()
{
    update();
    return isOpen;
}

#define __GET_BIT(n, b) ((n >> b) & 1)

void SSD1306_QtWindow::Update(SSD1306_Screen& screen, uint8_t page, uint8_t column, uint8_t count)
{
    (void)column;
    (void)count;

    if (page >= SSD1306_PagesCount) return;

	//for (size_t page = 0; page != SSD1306_PagesCount; ++page)
	//{
		for (size_t b = 0; b != 8; ++b)
		{
			for (size_t col = 0; col != SSD1306_ColumnsCount; ++col)
			{
				displayBuffer[col][page * 8 + b] = (__GET_BIT(screen.pages[page].columns[col], b) ? enablePen : disablePen);
			}
		}
	//}
}

void SSD1306_QtWindow::closeEvent(QCloseEvent* event)
{
    QMainWindow::closeEvent(event);
    isOpen = false;
    event->accept();
}

void SSD1306_QtWindow::paintEvent(QPaintEvent* event)
{    
    QPainter painter(this);

    for (int x = 0; x != SSD1306_Width; ++x)
    {
        for (int y = 0; y != SSD1306_Height; ++y)
        {
            painter.setPen(displayBuffer[x][y]);
            qreal x_ = SSD1306_POINT_WIDTH / 2 + x * SSD1306_POINT_WIDTH;
            qreal y_ = SSD1306_POINT_WIDTH / 2 + y * SSD1306_POINT_WIDTH;
            painter.drawPoint(QPointF(x_, y_));
        }
    }
    event->accept();
}

#endif
