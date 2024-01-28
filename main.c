#include <windows.h>
#include <tchar.h>

const char g_szClassName[] = "myWindowClass";
#define IDC_ADD_BUTTON 101
#define IDC_TASK_EDIT  102
#define IDC_TASK_LIST  103

HWND hTaskList;
HWND hTaskEdit;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
            hTaskList = CreateWindowEx(0, TEXT("LISTBOX"), NULL,
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY,
                10, 10, 300, 200, hwnd, (HMENU)IDC_TASK_LIST, GetModuleHandle(NULL), NULL);

            hTaskEdit = CreateWindowEx(0, TEXT("EDIT"), NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL,
                10, 220, 200, 25, hwnd, (HMENU)IDC_TASK_EDIT, GetModuleHandle(NULL), NULL);

            CreateWindowEx(0, TEXT("BUTTON"), TEXT("Add"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                220, 220, 90, 25, hwnd, (HMENU)IDC_ADD_BUTTON, GetModuleHandle(NULL), NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_ADD_BUTTON && HIWORD(wParam) == BN_CLICKED) {
                TCHAR taskText[256];
                GetWindowText(hTaskEdit, taskText, sizeof(taskText) / sizeof(taskText[0]));

                if (_tcscmp(taskText, TEXT("")) != 0) {
                    SendMessage(hTaskList, LB_ADDSTRING, 0, (LPARAM)taskText);
                    SetWindowText(hTaskEdit, TEXT(""));
                }
            }
            break;
      
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 450,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}