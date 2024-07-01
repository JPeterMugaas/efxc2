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
	void FindNOLOGO(const efxc2Utils::M_CMD_PARAMS& args, efxc2CompilerParams::CompilerParams& params);
	void FindDebug(const efxc2Utils::M_CMD_PARAMS& args, efxc2CompilerParams::CompilerParams& params);
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

	/* Parameter constants since we need both
	   Unicode and ASCII versions depending
	   upon platform.*/

#ifdef _WIN32
	constexpr const wchar_t* M_AT_SYMBOL = L"@";
	constexpr const wchar_t* M_QUESTION_MARK = L"?";
	constexpr const wchar_t* M_ALL_RESOURCES_BOUND = L"all_resources_bound";
	constexpr const wchar_t* M_CC = L"Cc";
	constexpr const wchar_t* M_COMPRESS = L"compress";
	constexpr const wchar_t* M_D = L"D";
	constexpr const wchar_t* M_DEBUG = L"debug";
	constexpr const wchar_t* M_DECOMPRESS = L"decompress";
	constexpr const wchar_t* M_DUMPBIN = L"dumpbin";
	/*This has to be M_E_ instead of M_E as that conflicts with a constant in math.h*/
	constexpr const wchar_t* M_E_ = L"E";
	constexpr const wchar_t* M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES = L"enable_unbounded_descriptor_tables";
	constexpr const wchar_t* M_FC = L"Fc";
	constexpr const wchar_t* M_FD = L"Fd";
	constexpr const wchar_t* M_FE = L"Fe";
	constexpr const wchar_t* M_FH = L"Fh";
	constexpr const wchar_t* M_FL = L"Fl";
	constexpr const wchar_t* M_FO = L"Fo";
	constexpr const wchar_t* M_FORCE_ROOTSIG_VER = L"force_rootsig_ver";
	constexpr const wchar_t* M_FX = L"Fx";
	constexpr const wchar_t* M_GCH = L"Gch";
	constexpr const wchar_t* M_GDP = L"Gdp";
	constexpr const wchar_t* M_GEC = L"Gec";
	constexpr const wchar_t* M_GES = L"Ges";
	constexpr const wchar_t* M_GETPRIVATE = L"getprivate";
	constexpr const wchar_t* M_GFA = L"Gfa";
	constexpr const wchar_t* M_GIS = L"Gis";
	constexpr const wchar_t* M_GPP = L"Gpp";
	constexpr const wchar_t* M_HELP = L"help";
	constexpr const wchar_t* M_I = L"I";
	constexpr const wchar_t* M_LX = L"Lx";
	constexpr const wchar_t* M_MATCHUAVS = L"matchUAVs";
	constexpr const wchar_t* M_MERGEUAVS = L"mergeUAVs";
	constexpr const wchar_t* M_NI = L"Ni";
	constexpr const wchar_t* M_NO = L"No";
	constexpr const wchar_t* M_NOLOGO = L"nologo";
	constexpr const wchar_t* M_O0 = L"O0";
	constexpr const wchar_t* M_O1 = L"O1";
	constexpr const wchar_t* M_O2 = L"O2";
	constexpr const wchar_t* M_O3 = L"O3";
	constexpr const wchar_t* M_OD = L"Od";
	constexpr const wchar_t* M_OP = L"Op";
	constexpr const wchar_t* M_P = L"P";
	constexpr const wchar_t* M_QSTRIP_DEBUG = L"Qstrip_debug";
	constexpr const wchar_t* M_QSTRIP_PRIV = L"Qstrip_priv";
	constexpr const wchar_t* M_QSTRIP_REFLECT = L"Qstrip_reflect";
	constexpr const wchar_t* M_QSTRIP_ROOTSIGNATURE = L"Qstrip_rootsignature";
	constexpr const wchar_t* M_RES_MAY_ALIAS = L"res_may_alias";
	constexpr const wchar_t* M_SETPRIVATE = L"setprivate";
	constexpr const wchar_t* M_SHTEMPLATE = L"shtemplate";
	constexpr const wchar_t* M_T = L"T";
	constexpr const wchar_t* M_VD = L"Vd";
	constexpr const wchar_t* M_VERSION = L"version";
	constexpr const wchar_t* M_VI = L"Vi";
	constexpr const wchar_t* M_VN = L"Vn";
	constexpr const wchar_t* M_WX = L"WX";
	constexpr const wchar_t* M_ZI = L"Zi";
	constexpr const wchar_t* M_ZPC = L"Zpc";
	constexpr const wchar_t* M_ZPR = L"Zpr";
	constexpr const wchar_t* M_ZSB = L"Zsb";
	constexpr const wchar_t* M_ZSS = L"Zss";
