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

void efxc2Files::Files::LoadInputFile(efxc2CompilerParams::CompilerParams& params) const {
    std::ifstream f;
    std::filesystem::path _path = inputFile;
    (void)f.open(_path);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", inputFile.native());
#else
        std::cerr << std::format("Can not open {}", inputFile.native());
#endif
        exit(1);  //-V2014 //-V3506 //-V2509
    }
    params.get_includeDirs()->set_input_parent_path(_path.parent_path());
    params.LoadSourceCode(f);
    f.close();
}

void efxc2Files::Files::LoadPrivateDataFile(efxc2CompilerParams::CompilerParams& params) const
{
    std::ifstream f;
    std::filesystem::path _path = privateDataFile;
    (void)f.open(_path);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", privateDataFile.native());
#else
        std::cerr << std::format("Can not open {}", privateDataFile.native());
#endif
        exit(1); //-V2014  //-V3506 //-V2509
    }
    params.LoadPrivateData(f);
    f.close();
}

void efxc2Files::Files::WriteDisassembly(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(DisassemblyFile), std::ios::out);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", DisassemblyFile.native());
#else
        std::cerr << std::format("Can not open {}", DisassemblyFile.native());
#endif
        exit(1); //-V2014 //-V2509 //-V3506
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

void efxc2Files::Files::WriteIncludeFile(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(IncludeFile), std::ios::out);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", IncludeFile.native());
#else
        std::cerr << std::format("Can not open {}", IncludeFile.native());
#endif
        exit(1); //-V2014  //-V3506 //-V2509
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

void efxc2Files::Files::WriteObjectFile(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(ObjectFile), std::ios::out | std::ios::binary);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", ObjectFile.native());
#else
        std::cerr << std::format("Can not open {}", ObjectFile.native());
#endif
        exit(1);  //-V2014 //-V3506 //-V2509
    }
    size_t  outputLen = 0;
    outputLen = compiler.WriteObjectFile(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, ObjectFile.native());
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, ObjectFile.native());
#endif
    }
}

void efxc2Files::Files::WritePreprocessFile(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) const {
    std::ofstream f;
    f = std::ofstream(std::filesystem::path(preprocessFile ), std::ios::out | std::ios::binary);
    if (!f.is_open()) {
#ifdef _WIN32
        std::wcerr << std::format(L"Can not open {}", preprocessFile.native());
#else
        std::cerr << std::format("Can not open {}", preprocessFile.native());
#endif
        exit(1); //-V2014  //-V3506 //-V2509
    }
    size_t  outputLen = 0;
    outputLen = compiler.WritePreprocessFile(f);
    f.close();
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Wrote {} bytes of shader output to {}\n", outputLen, preprocessFile.native());
#else
        std::cout << std::format("Wrote {} bytes of shader output to {}\n", outputLen, preprocessFile.native());
#endif
    }
}

void efxc2Files::Files::WritePDBFile(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) {
    
    /*write .PDB data if applicable*/
    if (pdbFile.empty() == false) {
        if (std::string pdbFileStr = (pdbFile.filename().string()); !pdbFileStr.empty() &&
            (pdbFileStr.compare(".") != 0) &&    //-V3551 //-V2578
            (pdbFileStr.compare("..") != 0)) {   //-V3551 //-V2578
#ifdef _WIN32
            auto c_pdbFile = efxc2Utils::wstring_to_utf8(pdbFile.filename().native());
            compiler.SetPDBFileName(c_pdbFile);
#else
            compiler.SetPDBFileName(pdbFile.filename().native());
#endif
        }
        else {
            /* if only a dir was specified, use the default
            filename in the shader data. */
            auto pPDBFileName = compiler.GetPDBFileName();
            pdbFileStr = pdbFile.filename().string();
            if ((pdbFileStr.compare(".") == 0) ||   //-V3551 //-V2578
                (pdbFileStr.compare("..") == 0)) {  //-V3551 //-V2578
                   (void)pPDBFileName.insert(pPDBFileName.begin(), std::filesystem::path::preferred_separator);
            }
#ifdef _WIN32
            auto w_pdbFile = pdbFile.native() + efxc2Utils::utf8_to_wstring(pPDBFileName);
            pdbFile = std::filesystem::path(w_pdbFile);
#else
            auto c_pdbFile = pdbFile.native() + pPDBFileName;
            pdbFile = std::filesystem::path(c_pdbFile);
#endif
        }
        std::ofstream f;
        f = std::ofstream(pdbFile, std::ios::out | std::ios::binary);
        if (!f) {
#ifdef _WIN32
            std::wcerr << std::format(L"Can not open {}", pdbFile.native());
#else
            std::cerr << std::format("Can not open {}", pdbFile.native());
#endif
            exit(1);  //-V2014  //-V3506 //-V2509
        }
        size_t  outputLen = 0;
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