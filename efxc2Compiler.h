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
#ifndef EFXC2COMPILER_H_INCLUDED
#define EFXC2COMPILER_H_INCLUDED
#include "efxc2.h"
#include "efxc2CompilerAPIContainer.h"
#include "efxc2CompilerParams.h"

namespace efxc2Compiler {
	// This struct represents the first four bytes of the name blob:
	struct ShaderDebugName
	{
		uint16_t Flags;       // Reserved, must be set to zero.
		uint16_t NameLength;  // Length of the debug name, without null terminator.
		// Followed by NameLength bytes of the UTF-8-encoded name.
		// Followed by a null terminator.
		// Followed by [0-3] zero bytes to align to a 4-byte boundary.
	};
	class Compiler {
	public:
		explicit Compiler(const efxc2CompilerAPIContainer::CompilerAPIContainer& _api, const efxc2CompilerParams::CompilerParams& _params) : api(_api), params(_params) { };
		void Preprocess();
		void Compile();
		void Disassemble();
		void StripShader();
		void EmbedPrivateData();
		size_t WriteIncludeFile(std::ofstream& f);
		size_t WriteObjectFile(std::ofstream& f);
		size_t WriteAssemblyCode(std::ofstream& f);
		size_t WritePreprocessFile(std::ofstream& f);
		std::string GetPDBFileName();
		void SetPDBFileName(_In_ const std::string_view _fileName);
		size_t WritePDBFile(std::ofstream& f);
	private:
		efxc2CompilerAPIContainer::CompilerAPIContainer api;
		efxc2CompilerParams::CompilerParams       params;
		/*mark these as will not serialize*/
		ID3DBlob* compilerOutput = nullptr;  //-V122_NOPTR
		ID3DBlob* disassemblyCodeBlob = nullptr;  //-V122_NOPTR
		ID3DBlob* strippedBlob = nullptr;  //-V122_NOPTR
		ID3DBlob* pPDBName = nullptr;  //-V122_NOPTR
		ID3DBlob* pPreprocessOutput = nullptr;  //-V122_NOPTR
		ID3D11Module* pLibraryModule = nullptr;  //-V122_NOPTR
		ID3DBlob* LinkedBlob = nullptr;  //-V122_NOPTR
	};
	void print_D3D_SHADER_MACRO(D3D_SHADER_MACRO i);
	void print_defines(std::vector<D3D_SHADER_MACRO> const& defines);
	void print_source_code_sample(efxc2Utils::M_BUFFER const& SourceCode);
	void SetupDefines(efxc2Utils::M_COMPILER_DEFINES _defines, std::vector<D3D_SHADER_MACRO>& defines);
	void print_compiler_error(std::string op, ID3DBlob* errors, HRESULT hr);

	void print_compiler_params_begin(efxc2Utils::M_BUFFER SourceCode,
		size_t SourceLen,
		const char* inputFile,
		std::vector<D3D_SHADER_MACRO> const& defines);
	void print_compiler_params_end();
}

#endif /* EFXC2COMPILER_H_INCLUDED */