//--------------------------------------------------------------------------------------
// File: efxc2CompilerTasks.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2Utils.h"
#include "efxc2Compiler.h"
#include "efxc2Files.h"

#ifdef _WIN32
void CompilerTasks(Compiler& compiler, Files& files, const CompilerParams& params) {
#else
void CompilerTasks(Compiler& compiler, Files& files, const CompilerParams& params) {
#endif
    if ((params.get_commands() & CMD_PREPROCESS_FILE) == CMD_PREPROCESS_FILE) {
        if (params.get_commands() != CMD_PREPROCESS_FILE) {
            std::cerr << "The /P option may only be used with the /D and /I parameters.";
            exit(1);
        }
        compiler.Preprocess();
        files.WritePreprocessFile(compiler, params);
        exit(0);
    }
    compiler.Compile();
    compiler.StripShader();
    if ((params.get_commands() & CMD_WRITE_HEADER) == CMD_WRITE_HEADER) {
        files.WriteIncludeFile(compiler,params);
    }
    if ((params.get_commands() & CMD_WRITE_PDB_FILE) == CMD_WRITE_PDB_FILE) {
        files.WritePDBFile(compiler, params);
    }
    if ((params.get_commands() & CMD_WRITE_OBJECT) == CMD_WRITE_OBJECT) {
        files.WriteObjectFile(compiler,params);
    }
    if ((params.get_commands() & CMD_WRITE_ASSEMBLY_CODE) == CMD_WRITE_ASSEMBLY_CODE) {
        compiler.Disassemble();
        files.WriteDisassembly(compiler,params);
    }
}
