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
#include "efxc2CompilerIncludes.h"

namespace efxc2CompilerParams {
	class CompilerParams {
	public:
		explicit CompilerParams() { initializeDefines(); };
		/* application flags and params */
		bool get_verbose() const { return verbose; }
		void set_verbose(bool _verbose) { verbose = _verbose; includeDirs.set_verbose(_verbose); }
		bool get_debug() const { return debug; }
		void set_debug(bool _debug) { debug = _debug; includeDirs.set_debug(_debug); }
		bool get_outputHex() const { return outputHex; }
		void set_outputHex(bool _outputHex) { outputHex = _outputHex; }
		UINT get_commands() const { return commands; }
		void set_commands(UINT _commands) { commands = _commands; }
		/* D3DComppile2 parameters */
		UINT get_sflags() const { return sflags; }
		void set_sflags(UINT _sflags) { sflags = _sflags; }
		UINT get_eflags() const { return eflags; }
		void set_eflags(UINT _eflags) { eflags = _eflags; }
		UINT get_secondary_flags() const { return secondary_flags; }
		void set_secondary_flags(UINT _secondary_flags) { secondary_flags = _secondary_flags; }
		std::string get_model() const { return model; }
		void set_model(const std::string_view _model) { model = _model; }
		std::string get_entryPoint() const { return entryPoint; }
		void set_entryPoint(const std::string_view _entryPoint) { entryPoint = _entryPoint; }
		std::string get_variableName() const { return variableName; }
		void set_variableName(const std::string_view _variableName) { variableName = _variableName; }
		std::string get_inputFile() const { return inputFile; }
		void set_inputFile(const std::string_view _inputFile) { inputFile = _inputFile; }

		efxc2Utils::M_COMPILER_DEFINES get_defines() const { return defines; }
		void add_define(const std::string_view defineOption);
		void LoadSourceCode(std::ifstream& f);
		void LoadPrivateData(std::ifstream& f);
		efxc2Utils::M_BUFFER get_SourceCode() const { return SourceCode; }
		efxc2Utils::M_BUFFER get_PrivateData() const { return SourceCode; }
		/* D3DStripShader parameters */
		UINT get_strip_flags() const { return strip_flags; }
		void set_strip_flags(UINT _strip_flags) { strip_flags = _strip_flags; }
		/* D3DDisassemble parameters */
		UINT get_disassembly_flags() const { return disassembly_flags; }
		void set_disassembly_flags(UINT _disassembly_flags) { disassembly_flags = _disassembly_flags; }
		efxc2CompilerIncludes::CompilerIncludes* get_includeDirs() { return &includeDirs; }
	private:
		/* application flags */
		bool                verbose = true;
		bool                debug = false;
		bool                outputHex = false;
		UINT               commands = 0;
		/* D3DCompile2 parameters */
		UINT               sflags = 0;
		UINT               eflags = 0;
		UINT               secondary_flags = 0;
		efxc2Utils::M_COMPILER_DEFINES defines;
		std::string        model = "";
		std::string        entryPoint = "main";
		std::string        inputFile = "";
		std::string        variableName = "";
		efxc2Utils::M_BUFFER           SourceCode = nullptr;
		efxc2Utils::M_BUFFER           PrivateData = nullptr;
		/* D3DStripShader parameters */
		UINT               strip_flags = 0;
		/* D3DDisassemble parameters */
		UINT               disassembly_flags = 0;
		efxc2CompilerIncludes::CompilerIncludes   includeDirs;

		void initializeDefines();
	};
}

#endif /* EFXC2CCOMPILERPARAMS */
