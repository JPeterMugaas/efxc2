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
    std::ifstream f;
    f.open(std::filesystem::path(inputFile));
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", inputFile.native());
#else
        std::cerr << std::format("Can not open {}", inputFile.native());
#endif
        exit(1);
    }
    params.LoadSourceCode(f);
    f.close();
}

void Files::WriteDisassembly(Compiler& compiler, const CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(DisassemblyFile), std::ios::out);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", DisassemblyFile.native());
#else
        std::cerr << std::format("Can not open {}", DisassemblyFile.native());
#endif
        exit(1);
    }
    size_t  outputLen = 0;
    outputLen = compiler.WriteAssemblyCode(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, DisassemblyFile.native());
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, DisassemblyFile.native());
#endif
    }
}

void Files::WriteIncludeFile(Compiler& compiler, const CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream( std::filesystem::path(IncludeFile), std::ios::out);
    if (!f.is_open() ) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", IncludeFile.native());
#else
        std::cerr << std::format("Can not open {}", IncludeFile.native());
#endif
        exit(1);
    }
    size_t  outputLen = 0;
    outputLen = compiler.WriteIncludeFile(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, IncludeFile.native());
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, IncludeFile.native());
#endif
    }
}

void Files::WriteObjectFile(Compiler& compiler, const CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(ObjectFile), std::ios::out | std::ios::binary);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", ObjectFile.native());
#else
        std::cerr << std::format("Can not open {}", ObjectFile.native());
#endif
        exit(1);
    }
    size_t  outputLen = 0;
    outputLen = compiler.WriteObjectFile(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, ObjectFile.native());
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, ObjectFile.native());;
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
            std::wcerr << std::format(L"Can not open {}", pdbFile.native());
#else
            std::cerr << std::format("Can not open {}", pdbFile.native());
#endif
            exit(1);
        }
        outputLen = compiler.WritePDBFile(f);
        f.close();
        if (params.get_verbose()) {
#ifdef _WIN32
            std::wcout << format(L"Wrote {} bytes of .PDB data to {}\n", outputLen, pdbFile.native());
#else
            std::cout << std::format("Wrote {} bytes of .PDB data to {}\n", outputLen, pdbFile.native());
#endif
        }
    }
}