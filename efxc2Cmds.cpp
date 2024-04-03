//--------------------------------------------------------------------------------------
// File: efxc2Cmds.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2Cmds.h"
#include "efxc2Utils.h"
#include "efxc2Files.h"

#ifdef _WIN32
void option_ignored(_In_ const wchar_t* Opt, CompilerParams& params) {
	if (params.get_verbose()) {
		wprintf(L"Option %ls ignored", Opt);
	}
	return;
}
#else
void option_ignored(_In_ const char* Opt, _In_ CompilerParams& params) {
	if (params.get_verbose()) {
		wprintf(L"Option %s ignored", Opt);
	}
	return;
}
#endif

#ifdef _WIN32
void parseInputFile(_In_ const wchar_t* parameter, CompilerParams& params, Files& files) {
	std::wstring inputFile = L"";
	std::string c_inputFile = "";
#else
void parseInputFile(_In_ const char* parameter, CompilerParams& params, Files & files) {
	std::string inputFile = "";
#endif
	FILE* f = nullptr;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif
	if (files.get_inputFile().empty()) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
		if (params.get_verbose()) {
			printf("Parse input file name\n");
		}
		inputFile = parameter;
#ifdef _WIN32
		FixupFileName(inputFile);
		c_inputFile = utf8_encode(inputFile);
		params.set_inputFile(c_inputFile);
#else  /* _WIN32 */
		params.set_inputFile(inputFile);
#endif /* _WIN32 */
		files.set_inputFile(inputFile);
		if (params.get_verbose()) {
#ifdef _WIN32
			wprintf(L"input file: %ls\n", inputFile.c_str());
#else  /* _WIN32 */
			printf("input file: %s\n", inputFile.c_str());
#endif /* _WIN32 */
		}
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif  /* _MSC_VER */
		errno_t err = _wfopen_s(&f, inputFile.c_str(), L"r");
#ifdef _MSC_VER
#pragma warning( pop )
#endif  /* _MSC_VER */
		if (err != 0) {
			print_errno(err);
#else
		FILE* f = fopen(inputFile.c_str(), "r");
		if (f == nullptr) {
			print_errno();
#endif  /* _WIN32 */
		}
		params.LoadSourceCode(f);
		_Analysis_assume_(f != NULL);
		fclose(f);
	}
	else {
		print_usage_toomany();
	}
}

void cmd_all_resources_bound(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -all_resources_bound sflags | D3DCOMPILE_ALL_RESOURCES_BOUND\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
	params.set_sflags(sflags);
	return;
}

void cmd_Cc(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Cc | D3D_DISASM_ENABLE_COLOR_CODED\n");
	}
	UINT disassembly_flags = params.get_disassembly_flags();
	disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_COLOR_CODE;
	params.set_disassembly_flags(disassembly_flags);
	return;
}

