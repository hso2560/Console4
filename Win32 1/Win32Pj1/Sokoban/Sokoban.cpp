﻿// Sokoban.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Sokoban.h"

#define MAX_LOADSTRING 100

#define MAX_STAGE 3
#define BW 32
#define BH 32

void DrawScreen(HDC hdc, PAINTSTRUCT& ps);
BOOL TestEnd();
void Move(int dir);
void InitStage();
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);

char ns[18][21];
int nStage;
int nx, ny;
int nMove;
HBITMAP hBitArr[5];

char arStage[MAX_STAGE][18][21] = {
    {
    "####################",
    "####################",
    "####################",
    "#####   ############",
    "#####O  ############",
    "#####  O############",
    "###  O O ###########",
    "### # ## ###########",
    "#   # ## #####  ..##",
    "# O  O   @      ..##",
    "##### ### # ##  ..##",
    "#####     ##########",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "####..  #     ######",
    "####..  # O  O  ####",
    "####..  #O####  ####",
    "####..    @ ##  ####",
    "####..  # #  O #####",
    "######### ##O O ####",
    "###### O  O O O ####",
    "######    #     ####",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "##########     @####",
    "########## O#O #####",
    "########## O  O#####",
    "###########O O #####",
    "########## O # #####",
    "##....  ## O  O  ###",
    "###...    O  O   ###",
    "##....  ############",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
};

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND hWndMain;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOKOBAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOKOBAN));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}





//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOKOBAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);
   
   hWndMain = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define SIZE 100
#define M 5
#define N 5

enum class BlockType {
    EMPTY,
    RED,
    BLUE
};



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //static HBITMAP hBit;
    //static BITMAP bit;

    /*static int ix, iy;
    static int nWidth, nHeight;
    static HBRUSH hRedBrush, hOldBrush, hBlueBrush;
    static int board[M][N];*/

    RECT rtClient;
    TCHAR userMsg[256];

    switch (message)
    {
    case WM_CREATE:
    {
        SetRect(&rtClient, 0, 0, 900, BH * 18);

        AdjustWindowRect(&rtClient, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

        SetWindowPos(hWnd, NULL, 0, 0, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top, SWP_NOMOVE | SWP_NOZORDER);

        for (int i = 0; i < 5; i++)
        {
            hBitArr[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EMPTY + i));
        }
        nStage = 0;
        InitStage();

        /*hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));

        GetObject(hBit, sizeof(BITMAP), &bit);


        SetWindowPos(hWnd, NULL, 0, 0, 1440, 800, SWP_NOMOVE);*/

        /*RECT rtClient = { 0,0, SIZE * N, SIZE * M };

        AdjustWindowRect(&rtClient, WS_CAPTION | WS_SYSMENU, FALSE);

        nWidth = rtClient.right - rtClient.left;
        nHeight = rtClient.bottom - rtClient.top;

        SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE);
        hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
        hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));*/
    }
        break;

    case WM_LBUTTONDOWN:
       /* ix = LOWORD(lParam) / SIZE;
        iy = HIWORD(lParam) / SIZE;
        if (ix >= N || iy >= M)
            break;
        board[iy][ix]++;
        if (board[iy][ix] > static_cast<int>(BlockType::BLUE)) {
            board[iy][ix] = static_cast<int>(BlockType::EMPTY);
        }
        InvalidateRect(hWnd, NULL, TRUE);*/
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            DrawScreen(hdc, ps);

            /*HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP hOldBit = static_cast<HBITMAP>(SelectObject(memDC, hBit));

            BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, hOldBit);
            DeleteObject(hWnd);*/

            /*for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    int x = j * SIZE;
                    int y = i * SIZE;

                    Rectangle(hdc, x, y, x + SIZE, y + SIZE);

                    if (board[i][j] == static_cast<int>(BlockType::EMPTY)) continue;

                    BlockType type = static_cast<BlockType>(board[i][j]);

                    switch (type)
                    {
                    case BlockType::EMPTY:
                        break;
                    case BlockType::RED:
                        hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hRedBrush));
                        break;
                    case BlockType::BLUE:
                        hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hBlueBrush));
                        break;
                    default:
                        break;
                    }

                    
                    Ellipse(hdc, x + 10, y + 10, x + SIZE-10, y + SIZE-10);

                    SelectObject(hdc, hOldBrush);
                }
            }*/

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
        case VK_RIGHT:
        case VK_UP:
        case VK_DOWN:
            Move(wParam);
            if (TestEnd())
            {
                wsprintf(userMsg, _T("%d 스테이지를 풀었습니다")_T("다음 스테이지로 이동합니다"), nStage+1);
                MessageBox(hWnd, userMsg, _T("알림"), MB_OK);
                if (nStage < MAX_STAGE - 1) {
                    nStage++;
                }
                InitStage();
            }
            break;
        case 'Q':
            DestroyWindow(hWnd);
            break;
        case 'R':
            InitStage();
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:

        for (int i = 0; i < 5; i++)
        {
            DeleteObject(hBitArr[i]);
        }

       /* DeleteObject(hRedBrush);
        DeleteObject(hBlueBrush);*/
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
    HDC hMemDC;
    HBITMAP hOldBitMap;
    int bx, by;
    BITMAP bit;

    hMemDC = CreateCompatibleDC(hdc);
    hOldBitMap = static_cast<HBITMAP>(SelectObject(hMemDC, hBit));

    GetObject(hBit, sizeof(BITMAP), &bit);

    bx = bit.bmWidth;
    by = bit.bmHeight;

    BitBlt(hdc, x, y, bx, by, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitMap);
    DeleteDC(hMemDC);
}

void InitStage()
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

    nMove = 0;
    InvalidateRect(hWndMain, NULL, TRUE);
}

BOOL TestEnd()
{
    return FALSE;
}

void Move(int dir)
{

}

void DrawScreen(HDC hdc, PAINTSTRUCT& ps)
{
    int iBit = 1;
    TCHAR userMsg[256];
    //EMPTY, MAN, PACK, TARGET, WALL
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            switch (ns[y][x])
            {
            case '#':
                iBit = 4;
                break;
                
            case '.':
                iBit = 3;
                break;
            case 'O':
                iBit = 2;
                break;
            case ' ':
                iBit = 0;
                break;

            default:
                break;
            }

            DrawBitmap(hdc, x * BW, y * BH, hBitArr[iBit]);
        }
    }
    DrawBitmap(hdc, nx * BW, ny * BH, hBitArr[1]);
}