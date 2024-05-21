//--------------------------------------------------------------------------------------
// File: efxc2Exception.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#pragma once
#include "efxc2.h"

namespace efxc2Exception {
	class ProgramExceptionBase : public std::exception {};
	class Win32APIBase : public ProgramExceptionBase {};
	class Win32APIFailure : public Win32APIBase {};
	class Win32HLSLFailure : public Win32APIBase {};
	class FileBaseError : public ProgramExceptionBase {};
	class FileOpenReadError : public ProgramExceptionBase {};
	class FileOpenWriteError : public ProgramExceptionBase {};
	class FileReadError : public ProgramExceptionBase {};
	class FileWritedError : public ProgramExceptionBase {};
	class InvalidPOption : public ProgramExceptionBase {};
	class TooManyParameters : public ProgramExceptionBase {	};
	class MissingArgument : public ProgramExceptionBase {};
	class NoInputFile : public ProgramExceptionBase {};
	class CompileError : public ProgramExceptionBase {};
	class PreprocessError : public ProgramExceptionBase {};
}