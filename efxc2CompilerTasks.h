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
#ifndef EFXC2COMPILERTASKS_H_INCLUDED
#define EFXC2COMPILERTASKS_H_INCLUDED
#include "efxc2.h"
#include "efxc2Compiler.h"
#include "efxc2CompilerParams.h"

#ifdef _WIN32
void CompilerTasks(Compiler& compiler, Files files, CompilerParams& params);
#else
void CompilerTasks(Compiler& compiler, Files files, CompilerParams& params);
#endif

#endif /* EFXC2COMPILERTASKS_H_INCLUDED */