#ifdef _WIN32
void cmd_D(CompilerParams& params,
	_In_ const wchar_t* _defineOption) {
	char* defineOption = utf8_encode(_defineOption);
#else
void cmd_D(CompilerParams& params,
	_In_ char* defineOption) {
#endif
	params.add_define(defineOption);
	if (params.get_verbose()) {
		printf("option -D with arg %s\n", defineOption);
	}
}

#ifdef _WIN32
void cmd_E(CompilerParams& params, _In_ const std::wstring& w_entryPoint) {
	std::string entryPoint = utf8_encode(w_entryPoint);
#else
void cmd_E(CompilerParams& params, _In_ const std::string entryPoint) {
#endif
	params.set_entryPoint(entryPoint);
	if (params.get_verbose()) {
		printf("option -E (Entry Point) with arg '%s'\n", entryPoint.c_str());
	}
	return;
}

void cmd_enable_unbounded_descriptor_tables(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -enable_unbounded_descriptor_tables sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
	params.set_sflags(sflags);
	return;
}

#ifdef _WIN32
void cmd_Fc(CompilerParams& params, Files& files, _In_ const std::wstring& assemblyCodeFile) {
#else
void cmd_Fc( CompilerParams& params, Files & files, _In_ const std::string& assemblyCodeFile) {
#endif
	files.set_DisassemblyFile(assemblyCodeFile);
	UINT cmd = params.get_commands();
	cmd = cmd | CMD_WRITE_ASSEMBLY_CODE;
	params.set_commands(cmd);
	if (params.get_verbose()) {
		printf("option -Fc (assembly code) with arg %ls\n", assemblyCodeFile.c_str());
	}
	return;
}

#ifdef _WIN32
void cmd_Fd(CompilerParams& params, Files & files, _In_ const std::wstring& pdbFile) {
#else
void cmd_Fd(CompilerParams& params, Files & files, _In_ const std::string& pdbFile) {
#endif
#ifdef _WIN32
	files.set_c_pdbFile(utf8_encode(pdbFile));
#endif /* _WIN32 */
	files.set_pdbFile(pdbFile);

	UINT cmd = params.get_commands();
	cmd = cmd | CMD_WRITE_PDB_FILE;
	params.set_commands(cmd);

	if (params.get_verbose()) {
		printf("option -Fd (.PDB) with arg %ls\n", pdbFile.c_str());
	}
	return;
}
#ifdef _WIN32
void cmd_Fh(CompilerParams& params, Files & files, _In_ const std::wstring& outputFile) {
#else
void cmd_Fh(CompilerParams& params, Files & files, _In_ const std::string& outputFile) {
#endif
	if (params.get_verbose()) {
		printf("option -Fh (Output File) with arg %ls\n", outputFile.c_str());
	}
	files.set_IncludeFile(outputFile);
	UINT cmd = params.get_commands();
	cmd = cmd | CMD_WRITE_HEADER;
	params.set_commands(cmd);
	return;
}

#ifdef _WIN32
void cmd_Fo(CompilerParams& params, Files & files, _In_ const std::wstring& outputFile) {
#else
void cmd_Fo(CompilerParams& params,  Files & files, _In_ const std::string& outputFile) {
#endif
	files.set_ObjectFile(outputFile);
	UINT cmd = params.get_commands();
	cmd = cmd | CMD_WRITE_OBJECT;
	if (params.get_verbose()) {
		printf("option -FO (Output File) with arg %ls\n", outputFile.c_str());
	}
	params.set_commands(cmd);
	return;
}

void cmd_Gch(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Gch eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT");
	}
	UINT eflags = params.get_eflags();
	eflags = eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
	params.set_eflags(eflags);
	return;
}

void cmd_Gdp(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Gdp eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS");
	}
	UINT eflags = params.get_eflags();
	eflags = eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
	params.set_eflags(eflags);
	return;
}

void cmd_Gec(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Gec sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
	params.set_sflags(sflags);
	return;
}

void cmd_Ges(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Ges sflags | D3DCOMPILE_ENABLE_STRICTNESS\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_ENABLE_STRICTNESS;
	params.set_sflags(sflags);
	return;
}

void cmd_Gfa(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Gfa sflags | D3DCOMPILE_AVOID_FLOW_CONTROL\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
	params.set_sflags(sflags);
	return;
}

void cmd_Gis(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Gis sflags | D3DCOMPILE_IEEE_STRICTNESS\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_IEEE_STRICTNESS;
	params.set_sflags(sflags);
	return;
}

void cmd_Gpp(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Gpp sflags | D3DCOMPILE_PARTIAL_PRECISION\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_PARTIAL_PRECISION;
	params.set_sflags(sflags);
	return;
}

void cmd_Lx(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Lx - output hexidecimal literals\n");
	}
	params.set_outputHex(TRUE);
	return;
}

void cmd_Ni(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Cc | D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING\n");
	}
	UINT disassembly_flags = params.get_disassembly_flags();
	disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING;
	params.set_disassembly_flags(disassembly_flags);
	return;
}

void cmd_No(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Cc | D3D_DISASM_ENABLE_INSTRUCTION_OFFSET\n");
	}
	UINT disassembly_flags = params.get_disassembly_flags();
	disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_OFFSET;
	params.set_disassembly_flags(disassembly_flags);
	return;
}

void cmd_O0(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -O0 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
	params.set_sflags(sflags);
	return;
}

void cmd_O1(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
	params.set_sflags(sflags);
	return;
}

void cmd_O2(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
	params.set_sflags(sflags);
	return;
}

void cmd_O3(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
	params.set_sflags(sflags);
	return;
}

void cmd_Od(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Od sflags | D3DCOMPILE_SKIP_OPTIMIZATION\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_SKIP_OPTIMIZATION;
	params.set_sflags(sflags);
	return;
}

void cmd_Op(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Op sflags | D3DCOMPILE_NO_PRESHADER\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_NO_PRESHADER;
	params.set_sflags(sflags);
	return;
}

void cmd_Qstrip_debug(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Qstrip_debug strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO\n");
	}
	UINT strip_flags = params.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO;
	params.set_strip_flags(strip_flags);
	return;
}

