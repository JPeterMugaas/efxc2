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
#include "efxc2Compiler.h"

class Files {
public:
#ifdef _WIN32
    wchar_t* get_DisassemblyFile() const { return DisassemblyFile;  }
    void set_DisassemblyFile(wchar_t* _DisassemblyFile) { DisassemblyFile = _DisassemblyFile;  }
    wchar_t* get_inputFile() const { return inputFile; }
    void set_inputFile(wchar_t* _inputFile) { inputFile = _inputFile; }
    wchar_t* get_IncludeFile() const { return IncludeFile; }
    void set_IncludeFile(wchar_t* _IncludeFile) { IncludeFile = _IncludeFile; }
    wchar_t* get_ObjectFile() const { return ObjectFile; }
    void set_ObjectFile(wchar_t* _ObjectFile) { ObjectFile = _ObjectFile; }
    wchar_t* get_pdbFile() const { return pdbFile; };
    void set_pdbFile(wchar_t* _pdbFile) { pdbFile = _pdbFile; }
    char* get_c_pdbFile() const { return c_pdbFile; };
    void set_c_pdbFile(char* _c_pdbFile) { c_pdbFile = _c_pdbFile; }
#else  /* _WIN32 */
    char* get_DisassemblyFile() const { return DisassemblyFile; }
    void set_DisassemblyFile(char* _DisassemblyFile) { DisassemblyFile = _DisassemblyFile; }
    char* get_inputFile() const { return inputFile;  }
    void set_inputFile(char* _inputFile) { inputFile = _inputFile;  }
    char* get_IncludeFile() const { return IncludeFile; }
    void set_IncludeFile(char* _IncludeFile) { IncludeFile = _IncludeFile;  }
    char* get_ObjectFile() const { return ObjectFile;  }
    void set_ObjectFile(char* _ObjectFile) { ObjectFile = _ObjectFile;  }
    char* get_pdbFile() const { return pdbFile;  };
    void set_pdbFile(char* _pdbFile) { pdbFile = _pdbFile; }
#endif /* _WIN32 */
    void WriteDisassembly(Compiler& compiler);
    void WritePDBFile(Compiler& compiler);
    void WriteIncludeFile(Compiler& compiler);
    void WriteObjectFile(Compiler& compiler);
private:
#ifdef _WIN32
    wchar_t* DisassemblyFile = nullptr;
    wchar_t* inputFile = nullptr;
    wchar_t* IncludeFile = nullptr;
    wchar_t* ObjectFile = nullptr;
    wchar_t* pdbFile = nullptr;
    wchar_t* temp = nullptr;
    char* c_pdbFile = nullptr;
#else  /* _WIN32 */
    char* DisassemblyFile = nullptr;
    char* inputFile = nullptr;
    char* IncludeFile = nullptr;
    char* ObjectFile = nullptr;
    char* pdbFile = nullptr;
    char* temp = nullptr;
#endif /* _WIN32 */
};

#endif /*EFXC2FILES_H_INCLUDED*/