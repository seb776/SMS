#pragma once

#include "../Tools/MeanLeanWindows.h"

class AWin32Window
{
protected:
	HINSTANCE _hInstance;
public:

	AWin32Window(HINSTANCE hInstance);
	virtual ~AWin32Window(void);
};