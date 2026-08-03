/**
 * @file WinDisplay.hpp
 * @date Apr 23, 2026
 * @author Rakhimov T.
 */

#ifdef __WIN__

#ifndef GRAPHICS_PC_WIN_DISPLAY_HPP_
#define GRAPHICS_PC_WIN_DISPLAY_HPP_

#include "LibrariesExport.h"

#include <Windows.h>
#include <stdint.h>
#include <deque>
#include <map>

#include <Graphics/IDisplay.hpp>

LRESULT CALLBACK	WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				DrawImage(HDC hdc);

struct LETO_CORE_EXPORT WinAPIKeyEvent
{
	enum { IDLE, PRESSED, RELEASED };
	uint64_t key;
	int event;
};

extern LETO_CORE_EXPORT std::map<uint64_t, std::deque<WinAPIKeyEvent>> WinAPIEventsMap;

class LETO_CORE_EXPORT WinDisplay : public IDisplay
{
	friend class SSD1306;
	const wchar_t*	CLASS_NAME = L"STM32_GAMES";
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;

public:
	WinDisplay(int width, int height, int scale);
	~WinDisplay();

	WinDisplay(const WinDisplay&) = delete;
    WinDisplay& operator =(const WinDisplay&) = delete;

    int Width() const override;
    int Height() const override;

	bool IsOpen() override;
	bool Init() override;
	void Loop() override;
    bool Render(IDrawer* drawer) override;
};

#endif /* GRAPICS_PC_WIN_DISPLAY_HPP_ */

#endif /* __WIN__ */
