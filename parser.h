#pragma once

#include "shared.h"

typedef struct node {
    struct node* next;
    char* string;
} node;


PIMAGE_SECTION_HEADER WINAPI parsePE(LPWSTR);
node* WINAPI searchForStrings(LPSTR, DWORD);