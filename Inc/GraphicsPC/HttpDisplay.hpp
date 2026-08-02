/**
 * @file HttpDisplay.hpp
 * @date May 10, 2026
 * @author Rakhimov T.
 */

#ifdef __TERMUX__

#ifndef GRAPHICS_PC_HTTP_DISPLAY_HPP_
#define GRAPHICS_PC_HTTP_DISPLAY_HPP_

#include "LibrariesExport.h"
#include <thread>

#include <Graphics/IDisplay.hpp>

class LETO_CORE_EXPORT HttpDisplay : public IDisplay
{
protected:
	std::thread http_server_thread;

public:
	HttpDisplay(int width, int height, int scale);
	~HttpDisplay();

    int Width() const override;
    int Height() const override;

	bool IsOpen() override;
	bool Init() override;
	void Loop() override;

    bool Render(IDrawer* drawer) override;
};

#endif /* INC_SSD1306_SSD1306_HTTP_WINDOW_HPP_ */

#endif /* __TERMUX__ */
