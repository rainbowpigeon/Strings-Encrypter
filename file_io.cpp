#include "file_io.h"

VOID WINAPI writeFile(LPCWSTR filepath, PBYTE filecontent, DWORD contentlen) {
    HANDLE hFile = CreateFileW(
        filepath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_ARCHIVE,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        if (WriteFile(hFile, filecontent, contentlen, NULL, NULL) == TRUE) {
            CloseHandle(hFile);
        }
    }
}

LPWSTR WINAPI readFile(LPCWSTR filepath) {
    HANDLE hFile = NULL;
    hFile = CreateFileW(
        filepath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER fileSize;
        if (GetFileSizeEx(hFile, &fileSize)) {
            wchar_t* fileData = (wchar_t*)HeapAlloc(GetProcessHeap(), 0, fileSize.QuadPart);
            if (fileData != NULL) {
                if (ReadFile(hFile, fileData, fileSize.QuadPart, NULL, NULL) == TRUE) {
                    CloseHandle(hFile);
                    return fileData; // Caller needs to HeapFree this allocated memory
                }
                HeapFree(GetProcessHeap(), 0, fileData);
            }
        }
        CloseHandle(hFile);
    }
    return NULL;
}