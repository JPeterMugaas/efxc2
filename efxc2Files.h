//--------------------------------------------------------------------------------------
// File: efxc2Files.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef EFXC2FILES_H_INCLUDED
#define EFXC2FILES_H_INCLUDED
#include "efxc2.h"
#include "efxc2CompilerParams.h"
#include "efxc2Compiler.h"
#include "efxc2Utils.h"

class Files {
public:
#ifdef _WIN32
    std::wstring get_DisassemblyFile() const { return DisassemblyFile;  }
    void set_DisassemblyFile(const std::wstring& _DisassemblyFile) { 
        DisassemblyFile = _DisassemblyFile;  FixupFileName(DisassemblyFile);  }
    std::wstring get_inputFile() const { return inputFile; }
    void set_inputFile(const std::wstring& _inputFile) { 
        inputFile = _inputFile; FixupFileName(inputFile); }
    std::wstring get_IncludeFile() const { return IncludeFile; }
    void set_IncludeFile(const std::wstring& _IncludeFile) { 
        IncludeFile = _IncludeFile; FixupFileName(IncludeFile); }
    std::wstring get_ObjectFile() const { return ObjectFile; }
    void set_ObjectFile(const std::wstring& _ObjectFile) {
        ObjectFile = _ObjectFile; FixupFileName(ObjectFile); }
    std::wstring get_pdbFile() const { return pdbFile; };
    void set_pdbFile(const std::wstring& _pdbFile) { 
        pdbFile = _pdbFile; FixupFileName(pdbFile); }
    std::string get_c_pdbFile() const { return c_pdbFile; };
    void set_c_pdbFile(const std::string& _c_pdbFile) { 
        c_pdbFile = _c_pdbFile; FixupFileName(c_pdbFile); }
#else  /* _WIN32 */
    std::string get_DisassemblyFile() const { return DisassemblyFile; }
    void set_DisassemblyFile(const std::string& _DisassemblyFile) { DisassemblyFile = _DisassemblyFile; }
    std::string get_inputFile() const { return inputFile;  }
    void set_inputFile(const std::string& _inputFile) { inputFile = _inputFile;  }
    std::string get_IncludeFile() const { return IncludeFile; }
    void set_IncludeFile(const std::string& _IncludeFile) { IncludeFile = _IncludeFile;  }
    std::string get_ObjectFile() const { return ObjectFile;  }
    void set_ObjectFile(const std::string& _ObjectFile) { ObjectFile = _ObjectFile;  }
    std::string get_pdbFile() const { return pdbFile;  };
    void set_pdbFile(const std::string& _pdbFile) { pdbFile = _pdbFile; }
#endif /* _WIN32 */

    void LoadInputFile(CompilerParams& params);
    void WriteDisassembly(Compiler& compiler, const CompilerParams& params) const;
    void WriteIncludeFile(Compiler& compile, const CompilerParams& params) const;
    void WriteObjectFile(Compiler& compiler, const CompilerParams& params) const;
    void WritePDBFile(Compiler& compiler, const CompilerParams& params);
private:
#ifdef _WIN32
    std::wstring DisassemblyFile = L"";
    std::wstring inputFile = L"";
    std::wstring IncludeFile = L"";
    std::wstring ObjectFile = L"";
    std::wstring pdbFile = L"";
    std::wstring temp = L"";
    std::string c_pdbFile = "";
#else  /* _WIN32 */
    std::string DisassemblyFile = "";
    std::string inputFile = "";
    std::string IncludeFile = "";
    std::string ObjectFile = "";
    std::string pdbFile = "";
    std::string temp = "";
#endif /* _WIN32 */
};

#endif /*EFXC2FILES_H_INCLUDED*/