//WindowsProject1.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "mine_sweep.h"

#pragma comment( lib, "comctl32.lib" )

#define MAX_LOADSTRING 100



// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING]= _T("mine_sweeping");                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]=_T("mine_sweeping");            // 主窗口类名

int flag = -1, TIME,tmp=1;
extern int X, Y, N, remain;
extern struct MAP MAP[50][30];
HWND hstatus;
HWND hButton2;
HWND hButton3;
HWND hButton4;
HWND hButton1 = NULL;
HWND textBox1 = NULL;
HWND textBox2;
HWND textBox3;

// 此代码模块中包含的函数的前向声明: 
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

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));
	MSG msg;            /* Here messages to the application are saved */
	srand((unsigned)time(NULL));

    // 主消息循环: 
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

	/* The Window structure */
	wcex.hInstance = hInstance;
	wcex.lpszClassName = szTitle;
	wcex.lpfnWndProc = WndProc;      /* This function is called by windows */
	wcex.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wcex.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;                 /* No menu */
	wcex.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wcex.cbWndExtra = 0;                      /* structure //////or the window instance */

	/* Use Windows's default colour as the background of the window */
	wcex.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   RECT wr;
   wr.top = 0;
   wr.bottom = 300;
   wr.left = 0;
   wr.right = 250;
   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowEx(	0,
								szWindowClass, 
								szTitle, 
								WS_OVERLAPPEDWINDOW^ WS_THICKFRAME^ WS_MAXIMIZEBOX,
								CW_USEDEFAULT,
								CW_USEDEFAULT, 
								wr.right - wr.left,                 /* The programs width */
								wr.bottom - wr.top,                 /* and height in pixels */
								HWND_DESKTOP,
								nullptr, 
								hInstance, 
								nullptr
							);
   MoveWindow(hWnd, 500, 200, wr.right - wr.left, wr.bottom - wr.top, false);
   InitCommonControls();

   if (!hWnd)   return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   menu(hWnd, hInst);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i = -1, j = -1, strlen = 0;
	TCHAR Time[13];
    TCHAR mine[10];
    TCHAR text[10];
    HDC hdc;
	
	switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case btn2:
                X = 10; Y = 10; N = 10;
                start(hWnd);
                break;
            case btn3:
                X = 20; Y = 15; N = 50;
                start(hWnd);
                break;
            case btn4:
                X = 30; Y = 15; N = 99;
                start(hWnd);
                break;
            case btn1:
                custom(hWnd, hInst);
                break;
            case btn5:
                X = 0; Y = 0; N = 0;
                strlen = GetWindowTextLength(textBox1);
                GetWindowText(textBox1, text, strlen + 1);
                for (i = 0; i < strlen; i++)
                {
                    if (text[i] >= '0' && text[i] <= '9')
                        Y = Y * 10 + text[i] - '0';
                    else
                        break;
                }
                if (i != strlen)    break;
                strlen = GetWindowTextLength(textBox2);
                GetWindowText(textBox2, text, strlen + 1);
                for (i = 0; i < strlen; i++)
                {
                    if (text[i] >= '0' && text[i] <= '9')
                        X = X * 10 + text[i] - '0';
                    else
                        break;
                }
                if (i != strlen)    break;
                strlen = GetWindowTextLength(textBox3);
                GetWindowText(textBox3, text, strlen + 1);
                for (i = 0; i < strlen; i++)
                {
                    if (text[i] >= '0' && text[i] <= '9')
                        N = N * 10 + text[i] - '0';
                    else
                        break;
                }
                if (i != strlen||X*Y-10<N || X > 50 || Y > 30)    break;
                start(hWnd);
                break;
            case btn6:
                if(flag!=-1)
                    over(hWnd, 0, 0);
                menu(hWnd, hInst);
                break;
            
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            hdc = GetDC(hWnd);
            if (flag == 0)
            {
                recover(hWnd);
                SetBkColor(hdc, RGB(200, 200, 200));       //底色
                _stprintf_s(Time, _T(" 时间:%d  "), TIME);
                TextOut(hdc, ((X + 1) * UNIT - 9) / 2 - 100, 8, (LPCWSTR)Time, lstrlen(Time));
                _stprintf_s(mine, _T(" 剩余:%d  "), remain);
                TextOut(hdc, ((X + 1) * UNIT - 9) / 2 + 25, 8, (LPCWSTR)mine, lstrlen(mine));
            }
            else if (flag == 1)
            {
                DR(hWnd, 3, i, j);
                SetBkColor(hdc, RGB(200, 200, 200));       //底色
                _stprintf_s(Time, _T(" 时间:0  "));
                TextOut(hdc, ((X + 1) * UNIT - 9) / 2 -100, 8, (LPCWSTR)Time, lstrlen(Time));
                _stprintf_s(mine, _T(" 剩余:%d  "), N);
                TextOut(hdc, ((X + 1) * UNIT - 9) / 2 + 25, 8, (LPCWSTR)mine, lstrlen(mine));
            }
            else
                menu(hWnd, hInst);
            ReleaseDC(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	case WM_TIMER:
		TIME++;
        _stprintf_s(Time, _T(" 时间:%d  "), TIME);
        hdc = GetDC(hWnd);
        SetBkColor(hdc, RGB(200, 200, 200));       //底色
        TextOut(hdc, ((X + 1)* UNIT - 9) / 2 -100, 8, (LPCWSTR)Time, lstrlen(Time));
        ReleaseDC(hWnd, hdc);

	case WM_LBUTTONDOWN:
        if (HIWORD(lParam) < 30 || flag == -1)
			break;
		i = LOWORD(lParam) / UNIT;
		j = (HIWORD(lParam)-30) / UNIT;
		if (i >= X || i < 0 || j >= Y || j < 0 ||MAP[i][j].state != 0)
			break;
		if (flag == 1)
		{
			build(hWnd, hInst, i, j);
			SetTimer(hWnd, 1, 1000, NULL);
			flag = 0;
		}
		B_L(hWnd, i, j);
		break;
	case WM_LBUTTONDBLCLK:
		if (HIWORD(lParam) < 30 || flag != 0)
			break;
		i = LOWORD(lParam) / UNIT;
		j = (HIWORD(lParam)-30) / UNIT;
		if (i >= X || i < 0 || j >= Y || j < 0 || MAP[i][j].state != 1)
			break;
		B_LL(hWnd, i, j);
		break;
	case WM_RBUTTONDOWN:
		if ((HIWORD(lParam)<30||flag != 0))
			break;
		i = LOWORD(lParam) / UNIT;
		j = (HIWORD(lParam)-30) / UNIT;
		if (i >= X || i < 0 || j >= Y || j < 0 || MAP[i][j].state == 1)
			break;
		B_R(hWnd, i, j);
		break;
    case WM_NCLBUTTONDOWN:
        if (LOWORD(lParam)< (X + 1) * UNIT - 9 -70 &&flag == 0)
        {
            tmp = 0;
            flag = 1;
        }
        DefWindowProc(hWnd, message, wParam, lParam);
        break;
    case WM_NCMOUSEMOVE:
        if (tmp == 0)
        {
            flag = 0;
            tmp=1;
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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
