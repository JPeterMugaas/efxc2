//--------------------------------------------------------------------------------------
// File: efxc2.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2Utils.h"
#include "efxc2Cmds.h"
#include "efxc2Compiler.h"

/*Cygwin and MSYS2 compilers amd linkers don't support
the wmain -Municode entry-point*/

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
#else  /* _WIN32 */
int main(int argc, char* argv[]) {
#endif /* _WIN32 */
	// ====================================================================================
	// Process Command Line Arguments
#ifdef _WIN32
	wchar_t* inputFile = nullptr;
	wchar_t* IncludeFile = nullptr;
	wchar_t* ObjectFile = nullptr;
	wchar_t* pdbFile = nullptr;
	wchar_t* temp = nullptr;
	char* c_pdbFile = nullptr;
#else  /* _WIN32 */
	char* inputFile = nullptr;
	char* IncludeFile = nullptr;
	char* ObjectFile = nullptr;
	char* pdbFile = nullptr;
	char* temp = nullptr;
#endif /* _WIN32 */
	FILE* f;

	Compiler compiler;

	/*first scan specifically for the nologo argument so no output
	is given regardless of parameter order*/
	int index = 1;
	while (index < argc) {
		/* Detect the end of the options. */
		if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
			compiler.set_verbose(false);
			break;
		}
		else {
			index++;
		}
	}

	/*now scan for all arguments and input file name*/
	index = 1;
	while (TRUE) {
		/* Detect the end of the options. */
		if (index >= argc)
			break;
		else if (parseOpt(M_AT_SYMBOL, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -@ not supported");
			print_unsupported_arg_help();
			return 1;
		}
		else if (parseOpt(M_QUESTION_MARK, argc, argv, &index, nullptr)) {
			print_help_screen();
			return 0;
		}
		else if (parseOpt(M_QUESTION_MARK, argc, argv, &index, nullptr)) {
			cmd_all_resources_bound(compiler);
			continue;
		}
		else if (parseOpt(M_CC, argc, argv, &index, nullptr)) {
			option_ignored(M_CC, compiler);
			continue;
		}
		else if (parseOpt(M_COMPRESS, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -compress not supported");
			print_unsupported_arg_help();
			return 1;
		}
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif /* _MSC_VER*/
		else if (parseOpt(M_D, argc, argv, &index, &temp)) {
			cmd_D(compiler, temp);
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
#else  /* _WIN32 */
		else if (parseOpt(M_D, argc, argv, &index, &temp)) {
			cmd_D(compiler, temp);
#endif /* _WIN32 */
			continue;
		}
		else if (parseOpt(M_DECOMPRESS, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -decompress not supported");
			print_unsupported_arg_help();
			return 1;
		}
		else if (parseOpt(M_DUMPBIN, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -dumpbin not supported");
			print_unsupported_arg_help();
			return 1;
		}

		else if (parseOpt(M_E_, argc, argv, &index, &temp)) {
			cmd_E(compiler, temp);
#ifdef _WIN32
			delete[] temp;
#endif
			continue;
		}
		else if (parseOpt(M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES, argc, argv, &index, nullptr)) {
			cmd_enable_unbounded_descriptor_tables(compiler);
			continue;
		}
		else if (parseOpt(M_FD, argc, argv, &index, &pdbFile)) {
#ifdef _WIN32
			cmd_Fd(compiler, pdbFile, &c_pdbFile);
#else
			cmd_Fd(compiler, pdbFile);
#endif
			continue;
		}
		else if (parseOpt(M_FE, argc, argv, &index, nullptr)) {
			option_ignored(M_FE, compiler);
			continue;
		}
		else if (parseOpt(M_FH, argc, argv, &index, &IncludeFile)) {
			cmd_Fh(compiler, IncludeFile);
			continue;
		}
		else if (parseOpt(M_FL, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -Fl not supported");
			print_unsupported_arg_help();
			return 1;
		}
		else if (parseOpt(M_FO, argc, argv, &index, &ObjectFile)) {
			cmd_Fo(compiler, ObjectFile);
			continue;
		}
		else if (parseOpt(M_FORCE_ROOTSIG_VER, argc, argv, &index, nullptr)) {
			option_ignored(argv[index], compiler);
			continue;
		}
		else if (parseOpt(M_FX, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -Fx not supported");
			print_unsupported_arg_help();
			return 1;
		}
		else if (parseOpt(M_GCH, argc, argv, &index, nullptr)) {
			cmd_Gch(compiler);
			continue;
		}
		else if (parseOpt(M_GDP, argc, argv, &index, nullptr)) {
			cmd_Gdp(compiler);
			continue;
		}
		else if (parseOpt(M_GEC, argc, argv, &index, nullptr)) {
			cmd_Gec(compiler);
			continue;
		}
		else if (parseOpt(M_GES, argc, argv, &index, nullptr)) {
			cmd_Ges(compiler);
			continue;
		}
		else if (parseOpt(M_GETPRIVATE, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -getprivate not supported");
			print_unsupported_arg_help();
			return 1;
		}
		else if (parseOpt(M_GFA, argc, argv, &index, nullptr)) {
			cmd_Gfa(compiler);
			continue;
		}
		else if (parseOpt(M_GIS, argc, argv, &index, nullptr)) {
			cmd_Gis(compiler);
			continue;
		}
		else if (parseOpt(M_GPP, argc, argv, &index, nullptr)) {
			cmd_Gpp(compiler);
			continue;
		}
		else if (parseOpt(M_HELP, argc, argv, &index, nullptr)) {
			print_help_screen();
			return 0;
		}
		else if (parseOpt(M_I, argc, argv, &index, nullptr)) {
			option_ignored(M_I, compiler);
			continue;
		}
		else if (parseOpt(M_LX, argc, argv, &index, nullptr)) {
			cmd_Lx(compiler);
			continue;
		}
		else if (parseOpt(M_MATCHUAVS, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -matchUAVs not supported");
			print_unsupported_arg_help();
			return 1;
		}
		else if (parseOpt(M_MERGEUAVS, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -mergeUAVs not supported");
			print_unsupported_arg_help();
			return 1;
		}
		else if (parseOpt(M_NI, argc, argv, &index, nullptr)) {
			option_ignored(M_NI, compiler);
			continue;
		}
		else if (parseOpt(M_NO, argc, argv, &index, nullptr)) {
			option_ignored(M_NO, compiler);
			continue;
		}
		else if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
			continue;
		}
		else if (parseOpt(M_O0, argc, argv, &index, nullptr)) {
			cmd_O0(compiler);
			continue;
		}
		else if (parseOpt(M_O1, argc, argv, &index, nullptr)) {
			cmd_O1(compiler);
			continue;
		}
		else if (parseOpt(M_O2, argc, argv, &index, nullptr)) {
			cmd_O2(compiler);
			continue;
		}
		else if (parseOpt(M_O3, argc, argv, &index, nullptr)) {
			cmd_O3(compiler);
			continue;
		}
		else if (parseOpt(M_OD, argc, argv, &index, nullptr)) {
			cmd_Od(compiler);
			continue;
		}
		else if (parseOpt(M_OP, argc, argv, &index, nullptr)) {
			cmd_Op(compiler);
			continue;
		}
		else if (parseOpt(M_P, argc, argv, &index, nullptr)) {
			option_ignored(M_P, compiler);
			continue;
		}
		else if (parseOpt(M_QSTRIP_DEBUG, argc, argv, &index, nullptr)) {
			cmd_Qstrip_debug(compiler);
			continue;
		}
		else if (parseOpt(M_QSTRIP_PRIV, argc, argv, &index, nullptr)) {
			cmd_Qstrip_priv(compiler);
			continue;
		}
		else if (parseOpt(M_QSTRIP_REFLECT, argc, argv, &index, nullptr)) {
			cmd_Qstrip_reflect(compiler);
			continue;
		}
		else if (parseOpt(M_QSTRIP_ROOTSIGNATURE, argc, argv, &index, nullptr)) {
			cmd_Qstrip_rootsignature(compiler);
			continue;
		}
		else if (parseOpt(M_RES_MAY_ALIAS, argc, argv, &index, nullptr)) {
			cmd_res_may_alias(compiler);
			continue;
		}
		else if (parseOpt(M_SETPRIVATE, argc, argv, &index, nullptr)) {
			fprintf(stderr, "option -setprivate not supported");
			print_unsupported_arg_help();
			return 1;
		}

		else if (parseOpt(M_T, argc, argv, &index, &temp)) {
			cmd_T(compiler, temp);
#ifdef _WIN32
			delete[] temp;
#endif
			continue;
		}
		else if (parseOpt(M_VD, argc, argv, &index, nullptr)) {
			cmd_Vd(compiler);
			continue;
		}
		else if (parseOpt(M_VERSION, argc, argv, &index, nullptr)) {
			print_version();
		}
		else if (parseOpt(M_VI, argc, argv, &index, nullptr)) {
			option_ignored(M_VI, compiler);
			continue;
		}
		else if (parseOpt(M_VN, argc, argv, &index, &temp)) {
			cmd_Vn(compiler, temp);
#ifdef _WIN32
			delete[] temp;
#endif /* _WIN32 */
			continue;
		}
		else if (parseOpt(M_WX, argc, argv, &index, nullptr)) {
			cmd_WX(compiler);
			continue;
		}
		else if (parseOpt(M_ZI, argc, argv, &index, nullptr)) {
			cmd_Zi(compiler);
			continue;
		}
		else if (parseOpt(M_ZPC, argc, argv, &index, nullptr)) {
			cmd_Zpc(compiler);
			continue;
		}
		else if (parseOpt(M_ZPR, argc, argv, &index, nullptr)) {
			cmd_Zpr(compiler);
			continue;
		}
		else if (parseOpt(M_ZSB, argc, argv, &index, nullptr)) {
			cmd_Zsb(compiler);
			continue;
		}
		else if (parseOpt(M_ZSS, argc, argv, &index, nullptr)) {
			cmd_Zss(compiler);
			continue;
		}
		else {
#ifdef _WIN32
			parseInputFile(argv[index], &inputFile, compiler);
#else
			parseInputFile(argv[index], &inputFile, compiler);
#endif
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
			fclose(f);
			index += 1;
		}
	}

	if (inputFile == nullptr)
		print_usage_missing("inputFile");
	if (IncludeFile == nullptr)
		print_usage_missing("IncludeFile");

	// ====================================================================================
	// Shader Compilation
	ID3DBlob* output = nullptr;

	compiler.Compile();
#ifdef _WIN32
	errno_t err = 0;
#endif
	size_t  outputLen = 0;
	/*write .PDB data if applicable*/
#ifdef _WIN32
	if (c_pdbFile != nullptr) {
		auto pPDBFileName = GetFileName(c_pdbFile);
#else
	if (pdbFile != nullptr) {
		auto pPDBFileName = GetFileName(pdbFile);
#endif
		if (pPDBFileName != nullptr) {
			compiler.SetPDBFileName(pPDBFileName);
		}
		else {
			/* if only a dir was specified, use the default
			filename in the shader data. */
			pPDBFileName = compiler.GetPDBFileName();
#ifdef _WIN32
			wchar_t* w_PDBFileName = utf8_decode(pPDBFileName, strlen(pPDBFileName));
			pdbFile = concat(pdbFile, w_PDBFileName);
			free(w_PDBFileName);
#else
			pdbFile = concat(pdbFile, pPDBFileName);
#endif
		}
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
		err = _wfopen_s(&f, pdbFile, L"w");
		if (err != 0) {
			print_errno(err);
		}
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
		f = fopen(IncludeFile, "w");
		if (f == nullptr) {
			print_errno();
		}
#endif
		outputLen = compiler.WritePDBFile(f);
		fclose(f);
#ifdef _WIN32
		wprintf(L"Wrote %zu bytes of .PDB data to %ls\n", outputLen, pdbFile);
#else   /* _WIN32 */
		printf("Wrote %zu", outputLen);
		printf(" bytes of .PDB data to %ls\n", pdbFile);
#endif  /* WIN32 */
	}
	// ====================================================================================
	// Output (or errors)
	compiler.StripShader();
	if ((compiler.get_commands() & CMD_WRITE_HEADER) == CMD_WRITE_HEADER) {
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
		err = _wfopen_s(&f, IncludeFile, L"w");
		if (err != 0) {
			print_errno(err);
		}
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
		f = fopen(IncludeFile, "w");
		if (f == nullptr) {
			print_errno();
		}
#endif
		outputLen = compiler.WriteIncludeFile(f);
		fclose(f);
		if (compiler.get_verbose()) {
#ifdef _WIN32
			wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, IncludeFile);
#else   /* _WIN32 */
			printf("Wrote %zu", outputLen);
			printf(" bytes of shader output to %ls\n", IncludeFile);
#endif  /* WIN32 */
		}
	}
	if ((compiler.get_commands() & CMD_WRITE_OBJECT) == CMD_WRITE_OBJECT) {
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
		err = _wfopen_s(&f, ObjectFile, L"w");
		if (err != 0) {
			print_errno(err);
		}
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
		f = fopen(ObjectFile, "w");
		if (f == nullptr) {
			print_errno();
		}
#endif
		outputLen = compiler.WriteObjectFile(f);
		fclose(f);
		if (compiler.get_verbose()) {
#ifdef _WIN32
			wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, ObjectFile);
#else   /* _WIN32 */
			printf("Wrote %zu", outputLen);
			printf(" bytes of shader output to %ls\n", ObjectFile);
#endif
		}
	}
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
	return 0;
}