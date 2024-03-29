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
#include "efxc2Compiler.h"
#include "efxc2Files.h"

#ifdef _WIN32
void option_ignored(_In_ const wchar_t* Opt, Compiler& compiler);
#else
void option_ignored(_In_ const char* Opt, _In_ Compiler& compiler);
#endif
#ifdef _WIN32
void parseInputFile(_In_ const wchar_t* parameter, Compiler& compiler, Files& files);
#else
void parseInputFile(_In_ const char* parameter, Compiler& compiler, Files& files);
#endif
void cmd_all_resources_bound(Compiler& compiler);
void cmd_Cc(Compiler& compiler);
#ifdef _WIN32
void cmd_D(Compiler& compiler, _In_ const wchar_t* _defineOption);
#else
void cmd_D(Compiler& compiler, _In_ char* _defineOption);
#endif
#ifdef _WIN32
void cmd_E(Compiler& compiler, _In_ const wchar_t* w_entryPoint);
#else
void cmd_E(Compiler& compiler, _In_ char* entryPoint);
#endif
void cmd_enable_unbounded_descriptor_tables(Compiler& compiler);
#ifdef _WIN32
void cmd_Fc(Compiler& compiler, Files& files, _In_ wchar_t* assemblyCodeFile);
#else
void cmd_Fc(Compiler & compiler, Files & files, _In_ char* assemblyCodeFile);
#endif
#ifdef _WIN32
void cmd_Fd(Compiler& compiler, Files& files, _In_ wchar_t* pdbFile);
#else
void cmd_Fd(Compiler& compiler, Files& files, _In_ char* pdbFile);
#endif
#ifdef _WIN32
void cmd_Fh(Compiler& compiler, Files& files, _Inout_ wchar_t* outputFile);
#else
void cmd_Fh(Compiler& compiler, Files& files, _Inout_ char* outputFile);
#endif
#ifdef _WIN32
void cmd_Fo(Compiler& compiler, Files& files, _Inout_ wchar_t* outputFile);
#else
void cmd_Fo(Compiler& compiler, Files& files, _Inout_ char* outputFile);
#endif
void cmd_Gch(Compiler& compiler);
void cmd_Gdp(Compiler& compiler);
void cmd_Gec(Compiler& compiler);
void cmd_Ges(Compiler& compiler);
void cmd_Gfa(Compiler& compiler);
void cmd_Gis(Compiler& compiler);
void cmd_Gpp(Compiler& compiler);
void cmd_Lx(Compiler& compiler);
void cmd_Ni(Compiler& compiler);
void cmd_No(Compiler& compiler);
void cmd_O0(Compiler& compiler);
void cmd_O1(Compiler& compiler);
void cmd_O2(Compiler& compiler);
void cmd_O3(Compiler& compiler);
void cmd_Od(Compiler& compiler);
void cmd_Op(Compiler& compiler);
void cmd_Qstrip_debug(Compiler& compiler);
void cmd_Qstrip_priv(Compiler& compiler);
void cmd_Qstrip_reflect(Compiler& compiler);
void cmd_Qstrip_rootsignature(Compiler& compiler);
void cmd_res_may_alias(Compiler& compiler);
#ifdef _WIN32
void cmd_T(Compiler& compiler, _In_ wchar_t* w_model);
#else
void cmd_T(Compiler& compiler, _In_ char* model);
#endif
void cmd_Vd(Compiler& compiler);
#ifdef _WIN32
void cmd_Vn(Compiler& compiler, _In_ wchar_t* w_variableName);
#else
void cmd_Vn(Compiler& compiler, _In_ char* variableName);
#endif
void cmd_WX(Compiler& compiler);
void cmd_Zi(Compiler& compiler);
void cmd_Zpc(Compiler& compiler);
void cmd_Zpr(Compiler& compiler);
void cmd_Zsb(Compiler& compiler);
void cmd_Zss(Compiler& compiler);

#ifdef _WIN32
using gCompilerFilep = void(Compiler&, Files&, wchar_t*);
#else
using gCompilerFilep = void(Compiler&, Files&, char*);
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
    Compiler& compiler,
    Files& files);
#else
bool parseCompilerFileCall(
    _In_ int argc,
    _In_ char* argv[1],
    _Inout_	int* index,
    Compiler& compiler,
    Files& files);
#endif

using gCompilerp = void (Compiler &);
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
    Compiler& compiler);
#else
bool parseCompilerOnlyCall(
    _In_ char* argv[1],
    _Inout_	int* index,
    Compiler& compiler);
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
    Compiler& compiler);
#else
bool parseIgnoredOpts(
    _In_ char* argv[1],
    _Inout_	int* index,
    Compiler & compiler);
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