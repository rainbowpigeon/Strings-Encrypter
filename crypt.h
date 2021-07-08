#pragma once

#include "shared.h"
#include "file_io.h"
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

extern BCRYPT_KEY_HANDLE hKey;

PBYTE WINAPI initCrypt();
VOID WINAPI encrypt(LPSTR, BCRYPT_KEY_HANDLE);
VOID WINAPI exportBlobToFile(BCRYPT_KEY_HANDLE);