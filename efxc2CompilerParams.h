//--------------------------------------------------------------------------------------
// File: efxc2CompilerParams.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef EFXC2CCOMPILERPARAMS_H_INCLUDED
#define EFXC2CCOMPILERPARAMS_H_INCLUDED
#include "efxc2.h"
#include "efxc2Utils.h"

struct CompilerDefine {
  std::string Name;
  std::string Definition;
};

using M_COMPILER_DEFINES = std::shared_ptr<std::vector<CompilerDefine>>;

class CompilerParams{
public:
	explicit CompilerParams() { initializeDefines(); };
	/* application flags and params */
	int get_verbose() const { return verbose; }
	void set_verbose(int _verbose) { verbose = _verbose; }
	int get_outputHex() const { return outputHex; }
	void set_outputHex(int _outputHex) { outputHex = _outputHex; }
	UINT get_commands() const { return commands; }
	void set_commands(UINT _commands) { commands = _commands;  }
	/* D3DComppile2 parameters */
	UINT get_sflags() const { return sflags; }
	void set_sflags(UINT _sflags) { sflags = _sflags; }
	UINT get_eflags() const { return eflags; }
	void set_eflags(UINT _eflags) { eflags = _eflags; }
	UINT get_secondary_flags() const { return secondary_flags; }
	void set_secondary_flags(UINT _secondary_flags) { secondary_flags = _secondary_flags; }
	std::string get_model() const { return model; }
	void set_model(const std::string& _model) { model = _model; }
	std::string get_entryPoint() const { return entryPoint; }
	void set_entryPoint(const std::string& _entryPoint) { entryPoint = _entryPoint;  }
	std::string get_variableName() const { return variableName; }
	void set_variableName(const std::string& _variableName) { variableName = _variableName; }
	std::string get_inputFile() const { return inputFile; }
	void set_inputFile(const std::string& _inputFile) { inputFile = _inputFile; }
	
	M_COMPILER_DEFINES get_defines() const { return defines; }
	size_t get_numDefines() const { return numDefines; }
	void add_define(const std::string& defineOption);
	void LoadSourceCode(FILE* f);
	M_BUFFER get_SourceCode() const { return SourceCode; }
	size_t get_SourceLen() const { return SourceLen; }
	/* D3DStripShader parameters */
	UINT get_strip_flags() const { return strip_flags; }
	void set_strip_flags(UINT _strip_flags) { strip_flags = _strip_flags; }
	/* D3DDisassemble parameters */
	UINT get_disassembly_flags() const { return disassembly_flags; 	}
	void set_disassembly_flags(UINT _disassembly_flags) { disassembly_flags = _disassembly_flags;  }
private:
	/* application flags */
	int                verbose = 1;
	int                outputHex = 0;
	UINT               commands = 0;
	/* D3DCompile2 parameters */
	UINT               sflags = 0;
	UINT               eflags = 0;
	UINT               secondary_flags = 0;
	size_t             numDefines = 1;
	M_COMPILER_DEFINES defines;
	std::string        model = "";
	std::string        entryPoint = "";
	std::string        inputFile = "";
	std::string        variableName = "";
	size_t             SourceLen = 0;
	M_BUFFER           SourceCode = nullptr;
	/* D3DStripShader parameters */
	UINT               strip_flags = 0;
	/* D3DDisassemble parameters */
	UINT               disassembly_flags = 0;

	void initializeDefines();
};


#endif /* EFXC2CCOMPILERPARAMS */
