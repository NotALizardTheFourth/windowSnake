#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>
#include "Header.h"

// not perfect but working, maby its time to add the keyboard parts
const wchar_t CLASS_NAME[] = L"Sample Window Class";
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{



    // Register the window class.

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND board[BoardHight][BoardLength];
    for (int i = 0; i < BoardHight; i++)
    {
        for (int j = 0; j < BoardLength; j++)
        {
            board[i][j] = NULL;
        }
    }
    
    Sleep(1000);
    for (int i = 0; i < BoardHight; i++)
    {
        for (int j = 0; j < BoardLength; j++)
        {
            board[i][j] = create(i * 100, j * 100);

            ShowWindow(board[i][j], SW_HIDE);
            //Sleep(40);
        }
    }
    
    //DestroyWindow(board[0][0]);

    std::vector<pos> snake = { pos(4,4),pos(3,4) ,pos(2,4) ,pos(1,4) };
    for (int i = 0; i < 4; i++)
    {
        ShowWindow(board[snake[i].x][snake[i].y],SW_SHOW);
    }
    // Run the message loop.
    MSG msg = {};
    int x = 4, y = 4;// placement of head
    applePos = createApple(snake);
    ShowWindow(board[applePos.x][applePos.y], SW_SHOW);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        //recive and dispatch messages
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        Sleep(300);
        pos posOfNext = findPosOfNext(direction, snake[0]);

        if (isOutOfBoard(direction, posOfNext))
            break;
        
        
        // now need to check if next pos is an apple or not
        if (posOfNext == applePos)
        {
            snake.insert(snake.begin(),posOfNext);
            x = applePos.x;
            y = applePos.y;
            applePos = createApple(snake);
            ShowWindow(board[applePos.x][applePos.y], SW_SHOW);

        }
        else
        {
            updateSnake(snake, posOfNext);

            for (int i = 0; i < BoardHight; i++)
            {
                for (int j = 0; j < BoardLength; j++)
                {
                    ShowWindow(board[i][j], SW_HIDE);
                }
            }
            for (int i = 0; i < snake.size(); i++)
            {
                ShowWindow(board[snake[i].x][snake[i].y], SW_SHOW);
            }
            ShowWindow(board[applePos.x][applePos.y], SW_SHOW);
            //for (int i = 0; i < snake.size(); i++)// showes that the snake loses sync with the virtual board
            //{
            //    if (!virtualBoard[snake[i].x][snake[i].y])
            //    {
            //        MessageBox(NULL, L"lost the snake", L"lost the snake", MB_OK);
            //        break;
            //    }
            //}
        }
                //break;

        
    }
    
    DestroyWindow(board[0][0]);
    MessageBox(NULL, L"exit", L"exit", MB_OK);
    return 0;
}
HWND create(int x, int y)
{
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        x, y, 90, 90,

        NULL,       // Parent window    
        NULL,       // Menu
        GetModuleHandle(NULL),  // Instance handle
        NULL        // Additional application data
    );
    return hwnd;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        MessageBox(hwnd, L"Message Test", L"WM_DESTROY", MB_OK);
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        MessageBox(hwnd, L"Message Test", L"WM_CLOSE", MB_OK);
        break;
    case WM_QUIT:
        MessageBox(hwnd, L"Message Test", L"WM_QUIT", MB_OK);
        break;
    
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
bool isOutOfBoard(int direction,pos p)
{
    //
    // if is out of bonds function return true, else return true
    //
    switch (direction)
    {
    case 1:
        if ((p.y - 1) < 0)
            return true;
    case 2:
        if ((p.x + 1) > 15)
            return true;
    case 3:
        if ((p.y + 1) > 8)
            return true;
    case 4:
        if ((p.x - 1) < 0)
            return true;
    default:
        break;
    }
    return false;
}
pos findPosOfNext(int direction, pos p)
{
    // need to find what is wrong here
    switch (direction)
    {
    case 1:
        p.y--;
        break;
    case 2:
        p.x++;
        break;
    case 3:
        p.y++;
        break;
    case 4:
        p.x--;
        break;
    default:
        break;
    }
    pos newPos = pos(p.x, p.y);
    return newPos;
}
pos createApple(std::vector<pos> &snake)// not showing an apple
{
    srand((unsigned)time(0));
    int x;
    
    int y;

    do
    {
        x = (rand() % BoardHight) + 1;
        y = (rand() % BoardLength) + 1;
    } while (clashWithSnake(x,y,snake));

    pos p = pos(x, y);
    return p;
}
bool clashWithSnake(int x, int y, std::vector<pos>& snake)
{
    for (int i = 0; i < snake.size(); i++)
    {
        if (snake[i].x == x && snake[i].y == y)
            return true;
    }
    return false;
}
bool operator==(pos& lhs, pos& rhs) 
{

    return (lhs.x == rhs.x && lhs.y == rhs.y) ? true : false;

}
void updateSnake(std::vector<pos>& snake, pos next)
{
    for (int i = snake.size()-1; i > 0 ; i--)
    {
        snake[i] = snake[i - 1];
    }
    snake[0] = next;
}