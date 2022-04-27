// GDIProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GDIProject3.h"
#include<math.h>
#include<vector>
#include "PointPair.h"

using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

vector<PointPair> ptList;

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
    LoadStringW(hInstance, IDC_GDIPROJECT3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIPROJECT3));

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

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPROJECT3));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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

#define R 30
#define M 5

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HPEN hBluePen;
	static BOOL bDraw = FALSE;
	static POINT ptStart, ptEnd;
	static int index = 0;

	switch (message)
	{
	case WM_CREATE:
	{
		hBluePen = CreatePen(PS_DASH, 1, RGB(0, 0, 255));
	}
	break;

	case WM_KEYDOWN:
		
		switch (wParam)
		{
		case 0x52:
			index--;
			bDraw = TRUE;
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case 0x5A:
			index++;
			bDraw = TRUE;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		break;

	case WM_LBUTTONDOWN:
	{
		ptStart.x = LOWORD(lParam);
		ptStart.y = HIWORD(lParam);
		ptEnd = ptStart;
	}
	break;
	case WM_LBUTTONUP:
	{
		bDraw = TRUE;

		PointPair pt(ptStart, ptEnd);
		if(index == 0)
			ptList.push_back(pt);
		else {
			PointPair tmp = ptList[ptList.size() - index];
			
			int curIdx = 0;
			for (vector<PointPair> ::iterator it = ptList.begin(); it != ptList.end(); ++it) {
				if (ptList.size() - index == curIdx)
				{
					ptList.insert(it,pt);
					break;
				}
				curIdx++;
			}
		}
		
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;
	case WM_MOUSEMOVE:
	{
		//지금 현재 마우스 왼쪽버튼이 눌린상태로 움직인거지?
		if (wParam & MK_LBUTTON) {
			HDC hdc = GetDC(hWnd);
			//#1 이전 라인을 삭제하는 모드
			int oldMode = SetROP2(hdc, R2_NOTXORPEN); //화면색상과 XOR지우기
			HPEN hOldPen = static_cast<HPEN>(SelectObject(hdc, hBluePen));
			MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
			LineTo(hdc, ptEnd.x, ptEnd.y);

			//#2 새로운 라인을 그리는 모드
			ptEnd.x = LOWORD(lParam);
			ptEnd.y = HIWORD(lParam);
			MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
			LineTo(hdc, ptEnd.x, ptEnd.y);

			SelectObject(hdc, hOldPen);
			SetROP2(hdc, oldMode);
			ReleaseDC(hWnd, hdc);
		}
	}
	break;
	case WM_SIZE:
		bDraw = TRUE;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

		if (bDraw)
		{
			int curIdx = 0;
			for (vector<PointPair> :: iterator it = ptList.begin(); it != ptList.end(); ++it) {
				
				if (ptList.size() - index == curIdx) break;

				MoveToEx(hdc, it->start.x, it->start.y, NULL);
				LineTo(hdc, it->end.x, it->end.y);

				Rectangle(hdc, it->start.x - M, it->start.y - M, it->start.x + M, it->start.y + M);
				Rectangle(hdc, it->end.x - M, it->end.y - M, it->end.x + M, it->end.y + M);
				bDraw = FALSE;
				curIdx++;
			}
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DeleteObject(hBluePen);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
