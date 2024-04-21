//--------------------------------------------------------------------------------------
// File: efxc2CompilerIncludes.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

/*
This unit does cause some warnings with SonarLint.I'm not sure what to do about these warnings
since the unit is part of an inheritance callback scheme from a .DLL.   The API is defined in
d3dcommon.h.
*/

#include "efxc2CompilerIncludes.h"

HRESULT CompilerIncludes::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) {
    std::filesystem::path Filename = std::string(pFileName);

    if (verbose) {
        std::cout << "Called CompilerIncludes::Open(\n";
        switch (IncludeType) {
        case D3D_INCLUDE_LOCAL: // #include "FILE"
            std::cout << "\tIncludeType: D3D_INCLUDE_LOCAL\n";
            break;
        case D3D_INCLUDE_SYSTEM: // #include <FILE>
            std::cout << "\tIncludeType: D3D_INCLUDE_SYSTEM\n";
            break;
        /* D3D10_INCLUDE_LOCAL is an alias for D3D_INCLUDE_LOCAL.
           D3D10_INCLUDE_SYSTEM is an alias for D3D_INCLUDE_SYSTEM  */
        default:;
        }
#ifdef _WIN32
        std::wcout << std::format(L"\tpFileName: {}\n", Filename.native());
#else
        std::cout << std::format("\tpFileName {}\n", Filename.native());
#endif
        if (pParentData != nullptr) {
            std::cout << "\tpParentData: *****)\n";
        }
        else {
            std::cout << "\tpParentData: nullptr)\n";
        }
    }
    std::ifstream f;
    std::error_code ec;
    std::uintmax_t fileSize = std::filesystem::file_size(Filename, ec);
    if (ec.value() == 0) {
        if (verbose) {
#ifdef _WIN32
            std::wcout << std::format(L"Found {}\n", Filename.native());
#else
            std::cout << std::format("Found {}\n", Filename.native());
#endif
        }
        auto buf = new char[fileSize];
        f = std::ifstream(std::filesystem::path(Filename), std::ios::in);
        f.read(buf, fileSize);
        f.close();
        *ppData = buf;
        *pBytes = (UINT)fileSize;
        return S_OK;
    }
    else {
        char* buf = nullptr;
        M_STRING currentFile;
        for (int i = 0; i < dirs.size(); ++i) {
            currentFile = dirs[i] + std::filesystem::path::preferred_separator;
            currentFile = currentFile + Filename.native();
            std::uintmax_t fileSize = std::filesystem::file_size(currentFile, ec);
            if (ec.value() == 0) {
                if (verbose) {
#ifdef _WIN32
                    std::wcout << std::format(L"Found {}\n", currentFile);
#else
                    std::cout << std::format("Found {}\n", currentFile);
#endif
                }
                buf = new char[fileSize];
                f = std::ifstream(std::filesystem::path(currentFile), std::ios::in);
                f.read(buf, fileSize);
                f.close();
                *ppData = buf;
                *pBytes = (UINT)fileSize;
                return S_OK;
            }
        }
    }
    *ppData = nullptr;
    *pBytes = 0;
    return E_FAIL;
}

/* do not change this signature, it's part of an "inheritance" API. */
HRESULT CompilerIncludes::Close(LPCVOID pData) {
    if (verbose) {
        std::cout << "Called CompilerIncludes::Close(\n";
        if (pData != nullptr) {
            std::cout << "\tpData: *****\n";
        }
        else {
            std::cout << "\tpData: nullptr\n";
        }
    }
    auto buf = (char*)pData;
    delete[] buf;
    return S_OK;
}

/* do not change this signature, it's part of an "inheritance" API. */
void CompilerIncludes::AddIncludeDir(const M_STRING_VIEW& _dir)
{
    M_STRING dir = { _dir.data(), _dir.size() };
    dirs.insert(dirs.end(), dir);
}
