#include "Game.h"
#include <vector>
#include <algorithm>

Game::Game(HINSTANCE hInst, HWND hWnd)
{
	RECT crt;
	SetRect(&crt, 0, 0, CS * 4 + 250, CS * 4);
	AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE);
	SetWindowPos(hWnd, nullptr, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);

	this->_hInst = hInst;  
	this->_hWnd = hWnd;
	_hBitSheet = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_BITSHEET));
	srand(GetTickCount64());
	_viewTimer = 0;
	_hintTimer = 0;
	_gStatus = GameStatus::PAUSED;
}

Game::~Game()
{
	DeleteObject(_hBitSheet);
}

void Game::InitGame()
{
	_count = 0;
	clearTimer = 0;
	memset(_cardCells, 0, sizeof(_cardCells));
	//int x = 0;
	//int y = 0;
	int index = 0;

	vector<int> v = { 1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8 };
	random_shuffle(v.begin(), v.end());
	/*for (int i = 0; i < 100; i++) 
	{
		int r1 = v[random(16)];
		int r2 = v[random(16)];

		int temp = v[r1];
		v[r1] = v[r2];
		v[r2] = temp;
	}*/

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			_cardCells[i][j]._num = v[index];
		    _cardCells[i][j]._status = Status::HIDDEN;
			index++;
		}
	}

	//for (int i = 0; i <= 8; i++) {
	//	for (int j = 0; j < 2; j++) {
	//		/*do {
	//			x = random(4);
	//			y = random(4);
	//		} while (_cardCells[y][x]._num != 0);*/

	//		_cardCells[y][x]._num = i;
	//		_cardCells[y][x]._status = Status::HIDDEN;
	//	}
	//}

	_gStatus = GameStatus::HINT;
	InvalidateRect(_hWnd, nullptr, TRUE);
	_hintTimer = 2000;
}

void Game::Update(ULONGLONG tick)
{
	if (_gStatus == GameStatus::PAUSED) return;

	if (_gStatus == GameStatus::VIEW) {
		_viewTimer -= tick;
		if (animTimer >= 0)
		{
			animTimer += tick;
			if (animTimer >= animTime)
			{
				animTimer = -1;
				for (int x = 0; x < 4; x++) {
					for (int y = 0; y < 4; y++) {
						if (_cardCells[y][x]._status == Status::TEMPFLIP) {
							_cardCells[y][x]._status = Status::HIDDEN;
						}
					}
				}
				_gStatus = GameStatus::RUN;
				InvalidateRect(_hWnd, nullptr, FALSE);
			}
		}
		else if (_viewTimer <= 0) {
			_viewTimer = 0;
			animTimer = 0;
			//_gStatus = GameStatus::RUN;
			
			//InvalidateRect(_hWnd, nullptr, FALSE);
		}
	}
	else if(_gStatus==GameStatus::HINT) {
		_hintTimer -= tick;
		if (_hintTimer <= 0) {
			_hintTimer = 0;
			_gStatus = GameStatus::RUN;
		    InvalidateRect(_hWnd, nullptr, FALSE);
		}
	}

	gameTimer += tick;
	clearTimer += tick;
	Timer();
}

void Game::HandleInput(shared_ptr<POINT> mouseCoord)
{
	int nx = mouseCoord->x;
	int ny = mouseCoord->y;
	if (_gStatus != GameStatus::RUN || nx > 3 || ny > 3 || _cardCells[ny][nx]._status != Status::HIDDEN)
	{
		return;
	}

	shared_ptr<POINT> p = GetTempFlip();
	if (p == nullptr) {
		_cardCells[ny][nx]._status = Status::TEMPFLIP;
		InvalidateRect(_hWnd, nullptr, FALSE);
	}
	else {
		_count++;
		if (_cardCells[p->y][p->x]._num == _cardCells[ny][nx]._num) {
			MessageBeep(0);
			_cardCells[p->y][p->x]._status = Status::FLIP;
			_cardCells[ny][nx]._status = Status::FLIP;
			InvalidateRect(_hWnd, nullptr, FALSE);
			if (GetRemainPair() == 0) {
				TCHAR msg[128];
				wsprintf(msg, _T("축하합니다. 클리어 하셨습니다.\n클리어 시간 : %d초"), static_cast<int>(clearTimer) / 1000);
				MessageBox(_hWnd, msg, _T("알림"), MB_OK);
				_gStatus = GameStatus::PAUSED;
			}
		}
		else {
			_cardCells[ny][nx]._status = Status::TEMPFLIP;
			InvalidateRect(_hWnd, nullptr, FALSE);
			_gStatus = GameStatus::VIEW;
			_viewTimer = 1000;

		}
	}
}


void Game::DrawBitmap(HDC hdc, int x, int y, int idx, Status status)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, _hBitSheet);

	if (status == Status::TEMPFLIP && animTimer >= 0)
	{
		float rate = animTimer / animTime;
		StretchBlt(hdc, x, y, CS, CS*rate, hMemDC, idx * CS, 0, CS, CS-8, SRCCOPY);
		InvalidateRect(_hWnd, nullptr, FALSE);
	}
	else {
		StretchBlt(hdc, x, y, CS, CS, hMemDC, idx * CS, 0, CS, CS, SRCCOPY);
	}

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

void Game::Timer()
{
	int timer = static_cast<int>(gameTimer) / 1000;
	if (timer != prevGameTimer) {
		prevGameTimer = timer;
		InvalidateRect(_hWnd, nullptr, FALSE);
	}
}

shared_ptr<POINT> Game::GetTempFlip()
{
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (_cardCells[y][x]._status == Status::TEMPFLIP) {
				shared_ptr<POINT> p = make_shared<POINT>();
				p->x = x;
				p->y = y;
				return p;
			}
		}
	}
	return nullptr;
}

int Game::GetRemainPair()
{
	int remain = 16;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (_cardCells[y][x]._status == Status::FLIP) {
				remain--;
			}
		}
	}
	return remain / 2;
}

void Game::DrawScreen(HDC hdc)
{
	int imgIdx;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (_gStatus == GameStatus::HINT || _cardCells[y][x]._status != Status::HIDDEN) {
				imgIdx = _cardCells[y][x]._num - 1;
			}
			else {
				imgIdx = 8;
			}
			//DrawBitmap(hdc, x * CS, y * CS, imgIdx, _cardCells[y][x]._status);
			if (_cardCells[y][x]._status == Status::TEMPFLIP && animTimer >= 0)
			{
				DrawBitmap(hdc, x * CS, y * CS, 8, _cardCells[y][x]._status);
			}
			else {
				DrawBitmap(hdc, x * CS, y * CS, imgIdx, _cardCells[y][x]._status);
			}
		}
	}

	TCHAR msg[128];
	lstrcpy(msg, _T("짝 맞추기 게임 Ver 0.01")); 
	TextOut(hdc, 300, 10, msg, lstrlen(msg));

	wsprintf(msg, _T("총 시도 횟수 : %d"), _count);
	TextOut(hdc, 300, 50, msg, lstrlen(msg));

	wsprintf(msg, _T("아직 못 찾은 것 : %d"), GetRemainPair());
	TextOut(hdc, 300, 70, msg, lstrlen(msg));

	wsprintf(msg, _T("플레이 시간 : %d초"), static_cast<int>(gameTimer) / 1000);
	TextOut(hdc, 300, 90, msg, lstrlen(msg));
}

