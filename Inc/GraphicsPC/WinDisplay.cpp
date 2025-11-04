#ifdef __WIN__

#include <string>

#ifndef UNICODE
#define UNICODE
#endif

#include <GraphicsPC/WinDisplay.hpp>

static bool closeFlag = false;

static int SCALE 	= 1;
static int WIDTH	= 128;
static int HEIGHT 	= 64;

static uint32_t* imageData{};

std::map<uint64_t, std::deque<WinAPIKeyEvent>> WinAPIEventsMap;

LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		closeFlag = false;
		break;
	case WM_MOVING:
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		// Флаг closeFlag добавлен для возможности перезапустить приложение
		closeFlag = true;
		//PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		WinAPIEventsMap[wParam].push_back({ wParam, WinAPIKeyEvent::PRESSED });
		//WinAPIEventsMap[wParam] = { wParam, WinAPIKeyEvent::PRESSED };
		//WinAPIKeyEvents.push_back({ wParam, WinAPIKeyEvent::PRESSED });
		break;
	case WM_KEYUP:
		WinAPIEventsMap[wParam].push_back({ wParam, WinAPIKeyEvent::RELEASED });
		//WinAPIEventsMap[wParam] = { wParam, WinAPIKeyEvent::RELEASED };
		//WinAPIKeyEvents.push_back({ wParam, WinAPIKeyEvent::RELEASED });
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		DrawImage(hdc);
		EndPaint(hWnd, &ps);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DrawImage(HDC hdc)
{
	if (!imageData) return;

	BITMAPINFOHEADER bmih	= { 0 };
	bmih.biSize				= sizeof(BITMAPINFOHEADER);
	bmih.biWidth			= WIDTH;
	bmih.biHeight			= -HEIGHT;
	bmih.biPlanes			= 1;
	bmih.biBitCount			= 32;
	bmih.biCompression		= BI_RGB;
	bmih.biSizeImage		= 0;
	bmih.biXPelsPerMeter	= 10;
	bmih.biYPelsPerMeter	= 10;

	BITMAPINFO dbmi			= { 0 };
	dbmi.bmiHeader			= bmih;

	// Draw pixels without stretching
  //  SetDIBitsToDevice(hdc, 0, 0, SSD1306_Width, SSD1306_Height,
  //                    0, 0, 0, SSD1306_Height, imageData, &dbmi, 0 );

	StretchDIBits(hdc, 
		0, 0, WIDTH * SCALE, HEIGHT * SCALE,
		0, 0, WIDTH, HEIGHT,
		imageData, &dbmi, DIB_RGB_COLORS, SRCCOPY);
}

WinDisplay::WinDisplay(int width, int height, int scale) :
	m_hInstance{ GetModuleHandle(nullptr) }
{
	SCALE = scale;
	WIDTH = width;
	HEIGHT = height;

	imageData = new uint32_t[WIDTH * HEIGHT]{};

}

WinDisplay::~WinDisplay()
{
	DestroyWindow(m_hWnd);
	UnregisterClass(CLASS_NAME, m_hInstance);
}

int WinDisplay::Width() const
{
	return WIDTH;
}

int WinDisplay::Height() const
{
	return HEIGHT;
}

bool WinDisplay::IsOpen()
{
	return !closeFlag;
}

bool WinDisplay::Init()
{
	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + WIDTH * SCALE;
	rect.bottom = rect.top + HEIGHT * SCALE;

	AdjustWindowRect(&rect, style, false);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"STM_INS",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}

void WinDisplay::Loop()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }
}

#include "BufferScreen.hpp"
#include "WinDisplay.hpp"

bool WinDisplay::Render(IDrawer* drawer)
{
	static BufferScreen screen;
	screen.SetSize(WIDTH, HEIGHT);

	drawer->Draw(screen);

	memcpy(imageData, screen.image, WIDTH * HEIGHT * sizeof(uint32_t));

	InvalidateRect(m_hWnd, nullptr, FALSE);
	UpdateWindow(m_hWnd);

	return true;
}

#endif

