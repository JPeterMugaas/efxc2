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

static int LoadFile(const std::filesystem::path& currentFile, int verbose, char** buf, std::uintmax_t* fileSize) {
    std::ifstream f;
    std::error_code ec;
    *fileSize = std::filesystem::file_size(currentFile, ec);
    if (ec.value() == 0) {
        if (verbose) {
#ifdef _WIN32
            std::wcout << std::format(L"Found {}\n", currentFile.native());
#else
            std::cout << std::format("Found {}\n", currentFile.native());
#endif
        }
        *buf = new char[*fileSize];
        f = std::ifstream(currentFile, std::ios::in);
        f.read(*buf, *fileSize);
        f.close();
        return true;
    }
    else {
        return false;
    }
}

HRESULT CompilerIncludes::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) {
    *ppData = nullptr;
    *pBytes = 0;
    std::filesystem::path Filename = std::string(pFileName);
    char* buf = nullptr;
    std::uintmax_t fileSize = 0;
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
    if  ((!input_parent_path.empty()) &&
        (LoadFile(input_parent_path /= Filename, verbose, &buf, &fileSize))) {
        *ppData = buf;
        *pBytes = (UINT)fileSize;
        return S_OK;
    }
    if (LoadFile(Filename, verbose, &buf, &fileSize)) {
        *ppData = buf;
        *pBytes = (UINT)fileSize;
        return S_OK;
    }
    std::filesystem::path currentFile;
    for (int i = 0; i < dirs.size(); ++i) {
        currentFile = dirs[i] /= Filename.native();
        if (LoadFile(currentFile, verbose, &buf, &fileSize)) {
            *ppData = buf;
            *pBytes = (UINT)fileSize;
            return S_OK;
        }
    }
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
    dirs.emplace(dirs.end(), dir);
}
