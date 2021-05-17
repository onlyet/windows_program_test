#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("SysMets2");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject
    (WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Get System Metrics No. 2"),
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,   //添加垂直滚动条
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,
    LPARAM lParam)
{
    static int cxChar, cxCaps, cyChar, cyClient, iVscrollPos;
    HDC hdc;
    int i, y;
    PAINTSTRUCT ps;
    TCHAR szBuffer[10];
    TEXTMETRIC tm;
    switch (message)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
        cyChar = tm.tmHeight + tm.tmExternalLeading;

        ReleaseDC(hwnd, hdc);
        SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
        SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
        return 0;

    case WM_SIZE:
        cyClient = HIWORD(lParam);
        return 0;

    case WM_VSCROLL:
        switch (LOWORD(wParam)) //滚动类型
        {
        case SB_LINEUP:
            iVscrollPos -= 1;
            break;

        case SB_LINEDOWN:
            iVscrollPos += 1;
            break;

        case SB_PAGEUP:
            iVscrollPos -= cyClient / cyChar;
            break;

        case SB_PAGEDOWN:
            iVscrollPos += cyClient / cyChar;
                break;

        //滚动条拖动时一直触发
        case SB_THUMBTRACK:
            iVscrollPos = HIWORD(wParam);   //当前位置
            break;

        ////滚动条释放时触发
        //case SB_THUMBPOSITION:
        //    iVscrollPos = HIWORD(wParam);
        //    break;

        default:
            break;
        }

        iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));
        if (iVscrollPos != GetScrollPos(hwnd, SB_VERT))
        {
            //移动滚动条位置，不调用则会迅速跳回原来的位置
            SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
            //无效化客户区，触发WM_PAINT
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        for (i = 0; i < NUMLINES; i++)
        {
            y = cyChar * (i - iVscrollPos);
            TextOut(hdc, 0, y,
                sysmetrics[i].szLabel,
                lstrlen(sysmetrics[i].szLabel));

            TextOut(hdc, 22 * cxCaps, y,
                sysmetrics[i].szDesc,
                lstrlen(sysmetrics[i].szDesc));

            SetTextAlign(hdc, TA_RIGHT | TA_TOP);
            TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer,
                wsprintf(szBuffer, TEXT("%5d"),
                    GetSystemMetrics
                    (sysmetrics[i].Index)));
            SetTextAlign(hdc, TA_LEFT | TA_TOP);
        }
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}