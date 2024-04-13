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

void option_ignored(_In_ const M_STRING& Opt, _In_ const CompilerParams& params);
void parseInputFile(_In_ const M_STRING& parameter, CompilerParams& params, Files& files);
void cmd_all_resources_bound(CompilerParams& params);
void cmd_Cc(CompilerParams& params);
void cmd_D(CompilerParams& params, _In_ const M_STRING& _defineOption);
void cmd_E(CompilerParams& params, _In_ const M_STRING& _entryPoint);
void cmd_enable_unbounded_descriptor_tables(CompilerParams& params);
void cmd_Fc(CompilerParams& params, Files& files, _In_ const M_STRING& assemblyCodeFile);
void cmd_Fd(CompilerParams& params, Files& files, _In_ const M_STRING& pdbFile);
void cmd_Fh(CompilerParams& params, Files& files, _In_ const M_STRING& outputFile);
void cmd_Fo(CompilerParams& params, Files& files, _In_ const M_STRING& outputFile);
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
void cmd_T(CompilerParams& params, _In_ const M_STRING& _model);
void cmd_Vd(CompilerParams& params);
void cmd_Vn(CompilerParams& params, _In_ const M_STRING& _variableName);
void cmd_WX(CompilerParams& params);
void cmd_Zi(CompilerParams& params);
void cmd_Zpc(CompilerParams& params);
void cmd_Zpr(CompilerParams& params);
void cmd_Zsb(CompilerParams& params);
void cmd_Zss(CompilerParams& params);

using gCompilerFilep = void(CompilerParams&, Files&, const M_STRING&);
struct CompileFileEntry {
    const M_STRING Param;
    gCompilerFilep* method;
};

constexpr auto COMPILER_FILE_ENTRIES_LENGTH = 4;
const std::array <CompileFileEntry, COMPILER_FILE_ENTRIES_LENGTH> g_CompilerFileCall = { {
    { M_FC, cmd_Fc },
    { M_FD, cmd_Fd },
    { M_FH, cmd_Fh },
    { M_FO, cmd_Fo }
} };

bool parseCompilerFileCall(
    _In_ const M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    CompilerParams& params,
    Files& files);

using gCompilerp = void (CompilerParams &);
struct CompilerOnlyEntry {
    const M_STRING Param;
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

bool  parseCompilerOnlyCall(
    _In_ const M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    CompilerParams& params);

constexpr auto IGNORED_OPTS_LENGTH = 5;
#ifdef _WIN32
const std::array <const std::wstring, IGNORED_OPTS_LENGTH>g_IgnoredOpts = {
#else
const std::array <const std::string,IGNORED_OPTS_LENGTH>g_IgnoredOpts = {
#endif
    M_FE,
    M_FORCE_ROOTSIG_VER,
    M_I,
    M_P,
    M_VI};

bool parseIgnoredOpts(
    _In_ const M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    CompilerParams& params);

bool parseNotSupportedOpts(
    _In_ const M_CMD_PARAMS& args,
    _In_ const size_t* index);

constexpr auto NOT_SUPPORTED_LENGTH = 10;
#ifdef _WIN32
const std::array <const std::wstring, NOT_SUPPORTED_LENGTH>g_NotSupportedArgs = { {
#else
const std::array <const std::string, NOT_SUPPORTED_LENGTH>g_NotSupportedArgs = { {
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