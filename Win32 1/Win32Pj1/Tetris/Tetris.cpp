//#include "Tetris.h"
//
//#define MAX_LOADSTRING 100
//
//// 전역 변수:
//HINSTANCE hInst;                                // 현재 인스턴스입니다.
//HWND hWndMain;                                  // 전역 윈도우 핸들 하나 보존해놓기
//WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
//WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
//
//// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//
////Game클래스, Player를 따로 만들어서 정의할거다.
//void DrawScreen(HDC hdc);
//void MakeNewBrick();
//Type GetAround(int x, int y, int block, int rot);
//// 현재 내 블럭이  x,y 위치에 있고, block 타입이고 rot이면 주변에 뭐가 있는지
//BOOL MoveDown(); //한칸 아래로 내리는거고, 아래에 블럭이 없었다면 FALSE, 있다면 TRUE를 반환할꺼야
//void TestFull(); //이건 줄이 꽉찼는지 검사해서 지워주는 함수
//void PrintTile(HDC hdc, int x, int y, Type t); //지정된 타입을 지정된 위치에 출력
//void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBitSheet, int idx);
//Type IntToType(int i);
//int TypeToInt(Type t);
//void UpdateBoard();
//
//Type board[BH + 2][BW + 2]; //외곽의 벽까지 2개씩 증가된 사이즈의 배열로 만들어냄.
//int px, py; //플레이어 현재 위치
//int nowBrick, rot;
//int nextBrick;
//
//int score = 0;
//int brickCount = 0; // 현재까지 소모된 브릭의 갯수를 저장해서 
//
//GameStatus status;
//int blockInterval; //처음 시작시에 1초로 놓는다.
//
//HBITMAP hBitSheet; //비트맵
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: 여기에 코드를 입력합니다.
//
//    // 전역 문자열을 초기화합니다.
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // 애플리케이션 초기화를 수행합니다:
//    if (!InitInstance(hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));
//
//    MSG msg;
//
//    // 기본 메시지 루프입니다:
//    while (true)
//    {
//        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//            if (msg.message == WM_QUIT) break;
//        }
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    return (int)msg.wParam;
//}
//
//
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = WndProc;
//    wcex.cbClsExtra = 0;
//    wcex.cbWndExtra = 0;
//    wcex.hInstance = hInstance;
//    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
//    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRIS);
//    wcex.lpszClassName = szWindowClass;
//    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
//
//    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);
//
//    if (!hWnd)
//    {
//        return FALSE;
//    }
//
//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);
//
//    return TRUE;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    RECT clientRect;
//    int targetRot;
//
//    switch (message)
//    {
//    case WM_CREATE:
//    {
//        hWndMain = hWnd;
//        
//    }
//    break;
//    case WM_TIMER:
//    {
//        //이게 게임루프가 될거다. 타이머가 발동됬다는건 블럭이 한칸 내려온다.
//        if (MoveDown() == TRUE) {
//            MakeNewBrick();
//        }
//    }
//    break;
//    case WM_COMMAND:
//    {
//        switch (LOWORD(wParam))
//        {
//        case IDM_GAME_START:
//        {
//            if (status != GameStatus::GAMEOVER) break;
//
//            for (int y = 0; y < BH + 2; y++)
//            {
//                for (int x = 0; x < BW + 2; x++) {
//                    board[y][x] = (x == 0 || x == BW + 1 || y == 0 || y == BH + 1)
//                        ? Type::WALL : Type::EMPTY;
//                }
//            }
//           
//            score = 0;
//            brickCount = 0;
//            status = GameStatus::RUNNING;
//            nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
//
//            MakeNewBrick(); // nextBrick => nowBrick 으로 변경하고 nextBrick하나 새로 뽑고
//            blockInterval = 1000; //1초에 한번씩만 내려오도록 해준다.
//
//            SetTimer(hWnd, 1, blockInterval, nullptr);
//            InvalidateRect(hWnd, NULL, TRUE);
//            //OutputDebugString(_T("Hello"));
//        }
//        break;
//        case IDM_GAME_PAUSE:
//            if (status == GameStatus::RUNNING) {
//                status = GameStatus::PAUSE;
//                KillTimer(hWnd, 1);
//            }
//            else if (status == GameStatus::PAUSE) {
//                status = GameStatus::RUNNING;
//                SetTimer(hWnd, 1, blockInterval, nullptr);
//            }
//            break;
//        case IDM_GAME_QUIT:
//            DestroyWindow(hWnd);
//            break;
//        default:
//            break;
//        }
//    }
//    break;
//    //case WH_KEYBOARD_LL
//    case WM_KEYDOWN:
//    {
//        if (status != GameStatus::RUNNING || brickCount == -1)
//            return 0;
//        switch (wParam) {
//        case VK_LEFT:
//            if (GetAround(px - 1, py, nowBrick, rot) == Type::EMPTY) {
//                px--;
//                InvalidateRect(hWnd, NULL, FALSE);
//            }
//            break;
//        case VK_RIGHT:
//            if (GetAround(px + 1, py, nowBrick, rot) == Type::EMPTY) {
//                px++;
//                InvalidateRect(hWnd, NULL, FALSE);
//            }
//            break;
//        case VK_UP:
//            targetRot = (rot + 1) % 4;
//            if (GetAround(px, py, nowBrick, targetRot) == Type::EMPTY) {
//                rot = targetRot;
//                InvalidateRect(hWnd, NULL, FALSE);
//            }
//            break;
//        case VK_DOWN:
//            if (MoveDown() == TRUE) {
//                MakeNewBrick();
//            }
//            break;
//        case VK_SPACE:
//            while (MoveDown() == FALSE) { ; }
//            MakeNewBrick();
//            break;
//        }
//    }
//    break;
//    case WM_PAINT:
//    {
//        PAINTSTRUCT ps;
//        HDC hdc = BeginPaint(hWnd, &ps);
//        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
//        DrawScreen(hdc);
//        EndPaint(hWnd, &ps);
//    }
//    break;
//    case WM_DESTROY:
//        KillTimer(hWndMain, 1);
//        DeleteObject(hBitSheet);
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//
//void DrawScreen(HDC hdc)
//{
//    
//
//    //플레이어 그리기
//    if (status != GameStatus::GAMEOVER && nowBrick != -1)
//    {
//        for (int i = 0; i < 4; i++)
//        {
//            PrintTile(hdc, px + Shape[nowBrick][rot][i].x, py + Shape[nowBrick][rot][i].y, IntToType(nowBrick+1));
//        }
//    }
//
//    for (int x = BW + 3; x <= BW + 11; x++) {
//        for (int y = BH - 5; y <= BH + 1; y++) {
//            if (x == BW + 3 || x == BW + 11 || y == BH - 5 || y == BH + 1) {
//                PrintTile(hdc, x, y, Type::WALL);
//            }
//            else {
//                PrintTile(hdc, x, y, Type::EMPTY);
//            }
//        }
//    }
//
//    if (status != GameStatus::GAMEOVER) {
//        for (int i = 0; i < 4; i++)
//        {
//            PrintTile(hdc, BW + 7 + Shape[nextBrick][0][i].x, BH - 2 + Shape[nextBrick][0][i].y, IntToType(nextBrick + 1));
//        }
//    }
//
//    TCHAR msgStr[256];
//    lstrcpy(msgStr, _T("Win32 Tetris"));
//    TextOut(hdc, (BW + 4) * TS, 30, msgStr, lstrlen(msgStr));
//    wsprintf(msgStr, _T("점수 : %d"), score);
//    TextOut(hdc, (BW + 4) * TS, 60, msgStr, lstrlen(msgStr));
//    wsprintf(msgStr, _T("벽돌 : %d개"), brickCount);
//    TextOut(hdc, (BW + 4) * TS, 90, msgStr, lstrlen(msgStr));
//    
//}
//
//void MakeNewBrick()
//{
//    brickCount++;
//    nowBrick = nextBrick;
//    nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
//
//    /*TCHAR temp[150];
//    wsprintf(temp, _T("now Brick : %d\n"), nowBrick);
//    OutputDebugString(temp);*/
//
//    px = BW / 2;
//    py = 3;
//    rot = 0;
//
//    InvalidateRect(hWndMain, NULL, FALSE);
//    if (GetAround(px, py, nowBrick, rot) != Type::EMPTY)
//    {
//        KillTimer(hWndMain, 1);
//        status = GameStatus::GAMEOVER;
//        MessageBox(hWndMain, _T("Game over!, 다시 하려면 게임시작을 누르세요"), _T("알림"), MB_OK);
//    }
//}
//
//Type GetAround(int x, int y, int block, int rot)
//{
//    for (int i = 0; i < 4; i++) {
//        Type t = board[y + Shape[nowBrick][rot][i].y][x + Shape[nowBrick][rot][i].x];
//        if (t != Type::EMPTY) return t;
//    }
//    return Type::EMPTY;
//}
//
//BOOL MoveDown()
//{
//    if (GetAround(px, py + 1, nowBrick, rot) != Type::EMPTY) {
//        TestFull();
//        return TRUE;
//    }
//    py++;
//    InvalidateRect(hWndMain, NULL, FALSE);
//    return FALSE;
//}
//
//void TestFull()
//{
//    for (int i = 0; i < 4; i++) {
//        board[py + Shape[nowBrick][rot][i].y][px + Shape[nowBrick][rot][i].x] = IntToType(nowBrick + 1);
//    }
//    nowBrick = -1;
//
//    int count = 0;
//
//    ONE:
//    for (int y = 1; y < BH+1; y++)
//    {
//        BOOL isClear = TRUE;
//        for (int x = 1; x < BW + 1; x++) {
//            if (board[y][x] == Type::EMPTY)
//            {
//                isClear = FALSE;
//                break;
//            }
//        }
//        if (isClear == TRUE)
//        {
//            count++;
//            for (int ty = y; ty > 1; ty--) {
//                for (int x = 1; x < BW + 1; x++) {
//                    board[ty][x] = board[ty - 1][x];
//                }
//            }
//
//            for (int x = 1; x < BW + 1; x++) {
//                board[1][x] = Type::EMPTY;
//            }
//            InvalidateRect(hWndMain, NULL, FALSE);
//        }
//    }
//
//    score += count * 10;
//    if (brickCount % 10 == 0 && blockInterval > 200) {
//        blockInterval -= 50;
//        SetTimer(hWndMain, 1, blockInterval, NULL);
//    }
//}
//
//
//
//void PrintTile(HDC hdc, int x, int y, Type t)
//{
//    DrawBitMap(hdc, x * TS, y * TS, hBitSheet, TypeToInt(t));
//}
//
//void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBitSheet, int idx)
//{
//    int sx = idx % 4;
//    int sy = idx / 4;
//
//    HDC hMemDC;
//    HBITMAP oldBitmap;
//    hMemDC = CreateCompatibleDC(hdc);
//    oldBitmap = static_cast<HBITMAP>(SelectObject(hMemDC, hBitSheet));
//
//    StretchBlt(hdc, x, y, TS, TS, hMemDC, sx * TS, sy * TS, TS, TS, SRCCOPY);
//    SelectObject(hMemDC, oldBitmap);
//    DeleteDC(hMemDC);
//}
//
//Type IntToType(int i)
//{
//    return static_cast<Type>(i);
//}
//
//int TypeToInt(Type t)
//{
//    return static_cast<int>(t);
//}
//
//void UpdateBoard()
//{
//    RECT rt;
//    SetRect(&rt, TS, TS, (BW + 1) * TS, (BH + 1) * TS);
//    InvalidateRect(hWndMain, &rt, FALSE);
//}