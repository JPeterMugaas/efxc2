//--------------------------------------------------------------------------------------
// File: efxc2Compiler.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#pragma once
#include "efxc2.h"

class Compiler {
public:
	explicit Compiler();
	HMODULE get_h() const { return h; }
	pD3DCompile2g get_ptr_D3DCompile2() const { return  ptr_D3DCompile2; }
	pD3DStripShaderg get_ptr_D3DStripShader() const { return ptr_D3DStripShader; }
	pD3DGetBlobPartg get_ptr_D3DGetBlobPart() const { return ptr_D3DGetBlobPart; }
	pD3DSetBlobPartg get_ptr_D3DSetBlobPart() const { return ptr_D3DSetBlobPart; }
	UINT get_sflags() const { return sflags; }
	void set_sflags(UINT _sflags) { sflags = _sflags; }
	UINT get_eflags() const { return eflags; }
	void set_eflags(UINT _eflags) { eflags = _eflags; }
	UINT get_secondary_flags() const { return secondary_flags; }
	void set_secondary_flags(UINT _secondary_flags) { secondary_flags = _secondary_flags; }
	UINT get_strip_flags() const { return strip_flags; }
	void set_strip_flags(UINT _strip_flags) { strip_flags = _strip_flags; }
	char* get_model() const { return model; }
	void set_model(char* _model) { model = _model; }
	char* get_entryPoint() const { return entryPoint; }
	void set_entryPoint(char* _entryPoint) { entryPoint = _entryPoint; }
	char* get_variableName() const { return variableName; }
	void set_variableName(char* _variableName) { variableName = _variableName; }
	char* get_inputFile() const { return inputFile; }
	void set_inputFile(char* _inputFile) { inputFile = _inputFile; }
	int get_verbose() const { return verbose; }
	void set_verbose(int _verbose) { verbose = _verbose; }
	int get_outputHex() const { return outputHex; }
	void set_outputHex(int _outputHex) { outputHex = _outputHex; }
	void Compile(
		_In_ const char* SourceCode,
		_In_ const size_t SourceLen,
		_In_ const size_t numDefines,
		_In_reads_opt_(_Inexpressible_(defines->Name != NULL)) CONST D3D_SHADER_MACRO* defines);
	void StripShader();
	size_t WriteIncludeFile(FILE* f);
	size_t WriteObjectFile(FILE* f);
	char* GetPDBFileName();
	void Compiler::SetPDBFileName(_In_ char* _fileName);
private:
	int              verbose = 1;
	UINT             sflags = 0;
	UINT             eflags = 0;
	UINT             secondary_flags = 0;
	UINT             strip_flags = 0;
	char*            model = nullptr;
	char*            entryPoint = nullptr;
	char*            inputFile = nullptr;
	char*            variableName = nullptr;
	int              outputHex = 0;
	HMODULE          h;
	pD3DCompile2g    ptr_D3DCompile2;
	pD3DStripShaderg ptr_D3DStripShader;
	pD3DGetBlobPartg ptr_D3DGetBlobPart;
	pD3DSetBlobPartg ptr_D3DSetBlobPart;
	ID3DBlob*        compilerOutput = nullptr;
	ID3DBlob*        strippedBlob = nullptr;
	ID3DBlob*        pPDBName = nullptr;
};
