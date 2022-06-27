#include "Game.h"

Game::Game(HWND main, HINSTANCE hInst)
{
	hWndMain = main;
	hBitSheet = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHEET));
	nStage = 0;
	undoList = new vector<MoveInfo>();
    //redoList = new vector<MoveInfo>();
}

Game::~Game()
{
	DeleteObject(hBitSheet);
	delete undoList;
    //delete redoList;
}

void Game::DrawScreen(HDC hdc, PAINTSTRUCT& ps)
{
	int iBit = 3;
	TCHAR userMsg[256];
    //EMPTY, MAN, PACK, TARGET, WALL
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            switch (ns[y][x])
            {
            case '#':
                iBit = 0;
                break;

            case '.':
                iBit = 2;
                break;
            case 'O':
                iBit = 1;
                break;
            case ' ':
                iBit = 7;
                break;

            default:
                break;
            }

            DrawBitmap(hdc,iBit ,x * BW, y * BH, hBitSheet);
        }
    }
    DrawBitmap(hdc,3 + pDir ,nx * BW, ny * BH, hBitSheet);

    wsprintf(userMsg, _T("Sokoban !"));
    TextOut(hdc, 700, 20, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("Q : 종료, R : 다시시작"));
    TextOut(hdc, 700, 40, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("N : 다음, P : 이전"));
    TextOut(hdc, 700, 60, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("Z : Undo, X : Redo"));
    TextOut(hdc, 700, 80, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("현재 스테이지 : %d"), nStage);
    TextOut(hdc, 700, 100, userMsg, lstrlen(userMsg));
    wsprintf(userMsg, _T("이동 횟수 : %d"), nMove);
    TextOut(hdc, 700, 120, userMsg, lstrlen(userMsg));
}

void Game::DrawBitmap(HDC hdc, int idx, int x, int y, HBITMAP bBitSheet)
{
    HDC hMemDC;
    HBITMAP hOldBitMap;
    int bx, by;
    BITMAP bit;

    hMemDC = CreateCompatibleDC(hdc);
    hOldBitMap = static_cast<HBITMAP>(SelectObject(hMemDC, hBitSheet));

    GetObject(hBitSheet, sizeof(BITMAP), &bit);

    bx = bit.bmWidth;
    by = bit.bmHeight;

    StretchBlt(hdc, x, y, 32, 32, hMemDC, idx * 32, 0, 32, 32, SRCCOPY);

    //BitBlt(hdc, x, y, bx, by, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitMap);
    DeleteDC(hMemDC);
}

BOOL Game::TestEnd()
{
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++) {
            if (arStage[nStage][y][x] == '.' && ns[y][x] != 'O')
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

void Game::Move(int dir)
{
    int dx = 0, dy = 0;
    BOOL bWithPack = FALSE; //팩과 함께 움직임?

    int tDir = pDir;
    switch (dir)
    {
    case VK_LEFT:
        dx = -1;
        pDir = 2;
        break;
    case VK_RIGHT:
        dx = 1;
        pDir = 3;
        break;
    case VK_UP:
        dy = -1;
        pDir = 0;
        break;
    case VK_DOWN:
        dy = 1;
        pDir = 1;
        break;
    default:
        break;
    }

    if (ns[ny + dy][nx + dx] != '#') //벽이 아니면
    {
        if (ns[ny + dy][nx + dx] == 'O') //상자 밀음
        {
            if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') //공백이거나 목적지이면
            {
                //상자 옮겼는데 원래 위치가 도착지면 다시 도착지 그림 넣음
                ErasePack(nx + dx, ny + dy);
                ns[ny + dy * 2][nx + dx * 2] = 'O';
                bWithPack = TRUE;
            }
            else
            {
                return;
            }
        }

        nx += dx;
        ny += dy;
        nMove++;

        MoveInfo mInfo(dx, dy, bWithPack, tDir); //수행 Redo
        undoList->insert(undoList->begin() + (++undoListIndex), mInfo);

        InvalidateRect(hWndMain, NULL, FALSE);
    }
}

void Game::InitStage()
{
    memcpy(ns, arStage[nStage], sizeof(ns));

    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++) {
            if (ns[y][x] == '@')
            {
                nx = x;
                ny = y;
                ns[y][x] = ' ';
            }
        }
    }

    //컨테이너 객체 쓰면 여기서 초기화 하는 작업
    undoList->clear();
    undoListIndex = -1;
    //redoList->clear();

    nMove = 0;
    InvalidateRect(hWndMain, NULL, TRUE);
}

