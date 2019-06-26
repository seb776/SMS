#include <Core/Core.h>

#include "../Tools/MeanLeanWindows.h"
#include "ParametersWindow.h"
#include "../Tools/Memory.h"

using namespace Discrepancy;

HWND ParametersWindow::g_hWndBtn;

// Run in windowed mode in debug but maximized in release.
#ifndef DEBUG
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP | WS_MAXIMIZE)
#else
#define GL_WINDOWFLAGS (WS_VISIBLE | WS_POPUP)
#endif

LRESULT CALLBACK ParametersWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			// see which button was clicked
			if ((HWND)lParam == g_hWndBtn)
			{
				MessageBox(hwnd, "Button was clicked", "OK", MB_OK);
				// todo get parameters from window
				Core::StartParameters startParams;
				Core::RunOpenGLInWindow(startParams);
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

ParametersWindow::~ParametersWindow(void) {}

ParametersWindow::ParametersWindow(HINSTANCE hInstance) :
	AWin32Window(hInstance)
{

}

void ParametersWindow::StartWindow()
{
	WNDCLASSEX wcex;

	Memory::Memset((char*)&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_szClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			"Call to RegisterClassEx failed!",
			"Win32 Guided Tour",
			NULL);

		return;
	}


	// The parameters to CreateWindow explained:  
	// szWindowClass: the name of the application  
	// szTitle: the text that appears in the title bar  
	// WS_OVERLAPPEDWINDOW: the type of window to create  
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
	// 500, 100: initial size (width, length)  
	// NULL: the parent of this window  
	// NULL: this application dows not have a menu bar  
	// hInstance: the first parameter from WinMain  
	// NULL: not used in this application  
	HWND hWnd = CreateWindow(
		g_szClassName,
		g_szWinTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 200,
		NULL,
		NULL,
		_hInstance,
		NULL
	);

	g_hWndBtn = CreateWindow(
		"BUTTON",  // Predefined class; Unicode assumed 
		"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		100,        // Button width
		100,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	{
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID != 0)
		{
			LPSTR messageBuffer = nullptr;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);



			//Free the buffer.
			LocalFree(messageBuffer);

		}
	}

	if (!hWnd)
	{
		MessageBox(NULL,
			"Call to CreateWindow failed!",
			"Win32 Guided Tour",
			NULL);

		return;
	}

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain  
	ShowWindow(hWnd, SW_SHOW);// nCmdShow);
	{
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID != 0)
		{
			LPSTR messageBuffer = nullptr;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);



			//Free the buffer.
			LocalFree(messageBuffer);

		}
	}
	UpdateWindow(hWnd);
}

int ParametersWindow::MessageLoop()
{
	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}