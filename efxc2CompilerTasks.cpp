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
void CompilerTasks(Compiler& compiler, Files files) {
#else
void CompilerTasks(Compiler & compiler, Files files) {
#endif
    compiler.Compile();
    files.WritePDBFile(compiler);
    compiler.StripShader();
    if ((compiler.get_commands() & CMD_WRITE_HEADER) == CMD_WRITE_HEADER) {
        files.WriteIncludeFile(compiler);
    }
    if ((compiler.get_commands() & CMD_WRITE_OBJECT) == CMD_WRITE_OBJECT) {
        files.WriteObjectFile(compiler);
    }
}
