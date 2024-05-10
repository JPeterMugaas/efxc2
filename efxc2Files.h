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

namespace efxc2Files{
    class Files {
    public:
#ifdef _WIN32
#endif /* _WIN32 */
        std::filesystem::path get_DisassemblyFile() const { return DisassemblyFile; }
        void set_DisassemblyFile(const efxc2Utils::M_STRING_VIEW _DisassemblyFile) { DisassemblyFile = _DisassemblyFile; (void)DisassemblyFile.make_preferred(); }
        std::filesystem::path get_inputFile() const { return inputFile; }
        void set_inputFile(const efxc2Utils::M_STRING_VIEW _inputFile) { inputFile = _inputFile; (void)inputFile.make_preferred(); }
        std::filesystem::path get_privateDataFile() const { return privateDataFile; }
        void set_privateDataFile(const efxc2Utils::M_STRING_VIEW _privateDataFile) { privateDataFile = _privateDataFile; (void)privateDataFile.make_preferred(); }
        std::filesystem::path get_IncludeFile() const { return IncludeFile; }
        void set_IncludeFile(const efxc2Utils::M_STRING_VIEW _IncludeFile) { IncludeFile = _IncludeFile; (void)IncludeFile.make_preferred(); }
        std::filesystem::path get_ObjectFile() const { return ObjectFile; }
        void set_ObjectFile(const efxc2Utils::M_STRING_VIEW _ObjectFile) { ObjectFile = _ObjectFile;  (void)ObjectFile.make_preferred(); }
        std::filesystem::path get_LibraryFile() const { return LibraryFile; }
        void set_LibraryFile(const efxc2Utils::M_STRING_VIEW _LibraryFile) { LibraryFile = _LibraryFile; (void)LibraryFile.make_preferred(); }
        std::filesystem::path get_pdbFile() const { return pdbFile; };
        void set_pdbFile(const efxc2Utils::M_STRING_VIEW _pdbFile) { pdbFile = _pdbFile; (void)pdbFile.make_preferred(); }
        std::filesystem::path get_preprocessFile() const { return preprocessFile; }
        void set_preprocessFile(const efxc2Utils::M_STRING_VIEW _preprocessFile) {
            preprocessFile = _preprocessFile; (void)preprocessFile.make_preferred();
        }
        void LoadInputFile(efxc2CompilerParams::CompilerParams& params) const;
        void LoadPrivateDataFile(efxc2CompilerParams::CompilerParams& params) const;
        void WriteDisassembly(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) const;
        void WriteIncludeFile(efxc2Compiler::Compiler& compile, const efxc2CompilerParams::CompilerParams& params) const;
        void WriteObjectFile(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) const;
        void WritePDBFile(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params);
        void WritePreprocessFile(efxc2Compiler::Compiler& compiler, const efxc2CompilerParams::CompilerParams& params) const;
    private:

        std::filesystem::path DisassemblyFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path inputFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path privateDataFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path IncludeFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path ObjectFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path LibraryFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path pdbFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path preprocessFile = efxc2Utils::M_STRING_INIT;
        std::filesystem::path temp = efxc2Utils::M_STRING_INIT;
    };
}

#endif /*EFXC2FILES_H_INCLUDED*/