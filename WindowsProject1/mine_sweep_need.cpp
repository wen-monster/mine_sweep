#include "stdafx.h"
#include "mine_sweep.h"


int X = 30, Y = 15, N = 99;

struct MAP MAP[50][30];
extern int flag,TIME;
extern HWND hstatus, hButton1, hButton2, hButton3, hButton4, textBox1, textBox2, textBox3;
int remain, open_;
LOGFONT logfont;                    //改变输出字体

void clean(HWND hwnd)
{
	int i, j;
	if (hButton1!=NULL)
		DestroyWindow(hButton1); hButton1 = NULL;
	if (hButton2 != NULL)
		DestroyWindow(hButton2); hButton2 = NULL;
	if (hButton3 != NULL)
		DestroyWindow(hButton3); hButton3 = NULL;
	if (hButton4 != NULL)
		DestroyWindow(hButton4); hButton4 = NULL;
	if (textBox1 != NULL)
		DestroyWindow(textBox1); textBox1 = NULL;
	if (textBox2 != NULL)
		DestroyWindow(textBox2); textBox2 = NULL;
	if (textBox3 != NULL)
		DestroyWindow(textBox3); textBox3 = NULL;

	for (i = 0; i<X; i++)
		for (j = 0; j<Y; j++)
		{
			MAP[i][j].number = 0;
			MAP[i][j].state = 0;
		}
	DR(hwnd, 3, 0, 0);
	remain = N;
	open_ = 0;
}

