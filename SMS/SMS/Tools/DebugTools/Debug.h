#pragma once

#define SMS_ERROR_MSG "ERROR"

void LogConsole(const char *str);
void LogConsole(const char *strA, const char *strB);
void LogConsole(const char *strA, const char *strB, const char *strC);
char *putnbr(int nbr, char *buf, int size);

