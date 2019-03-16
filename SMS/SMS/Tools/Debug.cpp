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