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

namespace efxc2CompilerIncludes {
    static void TrimTrailingWhiteSpace(const char* buf, std::uintmax_t* fileSize) {
        size_t tmp = *fileSize;
        for (size_t i = *fileSize; i > 0; --i) {
            if (buf[i - 1] >= 32) {  
                break;
            }
            else {
                tmp--;
            }
        }
        *fileSize = tmp;
        return;
    }

    static bool LoadFile(const std::filesystem::path& currentFile, int verbose, char** buf, std::uintmax_t* fileSize) { 
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
            (void)memset(*buf, 0, *fileSize);
            f = std::ifstream(currentFile, std::ios::in | std::ios::binary);
            if (f.is_open()) {
                (void)f.read(*buf, *fileSize);
                f.close();
                TrimTrailingWhiteSpace(*buf, fileSize);
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }

    __declspec(nothrow) HRESULT __stdcall efxc2CompilerIncludes::CompilerIncludes::Open(D3D_INCLUDE_TYPE IncludeType,
        LPCSTR pFileName, 
        LPCVOID pParentData, 
        LPCVOID* ppData, UINT* pBytes) { 

        *ppData = nullptr;
        *pBytes = 0;
        std::filesystem::path Filename = std::string(pFileName);
        char* buf = nullptr;
        std::uintmax_t fileSize = 0;
        if (verbose && debug) {
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
                break;
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
        std::filesystem::path TryInputFile = "";
        if (!input_parent_path.empty()) {
            TryInputFile = input_parent_path /= Filename; 
            if (LoadFile(TryInputFile, verbose, &buf, &fileSize)) {
                *ppData = buf;
                /* These is deliberately a UINT because of an API limitation in this .DLL inheritance callback. */
                *pBytes = (UINT)fileSize;  //-V2005
                return S_OK; 
            }
        }
        if (LoadFile(Filename, verbose, &buf, &fileSize)) {
            *ppData = buf;
            /* These is deliberately a UINT because of an API limitation in this .DLL inheritance callback. */
            *pBytes = (UINT)fileSize;  //-V2005
            return S_OK; 
        }
        for (std::filesystem::path& currentDir : dirs) {
            TryInputFile = currentDir /= Filename;  
            if (LoadFile(TryInputFile, verbose, &buf, &fileSize)) {
                *ppData = buf;
                /* These is deliberately a UINT because of an API limitation in this .DLL inheritance callback. */
                *pBytes = (UINT)fileSize;  //-V2005
                return S_OK;
            }
        }
        /* E_FAIL eas defined without a suffix in the Win32 API.*/
        return E_FAIL;  //-V3515 //-V2523
    }

    /* do not change this signature, it's part of an "inheritance" API. */
    __declspec(nothrow) HRESULT __stdcall efxc2CompilerIncludes::CompilerIncludes::Close(LPCVOID pData) {
        if (verbose && debug) {
            std::cout << "Called CompilerIncludes::Close(\n";
            if (pData != nullptr) {
                std::cout << "\tpData: *****\n";
            }
            else {
                std::cout << "\tpData: nullptr\n";
            }
        }
        auto buf = std::bit_cast<char*>(pData);  
        delete[] buf; 
        return S_OK;
    }

    /* do not change this signature, it's part of an "inheritance" API. */
    void efxc2CompilerIncludes::CompilerIncludes::AddIncludeDir(const efxc2Utils::M_STRING_VIEW _dir)
    {
        efxc2Utils::M_STRING dir = { _dir.data(), _dir.size() };

        (void) dirs.emplace(dirs.end(), dir);
        (void) dirs[dirs.size() - 1].make_preferred();  
    }

}