void menu(HWND hWnd, HINSTANCE hInst)
{
	RECT rect;
	TCHAR tmp[50];
	HDC hdc;

	if (hButton1 == NULL)
	{
		if(hButton4 != NULL)
			DestroyWindow(hButton4); hButton4 = NULL;		
		InvalidateRect(hWnd,NULL,FALSE);
		GetWindowRect(hWnd,&rect);
		MoveWindow(hWnd, rect.left, rect.top, 250, 300, TRUE);
		hButton2 = CreateWindow(_T("button"), _T("简单"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, 40, 100, 30, hWnd, (HMENU)btn2, hInst, NULL);
		hButton3 = CreateWindow(_T("button"), _T("中等"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, 90, 100, 30, hWnd, (HMENU)btn3, hInst, NULL);
		hButton4 = CreateWindow(_T("button"), _T("困难"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, 140, 100, 30, hWnd, (HMENU)btn4, hInst, NULL);
		hButton1 = CreateWindow(_T("button"), _T("自定义"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, 190, 100, 30, hWnd, (HMENU)btn1, hInst, NULL);
	}
	if (textBox1 != NULL)
	{
		hdc = GetDC(hWnd);
		SetBkColor(hdc,RGB(200,200,200));       //底色
		_stprintf_s(tmp, _T("行数：            (小于30)"));
		TextOut(hdc, 50, 40, (LPCWSTR)tmp, lstrlen(tmp));
		_stprintf_s(tmp, _T("列数：            (小于50)"));
		TextOut(hdc, 50, 90, (LPCWSTR)tmp, lstrlen(tmp));
		_stprintf_s(tmp, _T("雷数："));
		TextOut(hdc, 70, 140, (LPCWSTR)tmp, lstrlen(tmp));
		ReleaseDC(hWnd, hdc);
	}
}

void custom(HWND hWnd, HINSTANCE hInst)
{
	TCHAR tmp[50];
	HDC hdc;
	DestroyWindow(hButton2);
	DestroyWindow(hButton3);
	DestroyWindow(hButton4);
	DestroyWindow(hButton1);
	hButton2 = NULL;
	hButton3 = NULL;
	hButton4 = NULL;

	_stprintf_s(tmp, _T("%d"), Y);
	textBox1 = CreateWindow(_T("EDIT"), tmp,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN, 100, 40, 40, 20, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	_stprintf_s(tmp, _T("%d"), X);
	textBox2 = CreateWindow(_T("EDIT"), tmp,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN, 100, 90, 40, 20, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	_stprintf_s(tmp, _T("%d"), N);
	textBox3 = CreateWindow(_T("EDIT"), tmp,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN, 120, 140, 40, 20, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	hButton1 = CreateWindow(_T("button"), _T("确定"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, 190, 100, 30, hWnd, (HMENU)btn5, hInst, NULL);

	hdc = GetDC(hWnd);
	SetBkColor(hdc, RGB(200, 200, 200));       //底色
	_stprintf_s(tmp, _T("行数：            (小于30)"));
	TextOut(hdc, 50, 40, (LPCWSTR)tmp, lstrlen(tmp));
	_stprintf_s(tmp, _T("列数：            (小于50)"));
	TextOut(hdc, 50, 90, (LPCWSTR)tmp, lstrlen(tmp));
	_stprintf_s(tmp, _T("雷数："));
	TextOut(hdc, 70, 140, (LPCWSTR)tmp, lstrlen(tmp));
	ReleaseDC(hWnd, hdc);
}

void start(HWND hWnd)
{
	TCHAR Time[13];
	TCHAR mine[10];
	RECT rect;
	HDC hdc;
	clean(hWnd);
	GetWindowRect(hWnd,&rect);
	MoveWindow(hWnd, rect.left, rect.top, (X+1)*UNIT-9, (Y + 2)* UNIT+30-10, TRUE);
	TIME = 0;
	hdc = GetDC(hWnd);
	SetBkColor(hdc, RGB(200, 200, 200));       //底色
	_stprintf_s(Time, _T(" 时间:%d  "), TIME);
	TextOut(hdc, ((X + 1) * UNIT - 9) / 2 - 100, 8, (LPCWSTR)Time, lstrlen(Time));
	_stprintf_s(mine, _T(" 剩余:%d  "), N);
	TextOut(hdc, ((X + 1) * UNIT - 9) / 2 + 25, 8, (LPCWSTR)mine, lstrlen(mine));
	ReleaseDC(hWnd, hdc);
	flag = 1;
	KillTimer(hWnd, 1);
}

void build(HWND hWnd, HINSTANCE hInst, int x, int y)
{
	int i=0, j=0, tmp = 0;
	//do
	//{
	//	i = rand() % X;
	//	j = rand() % Y;
	//	if ((MAP[i][j].number != 9) && ((i<x - 1) || (i>x + 1) || (j<y - 1) || (j>y + 1)))
	//	{
	//		MAP[i][j].number = 9;
	//		ncount++;
	//	}
	//} while (ncount<N);
	int* mine_table = (int*)malloc(sizeof(int) * N);
	for (i = 0; i < N; i++)
	{
		while ((j / Y >= x - 1) && (j / Y <= x + 1) && (j % Y >= y - 1) && (j % Y <= y + 1))
			j++;
		MAP[j/Y][j%Y].number = 9;
		mine_table[i] = j;
		j++;
	}
	for (; j < X * Y; j++)
	{
		if ((j / Y >= x - 1) && (j / Y <= x + 1) && (j % Y >= y - 1) && (j % Y <= y + 1))
			continue;
		if (rand() % j < N)
		{
			MAP[j / Y][j % Y].number = 9;
			tmp = rand() % N;
			MAP[mine_table[tmp] / Y][mine_table[tmp] % Y].number = 0; 
			mine_table[tmp] = j;
			
		}
	}

	for (i = 0; i<X; i++)
		for (j = 0; j<Y; j++)
		{
			MAP[i][j].state = 0;
			if (MAP[i][j].number != 9)
			{
				if (i - 1 >= 0 && MAP[i - 1][j].number == 9)
					MAP[i][j].number++;
				if (i + 1<X && MAP[i + 1][j].number == 9)
					MAP[i][j].number++;
				if (j - 1 >= 0 && MAP[i][j - 1].number == 9)
					MAP[i][j].number++;
				if (j + 1<Y && MAP[i][j + 1].number == 9)
					MAP[i][j].number++;
				if (i - 1 >= 0 && j - 1 >= 0 && MAP[i - 1][j - 1].number == 9)
					MAP[i][j].number++;
				if (i - 1 >= 0 && j + 1<Y && MAP[i - 1][j + 1].number == 9)
					MAP[i][j].number++;
				if (i + 1<X && j - 1 >= 0 && MAP[i + 1][j - 1].number == 9)
					MAP[i][j].number++;
				if (i + 1<X && j + 1<Y && MAP[i + 1][j + 1].number == 9)
					MAP[i][j].number++;
			}
		}

	hButton4 = CreateWindow(_T("button"), L"O",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, ((X + 1) * UNIT)/2-28, 0, 30, 30, hWnd, (HMENU)btn6, hInst, NULL);
}

void B_L(HWND hwnd, int x, int y)
{

	if (MAP[x][y].number == 0)
	{
		open(hwnd, x, y);
		if (open_ + N == X * Y)
			win(hwnd);
		return;
	}
	if (MAP[x][y].number == 9)
	{
		over(hwnd, x, y);
		return;
	}
	else
	{
		MAP[x][y].state = 1;
		open_++;
		DR(hwnd, 1, x, y);
		if (open_+ N == X * Y)
			win(hwnd);
		return;
	}
}

void B_R(HWND hwnd, int x, int y)
{
	TCHAR mine[20];
	HDC hdc;
	if (MAP[x][y].state == 0)
	{
		MAP[x][y].state = 2;
		DR(hwnd, 2, x, y);
		remain--;
		_stprintf_s(mine, _T(" 剩余:%d  "), remain);
		hdc = GetDC(hwnd);
		SetBkColor(hdc, RGB(200, 200, 200));       //底色
		TextOut(hdc, ((X + 1) * UNIT - 9) / 2+25, 8, (LPCWSTR)mine, lstrlen(mine));
		ReleaseDC(hwnd, hdc);
		if (open_ == X * Y&&remain == 0)
			win(hwnd);
		return;
	}
	if (MAP[x][y].state == 2)
	{
		MAP[x][y].state = 0;
		DR(hwnd, 0, x, y);
		remain++;
		_stprintf_s(mine, _T(" 剩余:%d  "), remain);
		hdc = GetDC(hwnd);
		SetBkColor(hdc, RGB(200, 200, 200));       //底色
		TextOut(hdc, ((X + 1) * UNIT - 9) / 2+25, 8, (LPCWSTR)mine, lstrlen(mine));
		ReleaseDC(hwnd, hdc);
		return;
	}
}

void open(HWND hwnd, int x, int y)
{
	if (MAP[x][y].state == 0)
	{
		open_++;
		DR(hwnd, 1, x, y);
		MAP[x][y].state = 1;
		if (MAP[x][y].number != 0)
			return;
		if (x - 1 >= 0 && MAP[x - 1][y].state == 0)
			open(hwnd, x - 1, y);
		if (x + 1<X && MAP[x + 1][y].state == 0)
			open(hwnd, x + 1, y);
		if (y - 1 >= 0 && MAP[x][y - 1].state == 0)
			open(hwnd, x, y - 1);
		if (y + 1<Y && MAP[x][y + 1].state == 0)
			open(hwnd, x, y + 1);
		if (x - 1 >= 0 && y - 1 >= 0 && MAP[x - 1][y - 1].state == 0)
			open(hwnd, x - 1, y - 1);
		if (x - 1 >= 0 && y + 1<Y && MAP[x - 1][y + 1].state == 0)
			open(hwnd, x - 1, y + 1);
		if (x + 1<X && y - 1 >= 0 && MAP[x + 1][y - 1].state == 0)
			open(hwnd, x + 1, y - 1);
		if (x + 1<X && y + 1<Y && MAP[x + 1][y + 1].state == 0)
			open(hwnd, x + 1, y + 1);
	}
}

void B_LL(HWND hwnd, int x, int y)
{
	if (MAP[x][y].number == 9)
	{
		over(hwnd, x, y);
		return;
	}
	MAP[x][y].state = 1;
	int number = 0;
	if (x - 1 >= 0 && MAP[x - 1][y].state == 2)
		number++;
	if (x + 1<X && MAP[x + 1][y].state == 2)
		number++;
	if (y - 1 >= 0 && MAP[x][y - 1].state == 2)
		number++;
	if (y + 1<Y && MAP[x][y + 1].state == 2)
		number++;
	if (x - 1 >= 0 && MAP[x - 1][y - 1].state == 2)
		number++;
	if (x - 1 >= 0 && MAP[x - 1][y + 1].state == 2)
		number++;
	if (x + 1<X && MAP[x + 1][y - 1].state == 2)
		number++;
	if (x + 1<X && MAP[x + 1][y + 1].state == 2)
		number++;
	if (number == MAP[x][y].number)
	{
		if (x - 1 >= 0 && MAP[x - 1][y].state == 0)
		{
			B_L(hwnd, x - 1, y);
			B_LL(hwnd, x - 1, y);
		}
		if (x + 1<X && MAP[x + 1][y].state == 0)
		{
			B_L(hwnd, x + 1, y);
			B_LL(hwnd, x + 1, y);
		}
		if (y - 1 >= 0 && MAP[x][y - 1].state == 0)
		{
			B_L(hwnd, x, y - 1);
			B_LL(hwnd, x, y - 1);
		}
		if (y + 1<Y && MAP[x][y + 1].state == 0)
		{
			B_L(hwnd, x, y + 1);
			B_LL(hwnd, x, y + 1);
		}
		if (x - 1 >= 0 && y - 1 >= 0 && MAP[x - 1][y - 1].state == 0)
		{
			B_L(hwnd, x - 1, y - 1);
			B_LL(hwnd, x - 1, y - 1);
		}
		if (x - 1 >= 0 && y + 1<Y && MAP[x - 1][y + 1].state == 0)
		{
			B_L(hwnd, x - 1, y + 1);
			B_LL(hwnd, x - 1, y + 1);
		}
		if (x + 1<X && y - 1 >= 0 && MAP[x + 1][y - 1].state == 0)
		{
			B_L(hwnd, x + 1, y - 1);
			B_LL(hwnd, x + 1, y - 1);
		}
		if (x + 1<X && y + 1<Y && MAP[x + 1][y + 1].state == 0)
		{
			B_L(hwnd, x + 1, y + 1);
			B_LL(hwnd, x + 1, y + 1);
		}
	}
}

void DR(HWND hwnd, int state, int i, int j)
{
	ZeroMemory(&logfont, sizeof(LOGFONT));
	//logfont.lfCharSet=GB2312_CHARSET; //字体
	logfont.lfHeight = -20;               //字大小
	logfont.lfWeight = 700;                //字粗细
	HFONT hFont = CreateFontIndirect(&logfont);
	HDC hdc = GetDC(hwnd);

	switch (MAP[i][j].number)
	{
	case 1:
		SetTextColor(hdc, RGB(255, 185, 29));
		break;
	case 2:
		SetTextColor(hdc, RGB(112, 128, 144));
		break;
	case 3:
		SetTextColor(hdc, RGB(102, 205, 170));
		break;
	case 4:
		SetTextColor(hdc, RGB(205, 92, 92));
		break;
	case 5:
		SetTextColor(hdc, RGB(218, 112, 214));
		break;
	case 6:
		SetTextColor(hdc, RGB(255, 110, 180));
		break;
	case 7:
		SetTextColor(hdc, RGB(0, 139, 69));
		break;
	case 8:
		SetTextColor(hdc, RGB(255, 255, 0));
		break;
	}
	//SetTextColor(hdc,RGB(255,255,0)); //字颜色
	//SetBkColor(hdc,RGB(0,0,0));       //底色
	SetBkMode(hdc, TRANSPARENT);       //无底色
	SelectObject(hdc, hFont);

	HBRUSH hbrush1, hbrush2;
	RECT rect = { 0,0,0,0 };
	hbrush2 = CreateSolidBrush(RGB(0, 0, 0));
	switch (state)
	{
	case 0://关
		hbrush1 = CreateSolidBrush(RGB(127, 127, 127));
		DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		break;
	case 1://开
		hbrush1 = CreateSolidBrush(RGB(255, 255, 255));
		DR_number(hbrush1, hbrush2, hdc, rect, i, j);
		break;
	case 2://雷
		hbrush1 = CreateSolidBrush(RGB(255, 255, 0));
		DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		break;
	case 3://新
		hbrush1 = CreateSolidBrush(RGB(127, 127, 127));
		for (i = 0; i<X; i++)
			for (j = 0; j<Y; j++)
				DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		break;
	case 4://炸
		hbrush1 = CreateSolidBrush(RGB(255, 0, 0));
		DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		break;
	case 5://错
		hbrush1 = CreateSolidBrush(RGB(135, 206, 255));
		DR_number(hbrush1, hbrush2, hdc, rect, i, j);
		break;
	case 6://赢
		hbrush1 = CreateSolidBrush(RGB(0, 255, 0));
		DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		break;
	case 7://
		hbrush1 = CreateSolidBrush(RGB(0, 0, 0));
		DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		Sleep(200);
		hbrush1 = CreateSolidBrush(RGB(255, 255, 255));
		DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		Sleep(200);
		hbrush1 = CreateSolidBrush(RGB(0, 0, 0));
		DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		Sleep(200);
		break;
	default:
		hbrush1 = CreateSolidBrush(RGB(0, 0, 0));
	}
	ReleaseDC(hwnd, hdc);
	DeleteObject(hbrush1);
	DeleteObject(hbrush2);

	if (state == 2 || state == 4||state==6)
	{
		hdc = GetDC(hwnd);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP bg=NULL;
		switch (state)
		{
		case 2:
			bg = (HBITMAP)LoadBitmap(GetModuleHandle(NULL) , MAKEINTRESOURCE(IDB_BITMAP2));
			break;
		case 6:
			bg = (HBITMAP)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
			break;
		case 4:
			bg = (HBITMAP)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3));
			break;
		}
		SelectObject(mdc, bg);
		BitBlt(hdc, i* UNIT+1, j* UNIT+31, 22, 22, mdc, 0, 0, SRCCOPY);
		ReleaseDC(hwnd, hdc);
		DeleteDC(mdc);
	}


}

void DR_square(HBRUSH hbrush1, HBRUSH hbrush2, HDC hdc, RECT rect, int i, int j)
{
	int x1 = i * UNIT, x2 = (i + 1)*UNIT, y1 = j * UNIT, y2 = (j + 1)*UNIT;
	SetRect(&rect, x1, y1+30, x2, y2+30);
	FillRect(hdc, &rect, hbrush1);
	FrameRect(hdc, &rect, hbrush2);
}

void DR_number(HBRUSH hbrush1, HBRUSH hbrush2, HDC hdc, RECT rect, int i, int j)
{
	char print[2];
	sprintf_s(print, "%d", MAP[i][j].number);
	DR_square(hbrush1, hbrush2, hdc, rect, i, j);
	if (MAP[i][j].number != 0)
		TextOut(hdc, i*UNIT + 7, j*UNIT + 1+30, (LPCWSTR)print, 1);
}

void over(HWND hwnd, int x, int y)
{
	KillTimer(hwnd, 1);
	int i, j;
	for (i = 0; i<X; i++)
		for (j = 0; j<Y; j++)
		{
			if (MAP[i][j].number == 9 && MAP[i][j].state != 2)//炸
			{
				DR(hwnd, 4, i, j);
				continue;
			}
			if (MAP[i][j].number != 9 && MAP[i][j].state == 2)//错
			{
				DR(hwnd, 5, i, j);
				continue;
			}
			if (MAP[i][j].number != 9 && MAP[i][j].state != 2)//常
			{
				DR(hwnd, 1, i, j);
				continue;
			}
		}
	DR(hwnd, 7, x, y);
	if (MAP[x][y].number == 9 && MAP[x][y].state != 2)//炸
		DR(hwnd, 4, x, y);
	if (MAP[x][y].number != 9 && MAP[x][y].state != 2)//常
		DR(hwnd, 1, x, y);
	flag = -1;
}

void win(HWND hwnd)
{
	KillTimer(hwnd, 1);
	int i, j;
	HDC hdc;
	TCHAR mine[20];
	remain = 0;
	_stprintf_s(mine, _T(" 剩余:%d  "), remain);
	hdc = GetDC(hwnd);
	SetBkColor(hdc, RGB(200, 200, 200));       //底色
	TextOut(hdc, ((X + 1) * UNIT - 9) / 2 + 25, 8, (LPCWSTR)mine, lstrlen(mine));
	ReleaseDC(hwnd, hdc);

	for (i = 0; i<X; i++)
		for (j = 0; j<Y; j++)
			if (MAP[i][j].number == 9)
				DR(hwnd, 6, i, j);
	flag = -1;
}

void recover(HWND hwnd)
{
	int i, j;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	//logfont.lfCharSet=GB2312_CHARSET; //字体
	logfont.lfHeight = -20;               //字大小
	logfont.lfWeight = 700;                //字粗细
	HFONT hFont = CreateFontIndirect(&logfont);
	HBRUSH hbrush1, hbrush2;
	RECT rect = { 0,0,0,0 };

	HDC hdc = GetDC(hwnd);
	hbrush2 = CreateSolidBrush(RGB(0, 0, 0));
	hbrush1 = CreateSolidBrush(RGB(255, 255, 255));
	for (i=0;i<X;i++)
		for (j = 0; j < Y; j++)
		{
			if (MAP[i][j].state == 1)
			{
				switch (MAP[i][j].number)
				{
				case 1:
					SetTextColor(hdc, RGB(255, 185, 29));
					break;
				case 2:
					SetTextColor(hdc, RGB(112, 128, 144));
					break;
				case 3:
					SetTextColor(hdc, RGB(102, 205, 170));
					break;
				case 4:
					SetTextColor(hdc, RGB(205, 92, 92));
					break;
				case 5:
					SetTextColor(hdc, RGB(218, 112, 214));
					break;
				case 6:
					SetTextColor(hdc, RGB(255, 110, 180));
					break;
				case 7:
					SetTextColor(hdc, RGB(0, 139, 69));
					break;
				case 8:
					SetTextColor(hdc, RGB(255, 255, 0));
					break;
				}
				//SetTextColor(hdc,RGB(255,255,0)); //字颜色
				//SetBkColor(hdc,RGB(0,0,0));       //底色
				SetBkMode(hdc, TRANSPARENT);       //无底色
				SelectObject(hdc, hFont);
				DR_number(hbrush1, hbrush2, hdc, rect, i, j);
			}
		}
	ReleaseDC(hwnd, hdc);
	DeleteObject(hbrush1);
	DeleteObject(hbrush2);

	hdc = GetDC(hwnd);
	hbrush2 = CreateSolidBrush(RGB(0, 0, 0));
	hbrush1 = CreateSolidBrush(RGB(127, 127, 127));
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
		{
			if (MAP[i][j].state == 0)
				DR_square(hbrush1, hbrush2, hdc, rect, i, j);
		}
	ReleaseDC(hwnd, hdc);
	DeleteObject(hbrush1);
	DeleteObject(hbrush2);

	hdc = GetDC(hwnd);
	hbrush2 = CreateSolidBrush(RGB(0, 0, 0));
	hbrush1 = CreateSolidBrush(RGB(255, 255, 0));
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP bg = NULL;
	bg = (HBITMAP)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
	for (i = 0; i < X; i++)
		for (j = 0; j < Y; j++)
		{
			if (MAP[i][j].state == 2)
			{
				DR_square(hbrush1, hbrush2, hdc, rect, i, j);
				SelectObject(mdc, bg);
				BitBlt(hdc, i * UNIT + 1, j * UNIT + 31, 22, 22, mdc, 0, 0, SRCCOPY);
			}

		}
	ReleaseDC(hwnd, hdc);
	DeleteObject(hbrush1);
	DeleteObject(hbrush2);
	DeleteDC(mdc);
}