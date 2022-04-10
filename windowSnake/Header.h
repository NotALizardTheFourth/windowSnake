#pragma once
#include <windows.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>
#include "Header.h"

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

class pos;
HWND create(int x, int y);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool isOutOfBoard(int direction,pos p);
pos findPosOfNext(int direction,pos p);
pos createApple(std::vector<pos>& snake);
bool clashWithSnake(int x, int y, std::vector<pos>& snake);
void updateSnake(std::vector<pos> &snake,pos next);

class pos// a basic class to make an arry of positions, all values are public
{
public:
	int x;
	int y;

	pos()
	{
		x = 0;
		y = 0;
	}

	pos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	friend bool operator==(pos& lhs, pos &rhs);
};

static int direction = 1;
pos applePos;
const int BoardLength = 15;
const int BoardHight = 9;
HWND board[BoardHight][BoardLength];
bool virtualBoard[BoardHight][BoardLength];
