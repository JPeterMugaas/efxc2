//--------------------------------------------------------------------------------------
// File: efxc2CompilerIncludes.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2CompilerIncludes.h"

HRESULT CompilerIncludes::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) {
#ifdef _WIN32
    std::wstring Filename = utf8_decode(pFileName);
#else
    std::string filename = pFileName;
#endif
    if (verbose) {
        std::cout << "Called CompilerIncludes::Open(\n";
#ifdef _WIN32
        std::wcout << std::format(L"\tpFileName: {}\n", Filename);
#else
        std::cout << std::format("\tpFileName {}\n", Filename);
#endif
        if (pParentData != nullptr) {
            std::cout << "/tpParentData: *****/n";
        }
        else {
            std::cout << "/tpParentData: nullptr/n";
        }
    }
    return E_FAIL;
}

/* do not change this signature, it's part of an "inheritance" API. */
HRESULT CompilerIncludes::Close(LPCVOID pData) {
    return E_FAIL;
}

/* do not change this signature, it's part of an "inheritance" API. */
void CompilerIncludes::AddIncludeDir(const M_STRING_VIEW& _dir)
{
    M_STRING dir = { _dir.data(), _dir.size() };
    dirs.insert(dirs.begin(), dir);
}
