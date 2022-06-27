//#pragma once
//#include "Tetris.h"
//#include "Player.h"
//
//#define BW 10
//#define BH 20
//#define TS 24 //블럭 사이즈는 24픽셀
//
//class Game : enable_shared_from_this<Game>
//{
//public:
//    ~Game() {
//        DeleteObject(_blockSheet);
//    }
//
//    void Init(HINSTANCE hInst, HWND hWnd);
//    void Update(ULONGLONG tick);
//    void Render(HDC hdc);
//
//    void PrintTile(HDC hdc, int x, int y, Type t);
//    void DrawBitmap(HDC hdc, int x, int y, int idx);
//    void GetInput(WPARAM keyCode);
//    void StartGame();
//    void PauseGame();
//    void MakeNewBrick();
//    void UpdateBoard(); //지정된 게임판 새로 그리기
//    Type GetAround(POINT p, int brick, int rot);
//    void TestFull();
//
//    BOOL CheckPlayerRender(int x, int y);
//private:
//    const int _blockCount = 7;
//    HBITMAP _blockSheet;
//    HINSTANCE _hInst;
//    HWND _hWndMain;
//    Type _board[BH + 2][BW + 2];
//    unique_ptr<Player> _player;
//
//    int _nextBrick;
//    int _score;
//    int _brickCount;
//    GameStatus _status;
//    int _blockTime;
//    int _currentTime;
//
//    Point Shape[7][4][4] = {
//    { {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2}, {0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2} },
//    { {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1} },
//    { {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1}, {0,0,-1,0,0,-1,1,-1}, {0,0,0,1,-1,0,-1,-1} },
//    { {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1}, {0,0,-1,-1,0,-1,1,0}, {0,0,-1,0,-1,1,0,-1} },
//    { {0,0,-1,0,1,0,-1,-1}, {0,0,0,-1,0,1,-1,1}, {0,0,-1,0,1,0,1,1}, {0,0,0,-1,0,1,1,-1} },
//    { {0,0,1,0,-1,0,1,-1}, {0,0,0,1,0,-1,-1,-1}, {0,0,1,0,-1,0,-1,1}, {0,0,0,-1,0,1,1,1} },
//    { {0,0,-1,0,1,0,0,1}, {0,0,0,-1,0,1,1,0}, {0,0,-1,0,1,0,0,-1}, {0,0,-1,0,0,-1,0,1} },
//    };
//
//};