#else  /* _WIN32 */
	constexpr const char* M_AT_SYMBOL = "@";
	constexpr const char* M_QUESTION_MARK = "?";
	constexpr const char* M_ALL_RESOURCES_BOUND = "all_resources_bound";
	constexpr const char* M_CC = "Cc";
	constexpr const char* M_COMPRESS = "compress";
	constexpr const char* M_D = "D";
	constexpr const char* M_DEBUG = "debug";
	constexpr const char* M_DECOMPRESS = "decompress";
	constexpr const char* M_DUMPBIN = "dumpbin";
	constexpr const char* M_E = "E";
	constexpr const char* M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES = "enable_unbounded_descriptor_tables";
	constexpr const char* M_FC = "Fc";
	constexpr const char* M_FD = "Fd";
	constexpr const char* M_FE = "Fe";
	constexpr const char* M_FH = "Fh";
	constexpr const char* M_FL = "Fl";
	constexpr const char* M_FO = "Fo";
	constexpr const char* M_FORCE_ROOTSIG_VER = "force_rootsig_ver";
	constexpr const char* M_FX = "Fx";
	constexpr const char* M_GCH = "Gch";
	constexpr const char* M_GDP = "Gdp";
	constexpr const char* M_GEC = "Gec";
	constexpr const char* M_GES = "Ges";
	constexpr const char* M_GETPRIVATE = "getprivate";
	constexpr const char* M_GFA = "Gfa";
	constexpr const char* M_GIS = "Gis";
	constexpr const char* M_GPP = "Gpp";
	constexpr const char* M_HELP = "help";
	constexpr const char* M_I = "I";
	constexpr const char* M_LX = "Lx";
	constexpr const char* M_MATCHUAVS = "matchUAVs";
	constexpr const char* M_MERGEUAVS = "mergeUAVs";
	constexpr const char* M_NI = "Ni";
	constexpr const char* M_NO = "No";
	constexpr const char* M_NOLOGO = "nologo";
	constexpr const char* M_O0 = "O0";
	constexpr const char* M_O1 = "O1";
	constexpr const char* M_O2 = "O2";
	constexpr const char* M_O3 = "O3";
	constexpr const char* M_OD = "Od";
	constexpr const char* M_OP = "Op";
	constexpr const char* M_P = "P";
	constexpr const char* M_QSTRIP_DEBUG = "Qstrip_debug";
	constexpr const char* M_QSTRIP_PRIV = "Qstrip_priv";
	constexpr const char* M_QSTRIP_REFLECT = "Qstrip_reflect";
	constexpr const char* M_QSTRIP_ROOTSIGNATURE = "Qstrip_rootsignature";
	constexpr const char* M_RES_MAY_ALIAS = "res_may_alias";
	constexpr const char* M_SETPRIVATE = "setprivate";
	constexpr const char* M_SHTEMPLATE = "shtemplate";
	constexpr const char* M_T = "T";
	constexpr const char* M_VD = "Vd";
	constexpr const char* M_VERSION = "version";
	constexpr const char* M_VI = "Vi";
	constexpr const char* M_VN = "Vn";
	constexpr const char* M_WX = "WX";
	constexpr const char* M_ZI = "Zi";
	constexpr const char* M_ZPC = "Zpc";
	constexpr const char* M_ZPR = "Zpr";
	constexpr const char* M_ZSB = "Zsb";
	constexpr const char* M_ZSS = "Zss";
#endif /* _WIN32 */

	/*flag var names*/
constexpr auto M_SFLAGS_VAR = "sflags";
constexpr auto M_EFLAGS_VAR = "eflags";
constexpr auto M_STRIP_FLAGS_VAR = "strip_flags";
constexpr auto M_DISASM_FLAGS_VAR = "disassembly_flags";

	/*Parameter Descriptions*/
#ifdef _WIN32
constexpr auto M_E_DESCR = L"Entry Point";
constexpr auto M_FC_DESCR = L"Assembly Code";
constexpr auto M_FD_DESCR = L".PDB";
constexpr auto M_FH_DESCR = L"Header File";
constexpr auto M_FO_DESCR = L"Object File";
constexpr auto M_I_DESCR = L"Include Dir";
constexpr auto M_P_DESCR = L"Preprocessed Output";
constexpr auto M_SETPRIVATE_DESCR = L"Private Data";
constexpr auto M_T_DESCR = L"Shader Model/Profile";
#else
constexpr auto M_E_DESCR = "Entry Point";
constexpr auto M_FC_DESCR = "Assembly Code";
constexpr auto M_FD_DESCR = ".PDB";
constexpr auto M_FH_DESCR = "Header File";
constexpr auto M_FO_DESCR = "Object File";
constexpr auto M_I_DESCR = "Include Dir";
constexpr auto M_P_DESCR = "Preprocessor Output";
constexpr auto M_SETPRIVATE_DESCR = "Private Data";
constexpr auto M_T_DESCR = "Shader Model/Profile";
#endif

	using gCompilerFilep = void(efxc2CompilerParams::CompilerParams&, efxc2Files::Files&, const efxc2Utils::M_STRING_VIEW);
	struct CompileFileEntry {
		const efxc2Utils::M_STRING Param;
		//we need to mark this as will not serialize.
		gCompilerFilep* method = nullptr; //-V122_NOPTR
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

	void print_flag_parameter(const efxc2CompilerParams::CompilerParams& ComParams,
		const std::string_view cmdParam,
		const std::string_view AFlagsVar,
		const std::string_view AflagName);

	void print_string_parameter(_In_ const efxc2CompilerParams::CompilerParams& ComParams,
		const efxc2Utils::M_STRING_VIEW cmdParam,
		const efxc2Utils::M_STRING_VIEW ADesc,
		_In_ const efxc2Utils::M_STRING_VIEW fileName);

	using gCompilerp = void(efxc2CompilerParams::CompilerParams&);
	struct CompilerOnlyEntry {
		const efxc2Utils::M_STRING Param;
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