BOOL Game::GotoNextStage()
{
    if (nStage < MAX_STAGE - 1)
    {
        nStage++;
        InitStage();
        return TRUE;
    }
    return FALSE;
}

BOOL Game::GotoPrevStage()
{
    if (nStage > 0)
    {
        nStage--;
        InitStage();
        return TRUE;
    }
    return FALSE;
}

int Game::GetCurrentStage()
{
    return nStage;
}

void Game::Ending()
{



}

void Game::ErasePack(int x, int y)
{
    ns[y][x] = arStage[nStage][y][x] == '.' ? '.' : ' ';
}

//수행 : Redo , Undo
BOOL Game::Undo()
{
    if (undoListIndex >= 0)
    {
        MoveInfo mInfo = (*undoList)[undoListIndex--];

        UndoMove(mInfo.dx, mInfo.dy, mInfo.bWithPack, mInfo.pDir);

        //undoList->pop_back();
        return TRUE;
    }
    return FALSE;
}

BOOL Game::Redo()
{
    if (undoList->size() > 0 && undoListIndex < undoList->size() - 1)
    {
        MoveInfo mInfo = (*undoList)[++undoListIndex];
        RedoMove(mInfo.dx, mInfo.dy, mInfo.bWithPack, mInfo.pDir);
   
        return TRUE;
    }

    return FALSE;
}

void Game::UndoMove(int dx, int dy, bool bWithPack, int dir)
{
    if (bWithPack)
    {
        ns[ny][nx] = 'O';

        if (arStage[nStage][ny+dy][nx+dx] == '.')
        {
            ns[ny + dy][nx + dx] = '.';
        }
        else 
        {
            ns[ny + dy][nx + dx] = ' ';
        }
    }

    nx -= dx;
    ny -= dy;

    pDir = dir;

    //nMove++; //여기서는 카운트 증가해야하나
    
    InvalidateRect(hWndMain, NULL, FALSE);
}

void Game :: RedoMove(int dx, int dy, bool bWithPack, int dir)
{
    if (ns[ny + dy][nx + dx] == '#') return;
    if (ns[ny + dy][nx + dx] == 'O') //상자 밀음
    {
        if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') 
        {
            ErasePack(nx + dx, ny + dy);
            ns[ny + dy * 2][nx + dx * 2] = 'O';
        }
        else
        {
            return;
        }
    }

    if (dx == 0)
    {
        if (dy == 1)
        {
            pDir = 1;
        }
        else {
            pDir = 0;
        }
    }
    else {
        if (dx == 1)
        {
            pDir = 3;
        }
        else {
            pDir = 2;
        }
    }

    nx += dx;
    ny += dy;

    //pDir = dir;

    //nMove++; //여기서는 카운트 증가해야하나

    InvalidateRect(hWndMain, NULL, FALSE);
}


/*

 if (ns[ny + dy][nx + dx] != '#') //벽이 아니면
    {
        if (ns[ny + dy][nx + dx] == 'O') //상자 밀음
        {
            if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') //공백이거나 목적지이면
            {
                //상자 옮겼는데 원래 위치가 도착지면 다시 도착지 그림 넣음
                ErasePack(nx + dx, ny + dy);
                ns[ny + dy * 2][nx + dx * 2] = 'O';
                bWithPack = TRUE;
            }
            else
            {
                return;
            }
        }

        nx += dx;
        ny += dy;
        nMove++;

        MoveInfo mInfo(dx, dy, bWithPack, tDir); //수행 Redo
        undoList->insert(undoList->begin() + (++undoListIndex), mInfo);

        InvalidateRect(hWndMain, NULL, FALSE);
    }

*/


void Game::TestClear()
{
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++) {
            if (arStage[nStage][y][x] == '.')
            {
                ns[y][x] = 'O';
            }
            else if (arStage[nStage][y][x] == 'O')
            {
                ns[y][x] = ' ';
            }
        }
    }

    InvalidateRect(hWndMain, NULL, FALSE);



    nStage++;
    
}


