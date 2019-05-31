// crt_vsnwprintf.c
// compile by using: cl /W3 crt_vsnwprintf.c

// To turn off error C4996, define this symbol:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <wtypes.h>

#define BUFFCOUNT (10)

void FormatOutput(LPCWSTR formatstring, ...)
{
    int nSize = 0;
    wchar_t buff[BUFFCOUNT];
    memset(buff, 0, sizeof(buff));
    va_list args;
    va_start(args, formatstring);
    // Note: _vsnwprintf is deprecated; consider vsnwprintf_s instead
    nSize = _vsnwprintf(buff, BUFFCOUNT - 1, formatstring, args); // C4996
    wprintf(L"nSize: %d, buff: %ls\n", nSize, buff);
    va_end(args);
}

int main() {
    FormatOutput(L"%ls %ls", L"Hi", L"there");
    FormatOutput(L"%ls %ls", L"Hi", L"there!");
    FormatOutput(L"%ls %ls", L"Hi", L"there!!");
    system("pause");
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//    PSTR szCmdLine, int iCmdShow)
//{
//    FormatOutput(L"%ls %ls", L"Hi", L"there");
//    FormatOutput(L"%ls %ls", L"Hi", L"there!");
//    FormatOutput(L"%ls %ls", L"Hi", L"there!!");
//    system("pause");
//
//    return 0;
//}