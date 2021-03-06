#pragma once
#include "resource.h"

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <Commctrl.h>


#define UNIT 25

#define btn1 1//自定义
#define btn2 2//easy
#define btn3 3//mid
#define btn4 4//hard
#define btn5 5//开始
#define btn6 6//重启


struct MAP
{
	int number;
	int state;//0,不显示	1,数	2,雷
};

void clean(HWND);
void menu(HWND, HINSTANCE);
void start(HWND);
void build(HWND, HINSTANCE, int, int);
void B_L(HWND, int, int);
void B_R(HWND, int, int);
void open(HWND, int, int);
void B_LL(HWND, int, int);
void DR(HWND, int, int, int);
void DR_square(HBRUSH, HBRUSH, HDC, RECT, int, int);
void DR_number(HBRUSH, HBRUSH, HDC, RECT, int, int);
void over(HWND, int, int);
void win(HWND);
void recover(HWND);
void custom(HWND, HINSTANCE);
