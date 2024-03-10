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

#ifdef _WIN32
void option_ignored(_In_ const wchar_t* Opt, _In_ int verbose) {
	if (verbose) {
		wprintf(L"Option %ls ignored", Opt);
	}
	return;
}
#else
void option_ignored(_In_ const char* Opt, _In_ int verbose) {
	if (verbose) {
		wprintf(L"Option %s ignored", Opt);
	}
	return;
}
#endif

#ifdef _WIN32
void parseInputFile(_In_ const wchar_t* inputStr, _Out_ wchar_t** inputFile, _Out_ char** c_inputFile, _In_ int verbose) {
#else
void parseInputFile(_In_ const char* inputStr, _Out_ char** inputFile, _In_ int verbose) {
#endif
	if (!*inputFile) {
		if (verbose) {
			printf("Parse input file name\n");
		}
#ifdef _WIN32
		* inputFile = new wchar_t[wcslen(inputStr) + 1];
		wcscpy_s(*inputFile, wcslen(inputStr) + 1, inputStr);
		FixupFileName(*inputFile);
		*c_inputFile = wcharToChar(*inputFile);
#else  /* _WIN32 */
		* inputFile = new char[strlen(inputStr) + 1];
		strncpy(*inputFile, inputStr, strlen(inputStr) + 1);
#endif /* _WIN32 */

		if (verbose) {
#ifdef _WIN32
			wprintf(L"input file: %ls\n", *inputFile);
#else  /* _WIN32 */
			printf("input file: %ls\n", *inputFile);
#endif /* _WIN32 */
		}
	}
	else {
		print_usage_toomany();
	}
}

void cmd_all_resources_bound(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -all_resources_bound sflags | D3DCOMPILE_ALL_RESOURCES_BOUND\n");
	}
	*sflags = *sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
	return;
}

void cmd_D(_In_ int verbose,
	_Inout_ const size_t * numDefines,
#ifdef _WIN32
	_In_ const wchar_t* _defineOption,
	_Inout_ D3D_SHADER_MACRO * defines) {
	char* defineOption = wcharToChar(_defineOption);
#else
	_In_ char* defineOption,
	_Inout_ D3D_SHADER_MACRO* defines ) {
#endif

	assert(defineOption == nullptr);
	numDefines++;
	//Copy the old array into the new array, but put the new definition at the beginning
	auto newDefines = new D3D_SHADER_MACRO[*numDefines];
	for (size_t i = 1; i < *numDefines; i++) {
		newDefines[i] = defines[i - 1];
	}
	delete[] defines;
	defines = newDefines;
	defines[0].Name = defineOption;
	defines[0].Definition = "1";
	if (verbose) {
		printf("option -D with arg %s\n", defineOption);
	}
}

void cmd_E(_In_ int verbose,
#ifdef _WIN32
	_Out_ char** entryPoint,
	_In_ const wchar_t* w_entryPoint) {
#else
_In_ char* entryPoint) {
#endif
#ifdef _WIN32
	* entryPoint = wcharToChar(w_entryPoint);
#endif
	if (verbose) {
		printf("option -E (Entry Point) with arg '%s'\n", *entryPoint);
	}
	return;
}

void cmd_enable_unbounded_descriptor_tables(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -enable_unbounded_descriptor_tables sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n");
	}
	*sflags = *sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
	return;
}

void cmd_Fd(
	_In_ int verbose,
#ifdef _WIN32
	_In_ wchar_t* pdbFile) {
#else
	_In_ char* pdbFile) {
#endif
#ifdef _WIN32
	FixupFileName(pdbFile);
#endif /* _WIN32 */
	if (verbose) {
		printf("option -Fd (.PDB) with arg %ls\n", pdbFile);
	}
	return;
}

void cmd_Fh(
	_In_ int verbose,
	_Inout_ UINT * cmd,
#ifdef _WIN32
	_Inout_ wchar_t* outputFile) {
#else
	_Inout_ char* outputFile) {
#endif
#ifdef _WIN32
	FixupFileName(outputFile);
#endif /* _WIN32 */
	* cmd = *cmd | 1;
	if (verbose) {
		printf("option -Fh (Output File) with arg %ls\n", outputFile);
	}
	return;
}

void cmd_Fo(
	_In_ int verbose,
	_Inout_ UINT * cmd,
#ifdef _WIN32
	_Inout_ wchar_t* outputFile) {
#else
	_Inout_ char* outputFile) {
#endif
#ifdef _WIN32
	FixupFileName(outputFile);
#endif /* _WIN32 */
	* cmd = *cmd | 2;
	if (verbose) {
		printf("option -FO (Output File) with arg %ls\n", outputFile);
	}
	return;
}

void cmd_Gch(_Inout_ UINT * eflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Gch eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT");
	}
	*eflags = *eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
	return;
}

void cmd_Gdp(
	_Inout_ UINT * eflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Gdp eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS");
	}
	*eflags = *eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
	return;
}

