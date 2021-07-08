#include "crypt.h"

BCRYPT_KEY_HANDLE hKey = NULL;

VOID WINAPI encrypt(char* string, BCRYPT_KEY_HANDLE hKey) {
    DWORD
        cbCipherText = 0,
        cbData = 0;
    PBYTE pbPlainText = (PBYTE)string;
    DWORD cbPlainText = strlen(string);

    // Get output buffer size required
    NTSTATUS status = BCryptEncrypt(
        hKey,
        pbPlainText,
        cbPlainText,
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        &cbCipherText,
        0
    );

    if (!NT_SUCCESS(status)) {
        return;
    }

    status = BCryptEncrypt(
        hKey,
        pbPlainText,
        cbPlainText,
        NULL,
        NULL,
        NULL,
        pbPlainText, // encryption in place so pbInput == pbOutput
        cbCipherText,
        &cbData,
        0
    );

    if (!NT_SUCCESS(status)) {
        return;
    }

}

PBYTE WINAPI initCrypt() {
    // choose your 16 bytes key here (128 bit)
    BYTE rgbAESKey[] = { 
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
    };
    DWORD
        cbKeyObject = 0,
        cbData;
    PBYTE pbKeyObject = NULL;

    NTSTATUS status = BCryptGetProperty(
        BCRYPT_RC4_ALG_HANDLE,
        BCRYPT_OBJECT_LENGTH,
        (PBYTE)&cbKeyObject,
        sizeof(DWORD),
        &cbData,
        0
    );

    if (!NT_SUCCESS(status)) {
        return NULL;
    }

    pbKeyObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
    if (NULL == pbKeyObject) {
        return NULL;
    }

    status = BCryptGenerateSymmetricKey(
        BCRYPT_RC4_ALG_HANDLE,
        &hKey,
        pbKeyObject,
        cbKeyObject,
        (PBYTE)rgbAESKey,
        sizeof(rgbAESKey),
        0
    );
    if (!NT_SUCCESS(status)) {
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
        // memset(pbKeyObject, 0, cbKeyObject);
        return NULL;
    }
    else {
        if (hKey != INVALID_HANDLE_VALUE) { ; }
    }

    exportBlobToFile(hKey);
    return pbKeyObject;

}

VOID WINAPI exportBlobToFile(BCRYPT_KEY_HANDLE hKey) {
    wchar_t blobPath[] = L"";
    NTSTATUS status;
    DWORD cbBlob = 0;
    PBYTE pbBlob = NULL;

    // get exported blob size required
    if (!NT_SUCCESS(status = BCryptExportKey(
        hKey,
        NULL,
        BCRYPT_OPAQUE_KEY_BLOB,
        NULL,
        0,
        &cbBlob,
        0
    ))) {
        return;
    }

    pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
    if (NULL == pbBlob) {
        return;
    }

    if (!NT_SUCCESS(status = BCryptExportKey(
        hKey,
        NULL,
        BCRYPT_OPAQUE_KEY_BLOB,
        pbBlob,
        cbBlob,
        &cbBlob,
        0
    ))) {
        return;
    }

    writeFile(blobPath, pbBlob, cbBlob);
    HeapFree(GetProcessHeap(), 0, pbBlob);
}
