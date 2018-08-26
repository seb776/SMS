#pragma once

#include "../Tools/MeanLeanWindows.h"

class AWin32Window
{
protected:
	HINSTANCE _hInstance;
public:

	AWin32Window(HINSTANCE hInstance);
	~AWin32Window(void);
};