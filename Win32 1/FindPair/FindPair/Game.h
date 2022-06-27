#pragma once
#include "FindPair.h"

#define CS 64
#define random(n) (rand() % n)

class Game
{
public:
	Game(HINSTANCE hInst, HWND hWnd);
	void Update(ULONGLONG tick);
	void HandleInput(shared_ptr<POINT> mouseCoord);
	void InitGame();
	void DrawScreen(HDC hdc);
	shared_ptr<POINT> GetTempFlip();
	int GetRemainPair();
	void DrawBitmap(HDC hdc, int x, int y, int idx, Status status);
	void Timer();
	~Game();
private:
	HINSTANCE _hInst;
	HWND _hWnd;
	CardCell _cardCells[4][4];
	int _count;
	HBITMAP _hBitSheet;
	GameStatus _gStatus;
	float _viewTimer;
	float _hintTimer;
	float gameTimer;
	int prevGameTimer = 0;
	float clearTimer;
	float animTime = 1200;
	float animTimer = -1;
};

