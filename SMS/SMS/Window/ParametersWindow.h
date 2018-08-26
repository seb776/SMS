#pragma once

#include "AWin32Window.h"

class ParametersWindow : public AWin32Window
{
public:
	ParametersWindow(HINSTANCE hInstance);
	HINSTANCE _hInstance;

	static HWND g_hWndBtn;
	const char *g_szClassName = "myWindowClass";
	const char *g_szWinTitle = "myWindowClasstest";
		
	void StartWindow();
	int MessageLoop();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	~ParametersWindow(void);
};