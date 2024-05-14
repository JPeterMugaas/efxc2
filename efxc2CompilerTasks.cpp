//--------------------------------------------------------------------------------------
// File: efxc2CompilerTasks.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2CompilerTasks.h"
#include "efxc2Utils.h"
#include "efxc2Compiler.h"
#include "efxc2Files.h"
#include "efxc2Exception.h"

#ifdef _WIN32
void efxc2CompilerTasks::CompilerTasks(efxc2Compiler::Compiler& compiler, efxc2Files::Files& files, const efxc2CompilerParams::CompilerParams& params) {
#else
void efxc2CompilerTasks::CompilerTasks(efxc2Compiler::Compiler& compiler, efxc2Files::Files& files, const efxc2CompilerParams::CompilerParams& params) {
#endif
    if ((params.get_commands() & efxc2Utils::CMD_PREPROCESS_FILE) == efxc2Utils::CMD_PREPROCESS_FILE) {
        if (params.get_commands() != efxc2Utils::CMD_PREPROCESS_FILE) {
            std::cerr << "The /P option may only be used with the /D and /I parameters.";
            throw efxc2Exception::InvalidPOption();
        }
        compiler.Preprocess();
        files.WritePreprocessFile(compiler, params);
        return;
    }
    compiler.Compile();
    if (params.get_PrivateData() != nullptr) {
        compiler.EmbedPrivateData();
    }
    compiler.StripShader();
    if ((params.get_commands() & efxc2Utils::CMD_WRITE_HEADER) == efxc2Utils::CMD_WRITE_HEADER) {
        files.WriteIncludeFile(compiler,params);
    }
    if ((params.get_commands() & efxc2Utils::CMD_WRITE_PDB_FILE) == efxc2Utils::CMD_WRITE_PDB_FILE) {
        files.WritePDBFile(compiler, params);
    }
    if ((params.get_commands() & efxc2Utils::CMD_WRITE_OBJECT) == efxc2Utils::CMD_WRITE_OBJECT) {
        files.WriteObjectFile(compiler,params);
    }
    if ((params.get_commands() & efxc2Utils::CMD_WRITE_ASSEMBLY_CODE) == efxc2Utils::CMD_WRITE_ASSEMBLY_CODE) {
        compiler.Disassemble();
        files.WriteDisassembly(compiler,params);
    }
}
