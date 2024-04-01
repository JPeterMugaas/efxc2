//--------------------------------------------------------------------------------------
// File: efxc2Files.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#include "efxc2Utils.h"
#include "efxc2Files.h"

void Files::WriteDisassembly(Compiler& compiler, CompilerParams& params) {
    FILE* f;
#ifdef _WIN32
    errno_t err = 0;
#endif
    size_t  outputLen = 0;
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
    err = _wfopen_s(&f, DisassemblyFile, L"w");
    if (err != 0) {
        print_errno(err);
    }
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
    f = fopen(DisassemblyFile, "w");
    if (f == nullptr) {
        print_errno();
    }
#endif
    outputLen = compiler.WriteAssemblyCode(f);
    _Analysis_assume_(f != NULL);
    fclose(f);
    if (params.get_verbose()) {
#ifdef _WIN32
        wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, DisassemblyFile);
#else   /* _WIN32 */
        printf("Wrote %zu", outputLen);
        printf(" bytes of shader output to %ls\n", DisassemblyFile);
#endif  /* WIN32 */
    }
}

void Files::WriteIncludeFile(Compiler& compiler,  CompilerParams& params) {
    FILE* f;
#ifdef _WIN32
    errno_t err = 0;
#endif
    size_t  outputLen = 0;
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
    err = _wfopen_s(&f, IncludeFile, L"w");
    if (err != 0) {
        print_errno(err);
    }
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
    f = fopen(IncludeFile, "w");
    if (f == nullptr) {
        print_errno();
    }
#endif
    outputLen = compiler.WriteIncludeFile(f);
    _Analysis_assume_(f != NULL);
    fclose(f);
    if (params.get_verbose()) {
#ifdef _WIN32
        wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, IncludeFile);
#else   /* _WIN32 */
        printf("Wrote %zu", outputLen);
        printf(" bytes of shader output to %ls\n", IncludeFile);
#endif  /* WIN32 */
    }
}

void Files::WriteObjectFile(Compiler& compiler, CompilerParams& params) {
    FILE* f;
#ifdef _WIN32
    errno_t err = 0;
#endif
    size_t  outputLen = 0;
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
    err = _wfopen_s(&f, ObjectFile, L"w");
    if (err != 0) {
        print_errno(err);
    }
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
    f = fopen(ObjectFile, "w");
    if (f == nullptr) {
        print_errno();
    }
#endif
    outputLen = compiler.WriteObjectFile(f);
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
    fclose(f);
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
    if (params.get_verbose()) {
#ifdef _WIN32
        wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, ObjectFile);
#else   /* _WIN32 */
        printf("Wrote %zu", outputLen);
        printf(" bytes of shader output to %ls\n", ObjectFile);
#endif
    }
}

void Files::WritePDBFile(Compiler& compiler, CompilerParams& params) {
    FILE* f;
    int	AppendSlash = false;
#ifdef _WIN32
    errno_t err = 0;
#endif
    size_t  outputLen = 0;
    /*write .PDB data if applicable*/
#ifdef _WIN32
    if (c_pdbFile != nullptr) {
        auto pPDBFileName = GetFileName(c_pdbFile, &AppendSlash);
#else
    if (pdbFile != nullptr) {
        auto pPDBFileName = GetFileName(pdbFile, &AppendSlash);
#endif
        if (pPDBFileName != nullptr) {
            compiler.SetPDBFileName(pPDBFileName);
        }
        else {
            /* if only a dir was specified, use the default
            filename in the shader data. */
            pPDBFileName = compiler.GetPDBFileName();
#ifdef _WIN32
            wchar_t* w_PDBFileName = utf8_decode(pPDBFileName, strlen(pPDBFileName));
            if (AppendSlash) {
                pdbFile = concat(pdbFile, L"\\");
            }
            pdbFile = concat(pdbFile, w_PDBFileName);
            free(w_PDBFileName);
#else
            if (AppendSlash) {
                pdbFile = concat(pdbFile, "/");
            }
            pdbFile = concat(pdbFile, pPDBFileName);
#endif
        }
#ifdef _WIN32
        err = _wfopen_s(&f, pdbFile, L"w");
        if (err != 0) {
            print_errno(err);
        }
#else
        f = fopen(IncludeFile, "w");
        if (f == nullptr) {
            print_errno();
        }
#endif
        outputLen = compiler.WritePDBFile(f);
        _Analysis_assume_(f != NULL);
        fclose(f);
#ifdef _WIN32
        wprintf(L"Wrote %zu bytes of .PDB data to %ls\n", outputLen, pdbFile);
#else   /* _WIN32 */
        printf("Wrote %zu", outputLen);
        printf(" bytes of .PDB data to %ls\n", pdbFile);
#endif  /* WIN32 */
    }
}