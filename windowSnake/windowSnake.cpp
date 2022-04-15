#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>
#include "Header.h"

// need to figure why right side is not visible

const wchar_t CLASS_NAME[] = L"Sample Window Class";
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    //setting up the windows hook
    gHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, keyLoggerHook, GetModuleHandle(NULL), 0);

    if (NULL == gHookHandle) {
        MessageBox(NULL, L"problem with hook", L"problem with hook", MB_OK);
        getchar();
        return 0;
    }


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
    for (int i = 0; i < BoardLength; i++)
    {
        for (int j = 0; j < BoardHight; j++)
        {
            board[i][j] = create(i * 90, j * 90);

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
        

        if (isOutOfBoard(direction, snake[0]))
            break;
        pos posOfNext = findPosOfNext(direction, snake[0]);

        
        // now need to check if next pos is an apple or not
        if (posOfNext == applePos)
        {
            snake.insert(snake.begin(),posOfNext);
            applePos = createApple(snake);
            ShowWindow(board[applePos.x][applePos.y], SW_SHOW);

        }
        else
        {
            if (clashWithSnake(posOfNext.x, posOfNext.y, snake))
                break;
            updateSnake(snake, posOfNext);
            ShowWindow(board[applePos.x][applePos.y], SW_SHOW);
            
        }
        for (int i = 0; i < BoardLength; i++)
        {
            for (int j = 0; j < BoardHight; j++)
            {
                    
                if(clashWithSnake(i,j,snake) || (applePos.x == i && applePos.y == j))
                {
                    ShowWindow(board[i][j], SW_SHOW);
                }
                else
                {
                    ShowWindow(board[i][j], SW_HIDE);
                }
                //ShowWindow(board[i][j], SW_HIDE);

            }
        }
        //ShowWindow(board[applePos.x][applePos.y], SW_SHOW);
            
        Sleep(100);

        
    }
    UnhookWindowsHookEx(gHookHandle);

    DestroyWindow(board[0][0]);
    MessageBox(NULL, L"exit", L"exit - exited correctly", MB_OK);
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
        x, y, 60, 90,

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
        MessageBox(hwnd, L"Message Test", L"WM_DESTROY - problem not exited correctly", MB_OK);
        UnhookWindowsHookEx(gHookHandle);
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        MessageBox(hwnd, L"Message Test", L"WM_CLOSE - problem not exited correctly", MB_OK);
        UnhookWindowsHookEx(gHookHandle);
        break;
    case WM_QUIT:
        MessageBox(hwnd, L"Message Test", L"WM_QUIT - problem not exited correctly", MB_OK);
        UnhookWindowsHookEx(gHookHandle);
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
        if ((p.x + 1) > BoardLength)
            return true;
    case 3:
        if ((p.y + 1) > BoardHight)
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
        x = (rand() % BoardLength) + 1;
        y = (rand() % BoardHight) + 1;
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
LRESULT CALLBACK keyLoggerHook(int nCode, WPARAM wParam, LPARAM lParam) {

    if (wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        
        if (direction % 2 != 0)
        {
            if (p->vkCode == VK_RIGHT)
            {
                direction = 2;
            }
            if (p->vkCode == VK_LEFT)
            {
                direction = 4;
            }
        }
        else
        {
            if (p->vkCode == VK_UP)
            {
                direction = 1;
            }
            if (p->vkCode == VK_DOWN)
            {
                direction = 3;
            }
        }
        

        return CallNextHookEx(gHookHandle, nCode, wParam, lParam);// must go to the next hook or bad things will happned
    }
    

    return CallNextHookEx(gHookHandle, nCode, wParam, lParam);// must go to the next hook or bad things will happned
}