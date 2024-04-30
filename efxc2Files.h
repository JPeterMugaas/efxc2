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
#endif /* _WIN32 */
    std::filesystem::path get_DisassemblyFile() const { return DisassemblyFile; }
    void set_DisassemblyFile(const M_STRING_VIEW& _DisassemblyFile) { DisassemblyFile = _DisassemblyFile; DisassemblyFile.make_preferred(); }
    std::filesystem::path get_inputFile() const { return inputFile;  }
    void set_inputFile(const M_STRING_VIEW& _inputFile) { inputFile = _inputFile; inputFile.make_preferred(); }
    std::filesystem::path get_privateDataFile() const { return privateDataFile;  }
    void set_privateDataFile(const M_STRING_VIEW& _privateDataFile) { privateDataFile = _privateDataFile; privateDataFile.make_preferred(); }
    std::filesystem::path get_IncludeFile() const { return IncludeFile; }
    void set_IncludeFile(const M_STRING_VIEW& _IncludeFile) { IncludeFile = _IncludeFile; IncludeFile.make_preferred(); }
    std::filesystem::path get_ObjectFile() const { return ObjectFile;  }
    void set_ObjectFile(const M_STRING_VIEW& _ObjectFile) { ObjectFile = _ObjectFile;  ObjectFile.make_preferred();  }
    std::filesystem::path get_pdbFile() const { return pdbFile;  };
    void set_pdbFile(const M_STRING_VIEW& _pdbFile) { pdbFile = _pdbFile; pdbFile.make_preferred(); }
    std::filesystem::path get_preprocessFile() const { return preprocessFile; }
    void set_preprocessFile(const M_STRING_VIEW& _preprocessFile) {
        preprocessFile = _preprocessFile; preprocessFile.make_preferred();
    }
    void LoadInputFile(CompilerParams& params) const;
    void LoadPrivateDataFile(CompilerParams& params) const;
    void WriteDisassembly(Compiler& compiler, const CompilerParams& params) const;
    void WriteIncludeFile(Compiler& compile, const CompilerParams& params) const;
    void WriteObjectFile(Compiler& compiler, const CompilerParams& params) const;
    void WritePDBFile(Compiler& compiler, const CompilerParams& params);
    void WritePreprocessFile(Compiler& compiler, const CompilerParams& params) const;
private:

    std::filesystem::path DisassemblyFile = M_STRING_INIT;
    std::filesystem::path inputFile = M_STRING_INIT;
    std::filesystem::path privateDataFile = M_STRING_INIT;
    std::filesystem::path IncludeFile = M_STRING_INIT;
    std::filesystem::path ObjectFile = M_STRING_INIT;
    std::filesystem::path pdbFile = M_STRING_INIT;
    std::filesystem::path preprocessFile = M_STRING_INIT;
    std::filesystem::path temp = M_STRING_INIT;
};

#endif /*EFXC2FILES_H_INCLUDED*/