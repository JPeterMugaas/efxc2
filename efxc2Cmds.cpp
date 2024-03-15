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
void option_ignored(_In_ const wchar_t* Opt, Compiler& compiler) {
	if (compiler.get_verbose()) {
		wprintf(L"Option %ls ignored", Opt);
	}
	return;
}
#else
void option_ignored(_In_ const char* Opt, _In_ Compiler& compiler) {
	if (compiler.get_verbose()) {
		wprintf(L"Option %s ignored", Opt);
	}
	return;
}
#endif

#ifdef _WIN32
void parseInputFile(_In_ const wchar_t* parameter, Compiler& compiler, Files& files) {
	wchar_t* inputFile = nullptr;
	char* c_inputFile = nullptr;
#else
void parseInputFile(_In_ const char* parameter, Compiler & compiler, Files & files) {
	char* inputFile = nullptr;
#endif
	FILE* f = nullptr;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif
	if (files.get_inputFile() == nullptr) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
		if (compiler.get_verbose()) {
			printf("Parse input file name\n");
		}
#ifdef _WIN32
		inputFile = new wchar_t[wcslen(parameter) + 1];
		wcscpy_s(inputFile, wcslen(parameter) + 1, parameter);
		FixupFileName(inputFile);
		c_inputFile = wcharToChar(inputFile);
		compiler.set_inputFile(c_inputFile);
#else  /* _WIN32 */
		inputFile = new char[strlen(parameter) + 1];
		strncpy(inputFile, parameter, strlen(parameter) + 1);
		compiler.set_inputFile(inputFile);
#endif /* _WIN32 */
		files.set_inputFile(inputFile);
		if (compiler.get_verbose()) {
#ifdef _WIN32
			wprintf(L"input file: %ls\n", inputFile);
#else  /* _WIN32 */
			printf("input file: %ls\n", inputFile);
#endif /* _WIN32 */
		}
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif  /* _MSC_VER */
		errno_t err = _wfopen_s(&f, inputFile, L"r");
#ifdef _MSC_VER
#pragma warning( pop )
#endif  /* _MSC_VER */
		if (err != 0) {
			print_errno(err);
#else
		FILE* f = fopen(inputFile, "r");
		if (f == nullptr) {
			print_errno();
#endif  /* _WIN32 */
		}
		compiler.LoadSourceCode(f);
		_Analysis_assume_(f != NULL);
		fclose(f);
	}
	else {
		print_usage_toomany();
	}
}

void cmd_all_resources_bound(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -all_resources_bound sflags | D3DCOMPILE_ALL_RESOURCES_BOUND\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
	compiler.set_sflags(sflags);
	return;
}

#ifdef _WIN32
void cmd_D(Compiler & compiler,
	_In_ const wchar_t* _defineOption) {
	char* defineOption = wcharToChar(_defineOption);
#else
void cmd_D(Compiler& compiler,
	_In_ char* defineOption) {
#endif
	compiler.add_define(defineOption);
	if (compiler.get_verbose()) {
		printf("option -D with arg %s\n", defineOption);
	}
}

#ifdef _WIN32
void cmd_E(Compiler & compiler, _In_ const wchar_t* w_entryPoint) {
	char* entryPoint = wcharToChar(w_entryPoint);
#else
void cmd_E(Compiler & compiler, _In_ char* entryPoint) {
#endif
	compiler.set_entryPoint(entryPoint);
	if (compiler.get_verbose()) {
		printf("option -E (Entry Point) with arg '%s'\n", entryPoint);
	}
	return;
}

void cmd_enable_unbounded_descriptor_tables(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -enable_unbounded_descriptor_tables sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
	compiler.set_sflags(sflags);
	return;
}

#ifdef _WIN32
void cmd_Fd(Compiler & compiler, Files & files, _In_ wchar_t* pdbFile) {
#else
void cmd_Fd(Compiler & compiler, Files & files, _In_ char* pdbFile) {
#endif
#ifdef _WIN32
	FixupFileName(pdbFile);
	files.set_c_pdbFile(utf8_encode(pdbFile, wcslen(pdbFile)));
#endif /* _WIN32 */
	files.set_pdbFile(pdbFile);
	if (compiler.get_verbose()) {
		printf("option -Fd (.PDB) with arg %ls\n", pdbFile);
	}
	return;
}
#ifdef _WIN32
void cmd_Fh(Compiler & compiler, Files & files, _Inout_ wchar_t* outputFile) {
#else
void cmd_Fh(Compiler & compiler, Files & files, _Inout_ char* outputFile) {
#endif
	if (compiler.get_verbose()) {
		printf("option -Fh (Output File) with arg %ls\n", outputFile);
	}
#ifdef _WIN32
	FixupFileName(outputFile);
#endif /* _WIN32 */
	files.set_IncludeFile(outputFile);
	UINT cmd = compiler.get_commands();
	cmd = cmd | CMD_WRITE_HEADER;
	compiler.set_commands(cmd);
	return;
}

#ifdef _WIN32
void cmd_Fo(Compiler & compiler, Files & files, _Inout_ wchar_t* outputFile) {
#else
void cmd_Fo(Compiler & compiler,  Files & files, _Inout_ char* outputFile) {
#endif
#ifdef _WIN32
	FixupFileName(outputFile);
#endif /* _WIN32 */
	files.set_ObjectFile(outputFile);
	UINT cmd = compiler.get_commands();
	cmd = cmd | CMD_WRITE_OBJECT;
	if (compiler.get_verbose()) {
		printf("option -FO (Output File) with arg %ls\n", outputFile);
	}
	compiler.set_commands(cmd);
	return;
}

void cmd_Gch(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Gch eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT");
	}
	UINT eflags = compiler.get_eflags();
	eflags = eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
	compiler.set_eflags(eflags);
	return;
}

void cmd_Gdp(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Gdp eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS");
	}
	UINT eflags = compiler.get_eflags();
	eflags = eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
	compiler.set_eflags(eflags);
	return;
}

void cmd_Gec(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Gec sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Ges(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Ges sflags | D3DCOMPILE_ENABLE_STRICTNESS\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_ENABLE_STRICTNESS;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Gfa(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Gfa sflags | D3DCOMPILE_AVOID_FLOW_CONTROL\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Gis(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Gis sflags | D3DCOMPILE_IEEE_STRICTNESS\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_IEEE_STRICTNESS;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Gpp(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Gpp sflags | D3DCOMPILE_PARTIAL_PRECISION\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_PARTIAL_PRECISION;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Lx(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Lx - output hexidecimal literals\n");
	}
	compiler.set_outputHex(TRUE);
	return;
}

void cmd_O0(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -O0 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
	compiler.set_sflags(sflags);
	return;
}

void cmd_O1(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
	compiler.set_sflags(sflags);
	return;
}

void cmd_O2(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
	compiler.set_sflags(sflags);
	return;
}

void cmd_O3(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Od(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Od sflags | D3DCOMPILE_SKIP_OPTIMIZATION\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_SKIP_OPTIMIZATION;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Op(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Op sflags | D3DCOMPILE_NO_PRESHADER\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_NO_PRESHADER;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Qstrip_debug(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Qstrip_debug strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO\n");
	}
	UINT strip_flags = compiler.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO;
	compiler.set_strip_flags(strip_flags);
	return;
}

void cmd_Qstrip_priv(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Qstrip_priv strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA\n");
	}
	UINT strip_flags = compiler.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA;
	compiler.set_strip_flags(strip_flags);
	return;
}

void cmd_Qstrip_reflect(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Qstrip_reflect strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA\n");
	}
	UINT strip_flags = compiler.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA;
	compiler.set_strip_flags(strip_flags);
	return;
}

void cmd_Qstrip_rootsignature(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Qstrip_rootsignature strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE\n");
	}
	UINT strip_flags = compiler.get_strip_flags();
	strip_flags = strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE;
	compiler.set_strip_flags(strip_flags);
	return;
}

void cmd_res_may_alias(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -res_may_alias sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
	compiler.set_sflags(sflags);
	return;
}

#ifdef _WIN32
void cmd_T(Compiler & compiler, _In_ wchar_t* w_model) {
	char* model = wcharToChar(w_model);
#else
void cmd_T(Compiler & compiler, _In_ char* model) {
#endif
	if (compiler.get_verbose()) {
		printf("option -T (Shader Model/Profile) with arg '%s'\n", model);
	}
	compiler.set_model(model);
	return;
}

void cmd_Vd(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Vd sflags | D3DCOMPILE_SKIP_VALIDATION\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_SKIP_VALIDATION;
	compiler.set_sflags(sflags);
	return;
}

#ifdef _WIN32
void cmd_Vn(Compiler & compiler, _In_ wchar_t* w_variableName) {
	char* variableName = wcharToChar(w_variableName);
#else
void cmd_Vn(Compiler & compiler, _In_ char* variableName) {
#endif
	if (compiler.get_verbose()) {
		printf("option -Vn (Variable Name) with arg '%s'\n", variableName);
	}
	compiler.set_variableName(variableName);
	return;
}

void cmd_WX(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -WX sflags |  D3DCOMPILE_WARNINGS_ARE_ERRORS\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Zi(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Zi sflags | D3DCOMPILE_DEBUG\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_DEBUG;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Zpc(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Zpc flags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Zpr(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Zpr sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Zsb(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Zsb sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
	compiler.set_sflags(sflags);
	return;
}

void cmd_Zss(Compiler & compiler) {
	if (compiler.get_verbose()) {
		printf("option -Zss sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n");
	}
	UINT sflags = compiler.get_sflags();
	sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
	compiler.set_sflags(sflags);
	return;
}

#ifdef _WIN32
bool parseCompilerOnlyCall(
	_In_ int argc,
	_In_ wchar_t* argv[1],
	_Inout_	int* index,
	Compiler& compiler) {
#else
bool parseCompilerOnlyCall(
	_In_ int argc,
	_In_ char* argv[1],
	_Inout_	int* index,
	Compiler& compiler) {
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

	for (int i = 0; g_CompilerOnlyCall[i].Param != nullptr; i++) {
#ifdef _WIN32
		if (wcscmp(g_CompilerOnlyCall[i].Param, argument) == 0) {
#else
		if (strcmp(g_CompilerOnlyCall[i].Param, argument) == 0) {
#endif
			auto ptr = (gCompilerp*)g_CompilerOnlyCall[i].method;
			ptr(compiler);
			* index += 1;
			return true;
		}
	}
	return false;
}

#ifdef _WIN32
bool parseIgnoredOpts(
	_In_ int argc,
	_In_ wchar_t* argv[1],
	_Inout_	int* index,
	Compiler& compiler) {
#else
bool parseIgnoredOpts(
	_In_ int argc,
	_In_ char* argv[1],
	_Inout_	int* index,
	Compiler & compiler) {
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
	for (int i = 0; g_IgnoredOpts[i] != nullptr; i++) {
#ifdef _WIN32
		if (wcscmp(g_IgnoredOpts[i], argument) == 0) {
#else
		if (strcmp(g_IgnoredOpts[i], argument) == 0) {
#endif
			option_ignored(argument, compiler);
			return true;
		}
	}
	return false;
}

#ifdef _WIN32
bool parseNotSupportedOpts(
	_In_ int argc,
	_In_ wchar_t* argv[1],
	_Inout_	int* index) {
#else
bool parseNotSupportedOpts(
	_In_ int argc,
	_In_ char* argv[1],
	_Inout_	int* index) {
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
	for (int i = 0; g_NotSupportedArgs[i] != nullptr; i++) {
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