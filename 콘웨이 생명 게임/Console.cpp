#include "Console.h"

void gotoXY(int x, int y)
{
	HANDLE hOut;
	COORD Cur;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Cur.X = x * 2;
	Cur.Y = y;
	SetConsoleCursorPosition(hOut, Cur);
}

void clrscr()
{
	system("cls");
}

void sleep(float s) 
{
	Sleep(s * 1000);
}