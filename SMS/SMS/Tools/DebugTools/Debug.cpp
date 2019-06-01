#include <Tools/MeanLeanWindows.h>
#include "Debug.h"

void LogConsole(const char *str)
{
#if DEBUG
	OutputDebugString(str);
	OutputDebugString("\n");
#endif // !DEBUG
}

void LogConsole(const char *strA, const char *strB)
{
#if DEBUG
	OutputDebugString(strA);
	OutputDebugString(strB);
	OutputDebugString("\n");
#endif // !DEBUG
}

void LogConsole(const char *strA, const char *strB, char *strC)
{
#if DEBUG
	OutputDebugString(strA);
	OutputDebugString(strB);
	OutputDebugString(strC);
	OutputDebugString("\n");
#endif // !DEBUG
}

char *putnbr(int nbr, char *buf, int size)
{
	int i;
	for (i = 0; nbr > 0 && i < size; nbr /= 10, ++i)
	{
		buf[(size - 1) - i] = '0' + nbr % 10;
	}
	return &(buf[(size)-i]);
}
