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

void Files::LoadInputFile(CompilerParams& params) const {
    FILE* f = nullptr;
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif  /* _MSC_VER */
    if (errno_t err = _wfopen_s(&f, inputFile.c_str(), L"r"); err != 0) {
        print_errno(err);
#ifdef _MSC_VER
#pragma warning( pop )
#endif  /* _MSC_VER */
#else
    f = fopen(inputFile.c_str(), "r");
    if (f == nullptr) {
        print_errno();
#endif  /* _WIN32 */
    }
    params.LoadSourceCode(f);
    _Analysis_assume_(f != NULL);
    fclose(f);
}

void Files::WriteDisassembly(Compiler& compiler, const CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(DisassemblyFile), std::ios::out);
    if (!f) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", DisassemblyFile);
#else
        std::cerr << std::format("Can not open {}", DisassemblyFile);
#endif
        exit(1);
    }
    size_t  outputLen = 0;
    outputLen = compiler.WriteAssemblyCode(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, DisassemblyFile);
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, DisassemblyFile);
#endif
    }
}

void Files::WriteIncludeFile(Compiler& compiler, const CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream( std::filesystem::path(IncludeFile), std::ios::out);
    if (!f) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", IncludeFile);
#else
        std::cerr << std::format("Can not open {}", IncludeFile);
#endif
        exit(1);
    }
    size_t  outputLen = 0;
    outputLen = compiler.WriteIncludeFile(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, IncludeFile);
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, IncludeFile);
#endif
    }
}

void Files::WriteObjectFile(Compiler& compiler, const CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(ObjectFile), std::ios::out | std::ios::binary);
    if (!f) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", ObjectFile);
#else
        std::cerr << std::format("Can not open {}", ObjectFile);
#endif
        exit(1);
    }
    size_t  outputLen = 0;
    outputLen = compiler.WriteObjectFile(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, ObjectFile);
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, ObjectFile);;
#endif
    }
}

void Files::WritePDBFile(Compiler& compiler, const CompilerParams& params) {
    std::ofstream f;
    size_t  outputLen = 0;
    int	AppendSlash = false;
    /*write .PDB data if applicable*/

#ifdef _WIN32
    if (c_pdbFile.empty() == false) {
        if (auto pPDBFileName = GetFileName(c_pdbFile, &AppendSlash); pPDBFileName.empty() == false) {
#else
    if (pdbFile.empty() == false) {
        if (auto pPDBFileName = GetFileName(pdbFile, &AppendSlash); pPDBFileName.empty() == false) {
#endif
            compiler.SetPDBFileName(pPDBFileName);
        }
        else {
            /* if only a dir was specified, use the default
            filename in the shader data. */
            pPDBFileName = compiler.GetPDBFileName();
#ifdef _WIN32
            std::wstring w_PDBFileName = utf8_decode(pPDBFileName);
            if (AppendSlash) {
                pdbFile.append(L"\\");
            }
            pdbFile.append(w_PDBFileName);
#else
            if (AppendSlash) {
                pdbFile.append("/");
            }
            pdbFile.append(pPDBFileName);
#endif
        }
        f = std::ofstream(std::filesystem::path(pdbFile), std::ios::out | std::ios::binary);
        if (!f) {
#ifdef _WIN32
            std::wcerr << std::format(L"Can not open {}", pdbFile);
#else
            std::cerr << std::format("Can not open {}", pdbFile);
#endif
            exit(1);
        }
        outputLen = compiler.WritePDBFile(f);
        f.close();
        if (params.get_verbose()) {
#ifdef _WIN32
            std::wcout << format(L"Wrote {} bytes of .PDB data to {}\n", outputLen, pdbFile);
#else
            std::cout << std::format("Wrote {} bytes of .PDB data to {}\n", outputLen, pdbFile);
#endif
        }
    }
}