void cmd_Gec(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Gec sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n");
	}
	*sflags = *sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
	return;
}

void cmd_Ges(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Ges sflags | D3DCOMPILE_ENABLE_STRICTNESS\n");
	}
	*sflags = *sflags | D3DCOMPILE_ENABLE_STRICTNESS;
	return;
}

void cmd_Gfa(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Gfa sflags | D3DCOMPILE_AVOID_FLOW_CONTROL\n");
	}
	*sflags = *sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
	return;
}

void cmd_Gis(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Gis sflags | D3DCOMPILE_IEEE_STRICTNESS\n");
	}
	*sflags = *sflags | D3DCOMPILE_IEEE_STRICTNESS;
	return;
}

void cmd_Gpp(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Gpp sflags | D3DCOMPILE_PARTIAL_PRECISION\n");
	}
	*sflags = *sflags | D3DCOMPILE_PARTIAL_PRECISION;
	return;
}

void cmd_Lx(
	_In_ int verbose,
	_Out_ int* outputHex) {
	*outputHex = 1;
	if (verbose) {
		printf("option -Lx - output hexidecimal literals\n");
	}
	return;
}

void cmd_O0(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -O0 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0\n");
	}
	*sflags = *sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
	return;
}

void cmd_O1(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1\n");
	}
	*sflags = *sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
	return;
}

void cmd_O2(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2\n");
	}
	*sflags = *sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
	return;
}

void cmd_O3(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3\n");
	}
	*sflags = *sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
	return;
}

void cmd_Od(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Od sflags | D3DCOMPILE_SKIP_OPTIMIZATION\n");
	}
	*sflags = *sflags | D3DCOMPILE_SKIP_OPTIMIZATION;
	return;
}

void cmd_Op(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Op sflags | D3DCOMPILE_NO_PRESHADER\n");
	}
	*sflags = *sflags | D3DCOMPILE_NO_PRESHADER;
	return;
}

void cmd_Qstrip_debug(
	_Inout_ UINT * strip_flags,
	_In_ int verbose) {
	*strip_flags = *strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO;
	if (verbose) {
		printf("option -Qstrip_debug strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO\n");
	}
	return;
}

void cmd_Qstrip_priv(
	_Inout_ UINT * strip_flags,
	_In_ int verbose) {
	*strip_flags = *strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA;
	if (verbose) {
		printf("option -Qstrip_priv strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA\n");
	}
	return;
}

void cmd_Qstrip_reflect(
	_Inout_ UINT * strip_flags,
	_In_ int verbose) {
	*strip_flags = *strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA;
	if (verbose) {
		printf("option -Qstrip_reflect strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA\n");
	}
	return;
}

void cmd_Qstrip_rootsignature(
	_Inout_ UINT * strip_flags,
	_In_ int verbose) {
	*strip_flags = *strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE;
	if (verbose) {
		printf("option -Qstrip_rootsignature strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE\n");
	}
	return;
}

void cmd_res_may_alias(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -res_may_alias sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS\n");
	}
	*sflags = *sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
	return;
}

void cmd_T(
	_In_ int verbose,
#ifdef _WIN32
	_Out_ char** model,
	_In_ wchar_t* w_model) {
#else
	_In_ char* model) {
#endif
#ifdef _WIN32
	* model = wcharToChar(w_model);
#endif
	if (verbose) {
		printf("option -T (Shader Model/Profile) with arg '%s'\n", *model);
	}
	return;
}

void cmd_Vd(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Vd sflags | D3DCOMPILE_SKIP_VALIDATION\n");
	}
	*sflags = *sflags | D3DCOMPILE_SKIP_VALIDATION;
	return;
}

void cmd_Vn(
	_In_ int verbose,
#ifdef _WIN32
	_Out_ char** variableName,
	_In_ wchar_t* w_variableName) {
#else
	_In_ char* variableName) {
#endif
#ifdef _WIN32
	* variableName = wcharToChar(w_variableName);
#endif
	if (verbose) {
		printf("option -Vn (Variable Name) with arg '%s'\n", *variableName);
	}
	return;
}

void cmd_WX(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -WX sflags |  D3DCOMPILE_WARNINGS_ARE_ERRORS\n");
	}
	*sflags = *sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
	return;
}

void cmd_Zi(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Zi sflags | D3DCOMPILE_DEBUG\n");
	}
	*sflags = *sflags | D3DCOMPILE_DEBUG;
	return;
}

void cmd_Zpc(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Zpc flags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n");
	}
	*sflags = *sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
	return;
}

void cmd_Zpr(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Zpr sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n");
	}
	*sflags = *sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	return;
}

void cmd_Zsb(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Zsb sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n");
	}
	*sflags = *sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
	return;
}

void cmd_Zss(_Inout_ UINT * sflags,
	_In_ int verbose) {
	if (verbose) {
		printf("option -Zss sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n");
	}
	*sflags = *sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
	return;
}