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

namespace efxc2Cmds {
    void option_ignored(_In_ const efxc2Utils::M_STRING_VIEW Opt, _In_ const efxc2CompilerParams::CompilerParams& params);
    void parseInputFile(_In_ const efxc2Utils::M_STRING_VIEW parameter, efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files);
    void cmd_all_resources_bound(efxc2CompilerParams::CompilerParams& params);
    void cmd_Cc(efxc2CompilerParams::CompilerParams& params);
    void cmd_D(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _defineOption);
    void cmd_E(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _entryPoint);
    void cmd_enable_unbounded_descriptor_tables(efxc2CompilerParams::CompilerParams& params);
    void cmd_Fc(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW assemblyCodeFile);
    void cmd_Fd(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW pdbFile);
    void cmd_Fh(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile);
    void cmd_Fo(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile);
    void cmd_Gch(efxc2CompilerParams::CompilerParams& params);
    void cmd_Gdp(efxc2CompilerParams::CompilerParams& params);
    void cmd_Gec(efxc2CompilerParams::CompilerParams& params);
    void cmd_Ges(efxc2CompilerParams::CompilerParams& params);
    void cmd_Gfa(efxc2CompilerParams::CompilerParams& params);
    void cmd_Gis(efxc2CompilerParams::CompilerParams& params);
    void cmd_Gpp(efxc2CompilerParams::CompilerParams& params);
    void cmd_I(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _includeDir);
    void cmd_Lx(efxc2CompilerParams::CompilerParams& params);
    void cmd_Ni(efxc2CompilerParams::CompilerParams& params);
    void cmd_No(efxc2CompilerParams::CompilerParams& params);
    void cmd_O0(efxc2CompilerParams::CompilerParams& params);
    void cmd_O1(efxc2CompilerParams::CompilerParams& params);
    void cmd_O2(efxc2CompilerParams::CompilerParams& params);
    void cmd_O3(efxc2CompilerParams::CompilerParams& params);
    void cmd_Od(efxc2CompilerParams::CompilerParams& params);
    void cmd_Op(efxc2CompilerParams::CompilerParams& params);
    void cmd_P(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile);
    void cmd_Qstrip_debug(efxc2CompilerParams::CompilerParams& params);
    void cmd_Qstrip_priv(efxc2CompilerParams::CompilerParams& params);
    void cmd_Qstrip_reflect(efxc2CompilerParams::CompilerParams& params);
    void cmd_Qstrip_rootsignature(efxc2CompilerParams::CompilerParams& params);
    void cmd_res_may_alias(efxc2CompilerParams::CompilerParams& params);
    void cmd_setprivate(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW inputfile);
    void cmd_T(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _model);
    void cmd_Vd(efxc2CompilerParams::CompilerParams& params);
    void cmd_Vn(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _variableName);
    void cmd_WX(efxc2CompilerParams::CompilerParams& params);
    void cmd_Zi(efxc2CompilerParams::CompilerParams& params);
    void cmd_Zpc(efxc2CompilerParams::CompilerParams& params);
    void cmd_Zpr(efxc2CompilerParams::CompilerParams& params);
    void cmd_Zsb(efxc2CompilerParams::CompilerParams& params);
    void cmd_Zss(efxc2CompilerParams::CompilerParams& params);

    using gCompilerFilep = void(efxc2CompilerParams::CompilerParams&, efxc2Files::Files&, const efxc2Utils::M_STRING_VIEW);
    struct CompileFileEntry {
        const efxc2Utils::M_STRING Param;
        gCompilerFilep* method = nullptr; //-V122
    };

    constexpr auto COMPILER_FILE_ENTRIES_LENGTH = 6;
    const std::array <CompileFileEntry, COMPILER_FILE_ENTRIES_LENGTH> g_CompilerFileCall = { {
        { M_FC, cmd_Fc },
        { M_FD, cmd_Fd },
        { M_FH, cmd_Fh },
        { M_FO, cmd_Fo },
        { M_P, cmd_P },
        { M_SETPRIVATE, cmd_setprivate }
    } };

    bool parseCompilerFileCall(
        _In_ const efxc2Utils::M_CMD_PARAMS& args,
        _Inout_	size_t* index,
        efxc2CompilerParams::CompilerParams& params,
        efxc2Files::Files& files);

    using gCompilerp = void(efxc2CompilerParams::CompilerParams&);
    struct CompilerOnlyEntry {
        const efxc2Utils::M_STRING Param; //-V122
        gCompilerp* method;  //-V122
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
    } };

    bool  parseCompilerOnlyCall(
        _In_ const efxc2Utils::M_CMD_PARAMS& args,
        _Inout_	size_t* index,
        efxc2CompilerParams::CompilerParams& params);

    constexpr auto IGNORED_OPTS_LENGTH = 4;
#ifdef _WIN32
    const std::array <const std::wstring, IGNORED_OPTS_LENGTH>g_IgnoredOptions = {
    #else
    const std::array <const std::string,IGNORED_OPTS_LENGTH>g_IgnoredOptions = {
    #endif
        M_FE,
        M_FL,
        M_FORCE_ROOTSIG_VER,
        M_VI };

    bool parseIgnoredOptions(
        _In_ const efxc2Utils::M_CMD_PARAMS& args,
        _Inout_	const size_t* index,
        const efxc2CompilerParams::CompilerParams& params);

    bool parseNotSupportedOptions(
        _In_ const efxc2Utils::M_CMD_PARAMS& args,
        _In_ const size_t* index);

    constexpr auto NOT_SUPPORTED_LENGTH = 8;
    #ifdef _WIN32
    const std::array <const std::wstring, NOT_SUPPORTED_LENGTH>g_NotSupportedArgs = { {
    #else
    const std::array <const std::string, NOT_SUPPORTED_LENGTH>g_NotSupportedArgs = { {
    #endif
        M_AT_SYMBOL,
        M_COMPRESS,
        M_DECOMPRESS,
        M_DUMPBIN,
        M_FX,
        M_GETPRIVATE,
        M_MATCHUAVS,
        M_MERGEUAVS
    } };

 }
#endif /*EFXC2CMDS_H_INCLUDE*/