void cmd_Qstrip_priv(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Qstrip_priv strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA\n");
	}
	UINT strip_flags = params.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA;
	params.set_strip_flags(strip_flags);
	return;
}

void cmd_Qstrip_reflect(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Qstrip_reflect strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA\n");
	}
	UINT strip_flags = params.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA;
	params.set_strip_flags(strip_flags);
	return;
}

void cmd_Qstrip_rootsignature(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Qstrip_rootsignature strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATUR\n");
	}
	UINT strip_flags = params.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE;
	params.set_strip_flags(strip_flags);
	return;
}

void cmd_res_may_alias(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -res_may_alias sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
	params.set_sflags(sflags);
	return;
}

#ifdef _WIN32
void cmd_T(CompilerParams& params, _In_ const std::wstring& w_model) {
	std::string model = utf8_encode(w_model);
#else
void cmd_T(CompilerParams & params, _In_ const std::string & model) {
#endif
	if (params.get_verbose()) {
		printf("option -T (Shader Model/Profile) with arg '%s'\n", model.c_str());
	}
	params.set_model(model);
	return;
}

void cmd_Vd(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Vd sflags | D3DCOMPILE_SKIP_VALIDATION\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_SKIP_VALIDATION;
	params.set_sflags(sflags);
	return;
}

#ifdef _WIN32
void cmd_Vn(CompilerParams& params, _In_ const std::wstring& w_variableName) {
	std::string variableName = utf8_encode(w_variableName);
#else
void cmd_Vn(CompilerParams& params, _In_ const std::string& variableName) {
#endif
	if (params.get_verbose()) {
		printf("option -Vn (Variable Name) with arg '%s'\n", variableName.c_str());
	}
	params.set_variableName(variableName);
	return;
}

void cmd_WX(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -WX sflags |  D3DCOMPILE_WARNINGS_ARE_ERRORS\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
	params.set_sflags(sflags);
	return;
}

void cmd_Zi(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Zi sflags | D3DCOMPILE_DEBUG\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_DEBUG;
	params.set_sflags(sflags);
	return;
}

void cmd_Zpc(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Zpc sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
	params.set_sflags(sflags);
	return;
}

void cmd_Zpr(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Zpr sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	params.set_sflags(sflags);
	return;
}

void cmd_Zsb(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Zsb sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
	params.set_sflags(sflags);
	return;
}

void cmd_Zss(CompilerParams& params) {
	if (params.get_verbose()) {
		printf("option -Zss sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n");
	}
	UINT sflags = params.get_sflags();
	sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
	params.set_sflags(sflags);
	return;
}

#ifdef _WIN32
bool  parseCompilerOnlyCall(
	_In_ wchar_t* argv[1],
	_Inout_	int* index,
	CompilerParams& params) {
#else
bool  parseCompilerOnlyCall(
	_In_ char* argv[1],
	_Inout_	int* index,
	CompilerParams& params) {
	char** argumentOption = nullptr;
#endif
#ifdef _WIN32
	const wchar_t* argument = argv[*index];
#else  /* _WIN32 */
	const char* argument = argv[*index];
#endif /* _WIN32 */
	if (argument[0] == '-' || argument[0] == '/') {
		argument++;
		if (argument[0] == '-') {
			argument++;
		}
	}
	else {
		return false;
	}

	for (int i = 0; i < COMPILER_ONLY_ENTRIES_LENGTH; i++) {
#ifdef _WIN32
		if (wcscmp(g_CompilerOnlyCall[i].Param, argument) == 0) {
#else
		if (strcmp(g_CompilerOnlyCall[i].Param, argument) == 0) {
#endif
			auto ptr = (gCompilerp*)g_CompilerOnlyCall[i].method;
			ptr(params);
			* index += 1;
			return true;
		}
	}
	return false;
}

#ifdef _WIN32
bool parseCompilerFileCall(
	_In_ int argc,
	_In_ wchar_t* argv[1],
	_Inout_	int* index,
	CompilerParams& params,
	Files& files) {
	wchar_t* argumentOption = nullptr;
#else
bool parseCompilerFileCall(
	_In_ int argc,
	_In_ char* argv[1],
	_Inout_	int* index,
	CompilerParams& params,
	Files & files) {
	char* argumentOption = nullptr;
#endif
	size_t optionSize = 0;

	if (!index || *index >= argc) {
		return false;
	}
#ifdef _WIN32
	const wchar_t* argument = argv[*index];
#else  /* _WIN32 */
	const char* argument = argv[*index];
#endif /* _WIN32 */
	if (argument[0] == '-' || argument[0] == '/') {
		argument++;
		if (argument[0] == '-') {
			argument++;
		}
	}
	else {
		return false;
	}
	for (int i = 0; i < COMPILER_FILE_ENTRIES_LENGTH; i++) {
#ifdef _WIN32
		optionSize = wcslen( g_CompilerFileCall[i].Param);
		if (wcsncmp(argument, g_CompilerFileCall[i].Param, optionSize) != 0) {
#else
		optionSize = strlen( g_CompilerFileCall[i].Param);
		if (strncmp(argument, g_CompilerFileCall[i].Param, optionSize) != 0) {
#endif
			continue;
		}
		argument += optionSize;
		if (*argument == '\0') {
			*index += 1;
			if (*index >= argc) {
				fprintf(stderr, "Error: missing required argument for option %ls\n", g_CompilerFileCall[i].Param);
				return false;
			}
#ifdef _WIN32
			argumentOption = M_WCSDUP(argv[*index]);
#else  /*_WIN32 */
			argumentOption = strdup(argv[*index]);
#endif /* _WIN32 */
		}
		else {
#ifdef _WIN32
			argumentOption = M_WCSDUP(argument);
#else  /* _WIN32 */
			argumentOption = strdup(argument);
#endif /* _WIN32 */
		}
		auto ptr = (gCompilerFilep*)g_CompilerFileCall[i].method;
		ptr(params, files, argumentOption);
		*index += 1;
		return true;
	}
	return false;
}


#ifdef _WIN32
bool parseIgnoredOpts(
	_In_ wchar_t* argv[1],
	_Inout_	int* index,
	CompilerParams& params) {
#else
bool parseIgnoredOpts(
	_In_ char* argv[1],
	_Inout_	int* index,
	CompilerParams& params) {
#endif
#ifdef _WIN32
	const wchar_t* argument = argv[*index];
#else  /* _WIN32 */
	const char* argument = argv[*index];
#endif /* _WIN32 */
	if (argument[0] == '-' || argument[0] == '/') {
		argument++;
		if (argument[0] == '-') {
			argument++;
		}
	}
	else {
		return false;
	}
	for (int i = 0; i < IGNORED_OPTS_LENGTH; i++) {
#ifdef _WIN32
		if (wcscmp(g_IgnoredOpts[i], argument) == 0) {
#else
		if (strcmp(g_IgnoredOpts[i], argument) == 0) {
#endif
			option_ignored(argument, params);
			return true;
		}
	}
	return false;
}

#ifdef _WIN32
bool parseNotSupportedOpts(
	_In_ wchar_t* argv[1],
	_In_ const int* index) {
#else
bool parseNotSupportedOpts(
	_In_ char* argv[1],
	_In_ const int* index) {
#endif
	#ifdef _WIN32
	const wchar_t* argument = argv[*index];
#else  /* _WIN32 */
	const char* argument = argv[*index];
#endif /* _WIN32 */
	if (argument[0] == '-' || argument[0] == '/') {
		argument++;
		if (argument[0] == '-') {
			argument++;
		}
	}
	else {
		return false;
	}
	for (int i = 0; i < NOT_SUPPORTED_LENGTH; i++) {
#ifdef _WIN32
		if (wcscmp(g_NotSupportedArgs[i], argument) == 0) {
			fwprintf(stderr, L"option -%ls not supported", argument);
#else
		if (strcmp(g_NotSupportedArgs[i], argument) == 0) {
			fprintf(stderr, "option -%s not supported", argument);
#endif
			print_unsupported_arg_help();
			return true;
		}
	}
	return false;
}
