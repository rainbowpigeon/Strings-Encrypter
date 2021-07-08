#include "strings_encrypter.h"

int main() {
    const DWORD MAX_FILEPATH = 256;
    wchar_t rfileName[MAX_FILEPATH] = L"";
    wchar_t wfileName[MAX_FILEPATH] = L"";
    LPWSTR fileData = readFile(rfileName); // need to HeapFree

    if (fileData != NULL) {
        // find section header for .data
        PIMAGE_SECTION_HEADER pSecHeader = parsePE(fileData);
        if (pSecHeader != NULL) {
            PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)fileData;
            char* pointerToData = (char*)((ULONG_PTR)dosHeader + pSecHeader->PointerToRawData);
            node* head = searchForStrings(pointerToData, pSecHeader->SizeOfRawData); // head pointer to linked list containing found strings
            if (head != NULL) {
                PBYTE pbKeyObject = initCrypt(); // initCrypt() modifies global hKey
                if (hKey != INVALID_HANDLE_VALUE) {
                    node* temp;
                    while (head != NULL) { // traverse linked list and encrypt each string
                        encrypt(head->string, hKey);
                        temp = head;
                        head = head->next;
                        HeapFree(GetProcessHeap(), 0, temp);
                    }

                    NTSTATUS status;
                    if (!NT_SUCCESS(status = BCryptDestroyKey(hKey))) {
                        OutputDebugStringW(L"Error destroying key.");
                    }

                    hKey = 0;
                    writeFile(wfileName, (PBYTE)fileData, HeapSize(GetProcessHeap(), 0, fileData));
                }
                else {
                    ;
                }
                HeapFree(GetProcessHeap(), 0, pbKeyObject); // key object memory cannot be freed before the destruction of hKey
            }
        }
        HeapFree(GetProcessHeap(), 0, fileData);
    }
}

