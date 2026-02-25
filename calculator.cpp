#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define ID_EDIT1   101
#define ID_EDIT2   102
#define ID_BTN_ADD 201
#define ID_BTN_SUB 202
#define ID_BTN_MUL 203
#define ID_BTN_DIV 204

static HBRUSH g_bgBrush = NULL;

double readDoubleFromEdit(HWND hEdit) {
    char buf[256] = {0};
    GetWindowTextA(hEdit, buf, 255);
    return atof(buf);
}

void showResult(HWND hwnd, double value) {
    char msg[128];
    sprintf(msg, "%g", value);
    MessageBoxA(hwnd, msg, "Result", MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit1 = NULL, hEdit2 = NULL;

    switch(Message) {
        case WM_CREATE: {
            g_bgBrush = CreateSolidBrush(RGB(220, 0, 120));

            CreateWindowA(
                "STATIC", "Please input two numbers",
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                25, 20, 200, 25,
                hwnd, NULL, NULL, NULL
            );

            hEdit1 = CreateWindowA(
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                50, 55, 150, 25,
                hwnd, (HMENU)ID_EDIT1, NULL, NULL
            );

            hEdit2 = CreateWindowA(
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                50, 85, 150, 25,
                hwnd, (HMENU)ID_EDIT2, NULL, NULL
            );

            CreateWindowA("BUTTON", "+", WS_CHILD | WS_VISIBLE,
                          60, 125, 30, 25, hwnd, (HMENU)ID_BTN_ADD, NULL, NULL);

            CreateWindowA("BUTTON", "-", WS_CHILD | WS_VISIBLE,
                          95, 125, 30, 25, hwnd, (HMENU)ID_BTN_SUB, NULL, NULL);

            CreateWindowA("BUTTON", "*", WS_CHILD | WS_VISIBLE,
                          130, 125, 30, 25, hwnd, (HMENU)ID_BTN_MUL, NULL, NULL);

            CreateWindowA("BUTTON", "/", WS_CHILD | WS_VISIBLE,
                          165, 125, 30, 25, hwnd, (HMENU)ID_BTN_DIV, NULL, NULL);

            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            return (LRESULT)g_bgBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkColor(hdc, RGB(255, 255, 255));
            static HBRUSH editBrush = NULL;
            if (!editBrush) editBrush = CreateSolidBrush(RGB(255, 255, 255));
            return (LRESULT)editBrush;
        }

        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rc;
            GetClientRect(hwnd, &rc);
            FillRect(hdc, &rc, g_bgBrush);
            return 1;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);

            if (id == ID_BTN_ADD || id == ID_BTN_SUB || id == ID_BTN_MUL || id == ID_BTN_DIV) {
                double a = readDoubleFromEdit(hEdit1);
                double b = readDoubleFromEdit(hEdit2);
                double ans = 0.0;

                switch (id) {
                    case ID_BTN_ADD: ans = a + b; break;
                    case ID_BTN_SUB: ans = a - b; break;
                    case ID_BTN_MUL: ans = a * b; break;
                    case ID_BTN_DIV:
                        if (b == 0.0) {
                            MessageBoxA(hwnd, "Cannot divide by zero.", "Result", MB_OK | MB_ICONWARNING);
                            return 0;
                        }
                        ans = a / b;
                        break;
                }

                showResult(hwnd, ans);
            }
            break;
        }

        case WM_DESTROY: {
            if (g_bgBrush) {
                DeleteObject(g_bgBrush);
                g_bgBrush = NULL;
            }
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    memset(&wc,0,sizeof(wc));
    wc.cbSize      = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance   = hInstance;
    wc.hCursor     = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "WindowClass";
    wc.hIcon        = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm      = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    RECT r = {0, 0, 250, 200};
    DWORD style = WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&r, style, FALSE);

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "WindowClass",
        "My Calculator",
        WS_VISIBLE | style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left,
        r.bottom - r.top,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}