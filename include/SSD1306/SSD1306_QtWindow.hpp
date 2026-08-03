/*
 * SSD1306_QtWindow.hpp
 *
 *  Created on: Feb 06, 2025
 *      Author: Timur
 */

#ifndef SSD1306_SSD1306_QTWINDOW_HPP_
#define SSD1306_SSD1306_QTWINDOW_HPP_

#ifdef __LINUX__

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPen>

#include "SSD1306/SSD1306_Window.hpp"
#include "SSD1306/SSD1306_Screen.hpp"
#include "SSD1306/SSD1306_Properties.hpp"

#define SSD1306_POINT_WIDTH 3
/*
    Реализация имитатора SSD1306 на основе Qt
*/
class SSD1306_QtWindow : public SSD1306_Window, protected QMainWindow
{
protected:
    QPen displayBuffer[SSD1306_Width][SSD1306_Height];
    QPen enablePen;
    QPen disablePen;
    
    bool isOpen;
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
public:
    SSD1306_QtWindow(QWidget *parent = 0);
    ~SSD1306_QtWindow();

    bool IsOpen() override;
    bool Init() override;
    bool Loop() override;
    void Update(SSD1306_Screen& screen, uint8_t page, uint8_t column = 0, uint8_t count = SSD1306_Width) override;
};

#endif /* __LINUX__ */

#endif /* SSD1306_SSD1306_QTWINDOW_HPP_ */
