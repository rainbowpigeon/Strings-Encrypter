#pragma once

#include "shared.h"

VOID WINAPI writeFile(LPCWSTR, PBYTE, DWORD);
LPWSTR WINAPI readFile(LPCWSTR);