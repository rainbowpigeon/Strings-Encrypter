#include "parser.h"

PIMAGE_SECTION_HEADER WINAPI parsePE(LPWSTR fileData) {
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)fileData;
    if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
        PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)fileData + dosHeader->e_lfanew);
        if (ntHeader->Signature == IMAGE_NT_SIGNATURE) { // PE signature check
            PIMAGE_SECTION_HEADER pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
            for (DWORD i = 0; i < ntHeader->FileHeader.NumberOfSections; i++) {
                if ((pSecHeader->Characteristics & 0x40) == IMAGE_SCN_CNT_INITIALIZED_DATA) {
                    if (strcmp((LPCSTR)pSecHeader->Name, ".data") == 0) {
                        printf("Found .data section.\n");
                        if (pSecHeader->SizeOfRawData != 0) {
                            return pSecHeader;
                        }
                    }
                }
                pSecHeader++;
            }
        }
    }
    return NULL;
}

node* WINAPI searchForStrings(char* pointerToData, DWORD sizeOfRawData) {
    node* head = NULL;
    node* curr = NULL;
    DWORD j = 0;
    while (j < sizeOfRawData) {
        // isprint function needs unsigned char
        if (isprint((unsigned char)*pointerToData) != 0) {
            char* stringStart = pointerToData;
            j++;
            pointerToData++;

            while (isprint((unsigned char)*pointerToData) != 0) {
                j++;
                pointerToData++;
            }

            if (*pointerToData == '\0') {
                printf("Found valid string: %s\n", stringStart);
                // insert new string into linked list
                node* temp = (node*)HeapAlloc(GetProcessHeap(), 0, sizeof(node));
                if (temp != NULL) {
                    temp->next = NULL;
                    temp->string = stringStart; // don't need new copy of string in linked list, just a reference
                    if (head == NULL) {
                        head = temp;
                        curr = temp;
                    }
                    else {
                        curr->next = temp;
                        curr = curr->next;
                    }
                }
            }
        }
        j++;
        pointerToData++;
    }
    return head;
}

