//--------------------------------------------------------------------------------------
// File: efxc2CompilerTasks.h
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
#include "efxc2Files.h"

namespace efxc2CompilerTasks {
#ifdef _WIN32
	void CompilerTasks(efxc2Compiler::Compiler& compiler, efxc2Files::Files& files, const efxc2CompilerParams::CompilerParams& params);
#else
	void CompilerTasks(efxc2Compiler::Compiler& compiler, efxc2Files::Files& files, const efxc2CompilerParams::CompilerParams& params);
#endif
}

#endif /* EFXC2COMPILERTASKS_H_INCLUDED */