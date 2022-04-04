// GDIProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GDIProject.h"
#include<math.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_GDIPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIPROJECT));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
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

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

//#define STEP    10
#define R   30
//#define RAD     3.14/180
//#define MAX_STRING 128

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#pragma region 주석

    //static HFONT hFont;
    //static HPEN hPen;
    //static HBRUSH hBrush1, hBrush2;
   /* RECT rect = { 50,50,200,200 };
    static HBRUSH hBrush[3];
    static HRGN hRgn[3];
    static POINT pts[3] = { {200,200}, {200,300}, {300,300} };*/
    //static HBRUSH hRedBrush, hBlueBrush;
#pragma endregion

    //static POINT ptCircle = { 100,100 }; //원의 중심

    /*static TCHAR str1[MAX_STRING + 1] = { 0, };
    static TCHAR str2[MAX_STRING + 1] = { 0, };

    int n;*/

    static POINT ptMouse;
    static BOOL bDraw = false;

    switch (message)
    {
    
//    case WM_KEYDOWN:
//        
//       /* n = lstrlen(str1);
//        if (n < MAX_STRING && IsCharAlphaNumeric(wParam))
//        {
//            str1[n] = static_cast<TCHAR>(wParam);
//        }
//        if (n > 0 && wParam == 8)
//        {
//            str1[n - 1] = 0;
//        }*/
//        
//
//        /*if (GetAsyncKeyState(VK_LEFT) & 0x8000) ptCircle.x -= STEP;
//        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) ptCircle.x += STEP;
//        if (GetAsyncKeyState(VK_UP) & 0x8000) ptCircle.y -= STEP;
//        if (GetAsyncKeyState(VK_DOWN) & 0x8000) ptCircle.y += STEP;*/
//		/*switch (wParam)
//		{
//        case VK_RIGHT:
//            ptCircle.x+= STEP;
//            break;
//        case VK_LEFT:
//            ptCircle.x-= STEP;
//            break;
//        case VK_UP:
//            ptCircle.y-= STEP;
//            break;
//        case VK_DOWN:
//            ptCircle.y+= STEP;
//            break;
//
//
//		    default:
//			    break;
//		}*/
//        //InvalidateRect(hWnd, NULL, TRUE); //창 전체가 지워지고 새로 갱신
//        /*if (wParam == VK_ESCAPE)
//        {
//            if (MessageBox(hWnd, _T("정말 종료할래?"), _T("Alert Message"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK) 
//            {
//                DestroyWindow(hWnd);
//            }
//        }*/
//        break;
//
//    /*case WM_CHAR:
//        n = lstrlen(str2);
//        if (n < MAX_STRING && IsCharAlphaNumeric(wParam))
//        {
//            str2[n] = static_cast<TCHAR>(wParam);
//        }
//        if (n > 0 && wParam == 8)
//        {
//            str2[n - 1] = 0;
//        }
//        InvalidateRect(hWnd, NULL, TRUE);
//        break;*/
//
//    case WM_CREATE:
//
//        
//
//#pragma region 주석
//        /*hBrush[0] = CreateSolidBrush(RGB(255, 0, 0));
//       hBrush[1] = CreateSolidBrush(RGB(0, 255, 0));
//       hBrush[2] = CreateSolidBrush(RGB(0, 0, 255));
//
//       hRgn[0] = CreateRectRgnIndirect(&rect);
//       hRgn[1] = CreateEllipticRgnIndirect(&rect);
//       hRgn[2] = CreatePolygonRgn(pts, 3, ALTERNATE);*/
//
//       //hFont = CreateFont(100, 0, 10, 0, FW_NORMAL,0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("명조"));
//       //hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 255));
//       //hBrush1 = CreateSolidBrush(RGB(50,50,200));
//       //hBrush2 = CreateHatchBrush(HS_BDIAGONAL, RGB(20,0,230));
//
//        //hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
//        //hBlueBrush = CreateSolidBrush(RGB(0, 255, 0));
//#pragma endregion
//
//        break;

    case WM_LBUTTONDOWN:

        ptMouse.x = LOWORD(lParam);
        ptMouse.y =  HIWORD(lParam);
        bDraw = TRUE;
        InvalidateRect(hWnd, NULL, FALSE);

            break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            if (bDraw) {
                Ellipse(hdc, ptMouse.x - R, ptMouse.y - R, ptMouse.x + R, ptMouse.y + R);
                bDraw = FALSE;
            }

           
#pragma region  주석
            /*HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, GetStockObject(NULL_PEN)));
            HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hRedBrush));

            Rectangle(hdc, 50, 50, 200, 200);
            SelectObject(hdc, hOldBrush);

            SetROP2(hdc, R2_XORPEN);
            hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hBlueBrush));
            Rectangle(hdc, 150, 150, 300, 300);
            SelectObject(hdc, hOldBrush);*/

            //PtInRegion(hRgn[0], 20, 30);
          /* for (int i = 0; i < 3; i++)
           {
               FillRgn(hdc, hRgn[i], hBrush[i]);
           }*/

           /*HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hBrush1));
           MoveToEx(hdc, 160, 100, NULL);
           for (int i = 1; i <= 360; i++)
           {
               LineTo(hdc, 100 + cos(i * 3.14 / 180) * 60, 100 + sin(i * 3.14 / 180) * 60);
           }
           SelectObject(hdc, hOldBrush);*/

           //HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hBrush1));  //브러시 바꾸고 이전의 브러시를 반환함
           //Rectangle(hdc, 50, 50, 400, 400);
           //SelectObject(hdc, hOldBrush);
           //HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, GetStockObject(NULL_PEN)));
           //Rectangle(hdc, 250, 100, 200, 200);
           //SelectObject(hdc, hOldPen);

           //hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(NULL_BRUSH)));
           //Rectangle(hdc, 250, 100, 350, 200);
           //SelectObject(hdc, hOldBrush);

           //hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hBrush2));
           //Rectangle(hdc, 100, 250, 200, 350);
           //SelectObject(hdc, hOldBrush);

             /* HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hPen));
            MoveToEx(hdc, 100, 100, NULL);
            LineTo(hdc, 100, 200);
            LineTo(hdc, 200, 200);

            SelectObject(hdc, hOldPen);
            LineTo(hdc, 200, 100);
            LineTo(hdc, 100, 200);*/

            /*TextOut(hdc, 50, 100, _T("Hello Win32API"), 14);
            SetTextAlign(hdc, TA_BOTTOM);
            TCHAR str[128] = _T("안녕하세요!");

            SetTextColor(hdc, RGB(255, 0, 0));
            SetBkColor(hdc, RGB(0, 0, 255));

            TextOut(hdc, 50, 200, str, lstrlen(str));

            HFONT hOldFont = static_cast<HFONT>(SelectObject(hdc, hFont));
            TextOut(hdc, 100, 200, _T("안녕"), 2);

            SelectObject(hdc, hOldFont);
            TextOut(hdc, 100, 300, _T("안녕 티라노"), 6);*/
            //DrawText
            //int baseX = 200;
            //int baseY = 250;
            //int width = 300;
            //int height = 150;
           /* MoveToEx(hdc, baseX, baseY, NULL);
            LineTo(hdc, baseX + width, baseY);
            LineTo(hdc, baseX + width, baseY+height);
            LineTo(hdc, baseX, baseY+height);
            LineTo(hdc, baseX, baseY);*/
            //Rectangle(hdc, baseX, baseY, baseX + width, baseY + height);
            //Ellipse(hdc, baseX, baseY, baseX + width, baseY + height);
            //SetPixel(hdc, 100, 200, RGB(0, 255, 255));

             //Ellipse(hdc, ptCircle.x-R, ptCircle.y-R, ptCircle.x+ R, ptCircle.y+ R);
            /*MoveToEx(hdc, ptCircle.x+R, ptCircle.y, NULL);
            for (int i = 1; i <= 360; i++)
            {
                LineTo(hdc, ptCircle.x + cos(i * RAD) * R, ptCircle.y + sin(i * RAD) * R);
            }*/

            //TextOut(hdc, 100, 50, str1, lstrlen(str1));
            //TextOut(hdc, 100, 100, str2, lstrlen(str2));
#pragma endregion      

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
#pragma region  주석
        //DeleteObject(hBrush1);
        //DeleteObject(hBrush2);
        //DeleteObject(hFont);
        //DeleteObject(hPen);
       /* for (int i = 0; i < 3; i++)
        {
            DeleteObject(hBrush[i]);
            DeleteObject(hRgn[i]);
        }*/
        /* DeleteObject(hRedBrush);
        DeleteObject(hBlueBrush);*/
#pragma endregion
        
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
