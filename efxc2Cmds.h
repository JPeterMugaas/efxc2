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
#ifndef EFXC2CMDS_H_INCLUDED
#define EFXC2CMDS_H_INCLUDED

#include "efxc2.h"
#include "efxc2CompilerParams.h"
#include "efxc2Files.h"

#ifdef _WIN32
void option_ignored(_In_ const wchar_t* Opt, CompilerParams& params);
#else
void option_ignored(_In_ const char* Opt, _In_ CompilerParams& params);
#endif
#ifdef _WIN32
void parseInputFile(_In_ const wchar_t* parameter, CompilerParams& params, Files& files);
#else
void parseInputFile(_In_ const char* parameter, CompilerParams& params, Files& files);
#endif
void cmd_all_resources_bound(CompilerParams& params);
void cmd_Cc(CompilerParams& params);
#ifdef _WIN32
void cmd_D(CompilerParams& params, _In_ const wchar_t* _defineOption);
#else
void cmd_D(CompilerParams& params, _In_ char* _defineOption);
#endif
#ifdef _WIN32
void cmd_E(CompilerParams& params, _In_ const std::wstring& w_entryPoint);
#else
void cmd_E(CompilerParams& params, _In_ const std::string entryPoint);
#endif
void cmd_enable_unbounded_descriptor_tables(CompilerParams& params);
#ifdef _WIN32
void cmd_Fc(CompilerParams& params, Files& files, _In_ const std::wstring& assemblyCodeFile);
#else
void cmd_Fc(CompilerParams& params, Files& files, _In_ const std::string& assemblyCodeFile);
#endif
#ifdef _WIN32
void cmd_Fd(CompilerParams& params, Files& files, _In_ const std::wstring& pdbFile);
#else
void cmd_Fd(CompilerParams& params, Files& files, _In_ const std::string& pdbFile);
#endif
#ifdef _WIN32
void cmd_Fh(CompilerParams& params, Files& files, _In_ const std::wstring& outputFile);
#else
void cmd_Fh(CompilerParams& params, Files& files, _In_ const std::string& outputFile);
#endif
#ifdef _WIN32
void cmd_Fo(CompilerParams& params, Files& files, _In_ const std::wstring& outputFile);
#else
void cmd_Fo(CompilerParams& params, Files& files, _In_ const std::string& outputFile);
#endif
void cmd_Gch(CompilerParams& params);
void cmd_Gdp(CompilerParams& params);
void cmd_Gec(CompilerParams& params);
void cmd_Ges(CompilerParams& params);
void cmd_Gfa(CompilerParams& params);
void cmd_Gis(CompilerParams& params);
void cmd_Gpp(CompilerParams& params);
void cmd_Lx(CompilerParams& params);
void cmd_Ni(CompilerParams& params);
void cmd_No(CompilerParams& params);
void cmd_O0(CompilerParams& params);
void cmd_O1(CompilerParams& params);
void cmd_O2(CompilerParams& params);
void cmd_O3(CompilerParams& params);
void cmd_Od(CompilerParams& params);
void cmd_Op(CompilerParams& params);
void cmd_Qstrip_debug(CompilerParams& params);
void cmd_Qstrip_priv(CompilerParams& params);
void cmd_Qstrip_reflect(CompilerParams& params);
void cmd_Qstrip_rootsignature(CompilerParams& params);
void cmd_res_may_alias(CompilerParams& params);
#ifdef _WIN32
void cmd_T(CompilerParams& params, _In_ const std::wstring& w_model);
#else
void cmd_T(CompilerParams& params, _In_ const std::string& model);
#endif
void cmd_Vd(CompilerParams& params);
#ifdef _WIN32
void cmd_Vn(CompilerParams& params, _In_ const std::wstring& w_variableName);
#else
void cmd_Vn(CompilerParams& params, _In_ const std::string& variableName);
#endif
void cmd_WX(CompilerParams& params);
void cmd_Zi(CompilerParams& params);
void cmd_Zpc(CompilerParams& params);
void cmd_Zpr(CompilerParams& params);
void cmd_Zsb(CompilerParams& params);
void cmd_Zss(CompilerParams& params);

#ifdef _WIN32
using gCompilerFilep = void(CompilerParams&, Files&, const std::wstring&);
#else
using gCompilerFilep = void(CompilerParams&, Files&, const std::string&);
#endif
struct CompileFileEntry {
#ifdef _WIN32
    const wchar_t* Param;
#else
    const char* Param;
#endif
    gCompilerFilep* method;
};

