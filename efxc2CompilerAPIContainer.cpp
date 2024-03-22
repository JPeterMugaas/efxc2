//--------------------------------------------------------------------------------------
// File: efxc2CompilerAPIContainer.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2CompilerAPIContainer.h"
#include "efxc2Utils.h"

CompilerAPIContainer::CompilerAPIContainer() {
    h = LoadLibrary(DLL_NAME);
    if (h == nullptr) {
#ifdef _WIN32
        //Find the WINDOWS dll
        wchar_t dllPath[MAX_PATH];
        size_t bytes = GetModuleFileName(nullptr, dllPath, MAX_PATH);
        if (bytes == 0) {
            fprintf(stderr, "Could not retrieve the directory of the running executable.\n");
            print_windows_error();
        }
        //Fill rest of the buffer with NULLs
        memset(dllPath + bytes, '\0', MAX_PATH - bytes);
        //Copy the dll location over top efxc2.exe
        wcscpy_s(dllPath + bytes, MAX_PATH, DLL_NAME);

        h = LoadLibrary(dllPath);
        if (h == nullptr) {
            wprintf(L"Error: could not load " DLL_NAME L" from %s\n", dllPath);
        }

#else  /* _WIN32 */
        print_windows_error();
#endif /* _WIN32 */
    }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
    ptr_D3DCompile2 = (pD3DCompile2g)GetProcAddress(h, "D3DCompile2");
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    if (ptr_D3DCompile2 == nullptr) {
        printf("Error: could not get the address of D3DCompile2.\n");
        print_windows_error();
    }
    ptr_D3DStripShader = (pD3DStripShaderg)GetProcAddress(h, "D3DStripShader");
    if (ptr_D3DStripShader == nullptr) {
        printf("Error: could not get the address of D3DStripShader.\n");
        print_windows_error();
    }
    ptr_D3DGetBlobPart = (pD3DGetBlobPartg)GetProcAddress(h, "D3DGetBlobPart");
    if (ptr_D3DGetBlobPart == nullptr) {
        printf("Error: could not get the address of D3DGetBlobPart.\n");
        print_windows_error();
    }
    ptr_D3DSetBlobPart = (pD3DSetBlobPartg)GetProcAddress(h, "D3DSetBlobPart");
    if (ptr_D3DGetBlobPart == nullptr) {
        printf("Error: could not get the address of D3DSetBlobPart.\n");
        print_windows_error();
    }
    ptr_D3DDisassemble = (pD3DDisassembleg)GetProcAddress(h, "D3DDisassemble");
    if (ptr_D3DDisassemble == nullptr) {
        printf("Error: could not get the address of D3DDisassemble.\n");
        print_windows_error();
    }
}
