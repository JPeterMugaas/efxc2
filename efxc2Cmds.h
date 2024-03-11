//--------------------------------------------------------------------------------------
// File: efxc2Cmds.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#pragma once
#include "efxc2.h"

#ifdef _WIN32
void option_ignored(_In_ const wchar_t* Opt, _In_ int verbose);
#else
void option_ignored(_In_ const char* Opt, _In_ int verbose);
#endif
#ifdef _WIN32
void parseInputFile(_In_ const wchar_t* inputStr, _Out_opt_ wchar_t** inputFile, _Out_ char** c_inputFile, _In_ int verbose);
#else
void parseInputFile(_In_ const char* inputStr, _Out_opt_ char** inputFile, _In_ int verbose);
#endif

void cmd_all_resources_bound(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_D(_In_ int verbose,
	_Inout_ const size_t* numDefines,
#ifdef _WIN32
	_In_ const wchar_t* _defineOption,
	_Inout_ D3D_SHADER_MACRO* defines);
#else
	_In_ char* defineOption,
	_Inout_ D3D_SHADER_MACRO* defines );
#endif
void cmd_E(_In_ int verbose,
#ifdef _WIN32
	_Out_ char** entryPoint,
	_In_ const wchar_t* w_entryPoint);
#else
_In_ char* w_entryPoint);
#endif
void cmd_enable_unbounded_descriptor_tables(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Fd(
	_In_ int verbose,
#ifdef _WIN32
	_In_ wchar_t* pdbFile,
	_Out_ char** c_pdbFile);
#else
	_In_ char* pdbFile);
#endif
void cmd_Fh(
	_In_ int verbose,
	_Inout_ UINT* cmd,
#ifdef _WIN32
	_Inout_ wchar_t* outputFile);
#else
	_Inout_ char* outputFile);
#endif
void cmd_Fo(
	_In_ int verbose,
	_Inout_ UINT* cmd,
#ifdef _WIN32
	_Inout_ wchar_t* outputFile);
#else
	_Inout_ char* outputFile);
#endif
void cmd_Gch(_Inout_ UINT* eflags,
	_In_ int verbose);
void cmd_Gdp(
	_Inout_ UINT* eflags,
	_In_ int verbose);
void cmd_Gec(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Ges(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Gfa(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Gis(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Gpp(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Lx(
	_In_ int verbose,
	_Out_ int* outputHex);
void cmd_O0(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_O1(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_O2(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_O3(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Od(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Op(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Qstrip_debug(
	_Inout_ UINT* strip_flags,
	_In_ int verbose);
void cmd_Qstrip_priv(
	_Inout_ UINT* strip_flags,
	_In_ int verbose);
void cmd_Qstrip_reflect(
	_Inout_ UINT* strip_flags,
	_In_ int verbose);
void cmd_Qstrip_rootsignature(
	_Inout_ UINT* strip_flags,
	_In_ int verbose);
void cmd_res_may_alias(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_T(
	_In_ int verbose,
#ifdef _WIN32
	_Out_ char** model,
	_In_ wchar_t* w_model);
#else
	_In_ char* model);
#endif
void cmd_Vd(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Vn(
	_In_ int verbose,
#ifdef _WIN32
	_Out_ char** variableName,
	_In_ wchar_t* w_variableName);
#else
	_In_ char* variableName);
#endif
void cmd_WX(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Zi(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Zpc(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Zpr(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Zsb(_Inout_ UINT* sflags,
	_In_ int verbose);
void cmd_Zss(_Inout_ UINT* sflags,
	_In_ int verbose);