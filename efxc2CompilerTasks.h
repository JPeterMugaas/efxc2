//--------------------------------------------------------------------------------------
// File: efxc2CompilerTasks.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#pragma once
#include "efxc2.h"
#include "efxc2Compiler.h"

#ifdef _WIN32
void CompilerTasks(wchar_t* IncludeFile,
    wchar_t* ObjectFile,
    wchar_t* pdbFile,
    char* c_pdbFile,
    Compiler& compiler);
#else
void CompilerTasks(char* IncludeFile,
    char* ObjectFile,
    char* pdbFile,
    Compiler & compiler);
#endif