constexpr auto COMPILER_FILE_ENTRIES_LENGTH = 4;
const std::array <CompileFileEntry, COMPILER_FILE_ENTRIES_LENGTH> g_CompilerFileCall = { {
    { M_FC, cmd_Fc },
    { M_FD, cmd_Fd },
    { M_FH, cmd_Fh },
    { M_FO, cmd_Fo }
} };

#ifdef _WIN32
bool parseCompilerFileCall(
    _In_ int argc,
    _In_ wchar_t* argv[1],
    _Inout_	int* index,
    CompilerParams& params,
    Files& files);
#else
bool parseCompilerFileCall(
    _In_ int argc,
    _In_ char* argv[1],
    _Inout_	int* index,
    CompilerParams& params,
    Files& files);
#endif

using gCompilerp = void (CompilerParams &);
struct CompilerOnlyEntry {
#ifdef _WIN32
    const wchar_t* Param;
#else
    const char* Param;
#endif
    gCompilerp* method;
};

constexpr auto COMPILER_ONLY_ENTRIES_LENGTH = 29;

const std::array <CompilerOnlyEntry, COMPILER_ONLY_ENTRIES_LENGTH> g_CompilerOnlyCall = { {
   { M_ALL_RESOURCES_BOUND, cmd_all_resources_bound },
    { M_CC, cmd_Cc },
    { M_GCH, cmd_Gch },
    { M_GDP, cmd_Gdp },
    { M_GEC, cmd_Gec },
    { M_GES, cmd_Ges },
    { M_GFA, cmd_Gfa },
    { M_GIS, cmd_Gis },
    { M_GPP, cmd_Gpp },
    { M_LX, cmd_Lx },
    { M_NI, cmd_Ni },
    { M_NO, cmd_No },
    { M_O0, cmd_O0 },
    { M_O1, cmd_O1 },
    { M_O2, cmd_O2 },
    { M_O3, cmd_O3 },
    { M_OD, cmd_Od },
    { M_OP, cmd_Op },
    { M_QSTRIP_DEBUG, cmd_Qstrip_debug },
    { M_QSTRIP_PRIV, cmd_Qstrip_priv },
    { M_QSTRIP_REFLECT, cmd_Qstrip_reflect },
    { M_QSTRIP_ROOTSIGNATURE, cmd_Qstrip_rootsignature },
    { M_RES_MAY_ALIAS, cmd_res_may_alias },
    { M_WX, cmd_WX },
    { M_ZI, cmd_Zi },
    { M_ZPC, cmd_Zpc },
    { M_ZPR, cmd_Zpr },
    { M_ZSB, cmd_Zsb },
    { M_ZSS, cmd_Zss },
}};


#ifdef _WIN32
bool parseCompilerOnlyCall(
    _In_ wchar_t* argv[1],
    _Inout_	int* index,
    CompilerParams& params);
#else
bool parseCompilerOnlyCall(
    _In_ char* argv[1],
    _Inout_	int* index,
    CompilerParams& params);
#endif

constexpr auto IGNORED_OPTS_LENGTH = 5;
#ifdef _WIN32
const std::array <const wchar_t*, IGNORED_OPTS_LENGTH>g_IgnoredOpts = {
#else
const std::array <const char*,IGNORED_OPTS_LENGTH>g_IgnoredOpts = {
#endif
    M_FE,
    M_FORCE_ROOTSIG_VER,
    M_I,
    M_P,
    M_VI};

#ifdef _WIN32
bool parseIgnoredOpts(
    _In_ wchar_t* argv[1],
    _Inout_	int* index,
    CompilerParams& params);
#else
bool parseIgnoredOpts(
    _In_ char* argv[1],
    _Inout_	int* index,
    CompilerParams& params);
#endif

#ifdef _WIN32
bool parseNotSupportedOpts(
    _In_ wchar_t* argv[1],
    _In_ const int* index);
#else
bool parseNotSupportedOpts(
    _In_ char* argv[1],
    _In_ const int* index);
#endif

constexpr auto NOT_SUPPORTED_LENGTH = 10;
#ifdef _WIN32
const std::array <const wchar_t*, NOT_SUPPORTED_LENGTH>g_NotSupportedArgs = { {
#else
const std::array <const char*, NOT_SUPPORTED_LENGTH>g_NotSupportedArgs = { {
#endif
    M_AT_SYMBOL,
    M_COMPRESS,
    M_DECOMPRESS,
    M_DUMPBIN,
    M_FL,
    M_FX,
    M_GETPRIVATE,
    M_MATCHUAVS,
    M_MERGEUAVS,
    M_SETPRIVATE
} };

#endif /*EFXC2CMDS_H_INCLUDE*/