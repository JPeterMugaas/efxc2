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
#include "efxc2Exception.h"

void efxc2Cmds::FindNOLOGO(const efxc2Utils::M_CMD_PARAMS& args, efxc2CompilerParams::CompilerParams& params) {
    /*first scan specifically for the nologo argument so no output
    is given regardless of parameter order*/
    size_t index = 0;
    while (index < args.size()) {
        /* Detect the end of the options. */
        if (efxc2Utils::parseOpt(efxc2Cmds::M_NOLOGO, args, &index, nullptr)) {
            params.set_verbose(false);
            break;
        }
        else {
            index++;
        }
    }
    return;
}

void efxc2Cmds::FindDebug(const efxc2Utils::M_CMD_PARAMS& args, efxc2CompilerParams::CompilerParams& params) {
    /* scan for debug parameter*/
    size_t index = 0;
    while (index < args.size()) {
        /* Detect the end of the options. */
        if (efxc2Utils::parseOpt(efxc2Cmds::M_DEBUG, args, &index, nullptr)) {
            params.set_debug(true);
            break;
        }
        else {
            index++;
        }
    }
    return;
}

void efxc2Cmds::print_string_parameter(_In_ const efxc2CompilerParams::CompilerParams& ComParams,
    _In_ const efxc2Utils::M_STRING_VIEW cmdParam,
    _In_ const efxc2Utils::M_STRING_VIEW ADesc,
    _In_ const efxc2Utils::M_STRING_VIEW fileName) {
    if (ComParams.get_verbose() && ComParams.get_debug()) {
#ifdef _WIN32
        std::wcout << M_FORMAT(L"option -{} ({}) with arg {}\n", cmdParam, ADesc, fileName);
#else
        std::cout << M_FORMAT("option -{} ({}) with arg {}\n", cmdParam, ADesc, fileName);
#endif
    }
}

void efxc2Cmds::print_flag_parameter(_In_ const efxc2CompilerParams::CompilerParams& ComParams,
    _In_ const std::string_view cmdParam,
    _In_ const std::string_view AFlagsVar,
    _In_ const std::string_view AflagName) {
    if (ComParams.get_verbose() && ComParams.get_debug()) {
        std::cout << M_FORMAT("option -{} {} | {}\n", cmdParam, AFlagsVar, AflagName);
    }
}

void efxc2Cmds::option_ignored(_In_ const efxc2Utils::M_STRING_VIEW Opt, _In_ const efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << M_FORMAT(L"Option {} ignored", Opt);
#else
        std::cout << M_FORMAT("Option {} ignored", Opt);
#endif
    }
    return;
}

void efxc2Cmds::parseInputFile(_In_ const efxc2Utils::M_STRING_VIEW parameter, efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files) {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif
    if (files.get_inputFile().empty()) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
        if (params.get_verbose() && params.get_debug()) {
            std::cout << "Parse input file name\n";
        }
        efxc2Utils::M_STRING inputFile = parameter.data();
#ifdef _WIN32
        std::string c_inputFile = "";
#endif
#ifdef _WIN32
        efxc2Utils::FixupFileName(inputFile);
        c_inputFile = efxc2Utils::wstring_to_utf8(inputFile);
        params.set_inputFile(c_inputFile);
#endif /* _WIN32 */
        files.set_inputFile(inputFile);
        if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
            std::wcout << M_FORMAT(L"input file: {}\n", inputFile);
#else
            std::cout << M_FORMAT("input file: {}\n", inputFile);
#endif
        }
        files.LoadInputFile(params);
    }
    else {
        efxc2Utils::print_usage_toomany();
        throw efxc2Exception::TooManyParameters();
    }
}

void efxc2Cmds::cmd_all_resources_bound(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
    params.set_sflags(sflags);
    print_flag_parameter(params, "all_resources_bound", M_SFLAGS_VAR, "D3DCOMPILE_ALL_RESOURCES_BOUND");
    return;
}

void efxc2Cmds::cmd_Cc(efxc2CompilerParams::CompilerParams& params) {
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_COLOR_CODE;
    params.set_disassembly_flags(disassembly_flags);
    print_flag_parameter(params, "Cc", M_DISASM_FLAGS_VAR, "D3D_DISASM_ENABLE_COLOR_CODE");
    return;
}

void efxc2Cmds::cmd_D(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _defineOption) {
#ifdef _WIN32
    std::string defineOption = efxc2Utils::wstring_to_utf8({ _defineOption.data(), _defineOption.size() });
#else
    std::string defineOption = { _defineOption.data(), _defineOption.size() };
#endif
    params.add_define(defineOption);
    print_string_parameter(params, M_D, M_FH_DESCR, _defineOption);
}

void efxc2Cmds::cmd_E(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _entryPoint) {
#ifdef _WIN32
    std::string entryPoint = efxc2Utils::wstring_to_utf8({ _entryPoint.data(), _entryPoint.size() });
#else
    std::string entryPoint = { _entryPoint.data(), _entryPoint.size() };
#endif
    params.set_entryPoint(entryPoint);
    print_string_parameter(params, M_E_, M_E_DESCR, _entryPoint);
    return;
}

void efxc2Cmds::cmd_enable_unbounded_descriptor_tables(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
    params.set_sflags(sflags);
    print_flag_parameter(params, "enable_unbounded_descriptor_tables", M_SFLAGS_VAR, "D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLE");
    return;
}

void efxc2Cmds::cmd_Fc(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW assemblyCodeFile) {
    files.set_DisassemblyFile(assemblyCodeFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_ASSEMBLY_CODE;
    params.set_commands(cmd);
    print_string_parameter(params, M_FC, M_FC_DESCR, assemblyCodeFile);
    return;
}

void efxc2Cmds::cmd_Fd(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW pdbFile) {
    files.set_pdbFile(pdbFile);

    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_PDB_FILE;
    params.set_commands(cmd);

    print_string_parameter(params, M_FD, M_FD_DESCR, pdbFile);
    return;
}

void efxc2Cmds::cmd_Fh(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile) {
    files.set_IncludeFile(outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_HEADER;
    params.set_commands(cmd);
    print_string_parameter(params, M_FH, M_FH_DESCR, outputFile);
    return;
}

void efxc2Cmds::cmd_Fo(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile) {
    files.set_ObjectFile(outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_OBJECT;
    params.set_commands(cmd);
    print_string_parameter(params, M_FO, M_FO_DESCR, outputFile);
    return;
}

void efxc2Cmds::cmd_Gch(efxc2CompilerParams::CompilerParams& params) {
    UINT eflags = params.get_eflags();
    eflags = eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
    params.set_eflags(eflags);
    print_flag_parameter(params, "Gch", M_EFLAGS_VAR, "D3DCOMPILE_EFFECT_CHILD_EFFECT");
    return;
}

void efxc2Cmds::cmd_Gdp(efxc2CompilerParams::CompilerParams& params) {
    UINT eflags = params.get_eflags();
    eflags = eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
    params.set_eflags(eflags);
    print_flag_parameter(params, "Gdp", M_EFLAGS_VAR, "D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS");
    return;
}

void efxc2Cmds::cmd_Gec(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Gec", M_SFLAGS_VAR, "D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY");
    return;
}

void efxc2Cmds::cmd_Ges(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_STRICTNESS;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Ges", M_SFLAGS_VAR, "D3DCOMPILE_ENABLE_STRICTNESS");
    return;
}

void efxc2Cmds::cmd_Gfa(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Gfa", M_SFLAGS_VAR, "D3DCOMPILE_AVOID_FLOW_CONTROL");
    return;
}

void efxc2Cmds::cmd_Gis(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_IEEE_STRICTNESS;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Gis", M_SFLAGS_VAR, "D3DCOMPILE_IEEE_STRICTNESS");
    return;
}

void efxc2Cmds::cmd_Gpp(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PARTIAL_PRECISION;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Gpp", M_SFLAGS_VAR, "D3DCOMPILE_PARTIAL_PRECISION");
    return;
}

void efxc2Cmds::cmd_I(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _includeDir) {
    params.get_includeDirs()->AddIncludeDir(_includeDir);
    print_string_parameter(params, M_I, M_FH_DESCR, _includeDir);
    return;
}

void efxc2Cmds::cmd_Lx(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Lx - output hexadecimal literals\n";
    }
    params.set_outputHex(TRUE);
    return;
}

void efxc2Cmds::cmd_Ni(efxc2CompilerParams::CompilerParams& params) {
     UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING;
    params.set_disassembly_flags(disassembly_flags);
    print_flag_parameter(params, "Ni", M_DISASM_FLAGS_VAR, "D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING");
    return;
}

void efxc2Cmds::cmd_No(efxc2CompilerParams::CompilerParams& params) {
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_OFFSET;
    params.set_disassembly_flags(disassembly_flags);
    print_flag_parameter(params, "No", M_DISASM_FLAGS_VAR, "D3D_DISASM_ENABLE_INSTRUCTION_OFFSET");
    return;
}

void efxc2Cmds::cmd_O0(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
    params.set_sflags(sflags);
    print_flag_parameter(params, "O0", M_SFLAGS_VAR, "D3DCOMPILE_OPTIMIZATION_LEVEL0");
    return;
}

void efxc2Cmds::cmd_O1(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
    params.set_sflags(sflags);
    print_flag_parameter(params, "O1", M_SFLAGS_VAR, "D3DCOMPILE_OPTIMIZATION_LEVEL1");
    return;
}

void efxc2Cmds::cmd_O2(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
    params.set_sflags(sflags);
    print_flag_parameter(params, "O2", M_SFLAGS_VAR, "D3DCOMPILE_OPTIMIZATION_LEVEL2");
    return;
}

void efxc2Cmds::cmd_O3(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
    params.set_sflags(sflags);
    print_flag_parameter(params, "O3", M_SFLAGS_VAR, "D3DCOMPILE_OPTIMIZATION_LEVEL3");
    return;
}

void efxc2Cmds::cmd_Od(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_SKIP_OPTIMIZATION;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Od", M_SFLAGS_VAR, "D3DCOMPILE_SKIP_OPTIMIZATION");
    return;
}

void efxc2Cmds::cmd_Op(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_NO_PRESHADER;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Op", M_SFLAGS_VAR, "D3DCOMPILE_NO_PRESHADER");
    return;
}

void efxc2Cmds::cmd_P(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile) {
    files.set_preprocessFile(outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_PREPROCESS_FILE;
    params.set_commands(cmd);
    print_string_parameter(params, M_P, M_P_DESCR, outputFile);
    return;
}

void efxc2Cmds::cmd_Qstrip_debug(efxc2CompilerParams::CompilerParams& params) {
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO;
    params.set_strip_flags(strip_flags);
    print_flag_parameter(params, "Qstrip_debug", M_STRIP_FLAGS_VAR, "D3DCOMPILER_STRIP_DEBUG_INFO");
    return;
}

void efxc2Cmds::cmd_Qstrip_priv(efxc2CompilerParams::CompilerParams& params) {
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA;
    params.set_strip_flags(strip_flags);
    print_flag_parameter(params, "Qstrip_priv", M_STRIP_FLAGS_VAR, "D3DCOMPILER_STRIP_PRIVATE_DATA");
    return;
}

void efxc2Cmds::cmd_Qstrip_reflect(efxc2CompilerParams::CompilerParams& params) {
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA;
    params.set_strip_flags(strip_flags);
    print_flag_parameter(params, "Qstrip_reflect", M_STRIP_FLAGS_VAR, "D3DCOMPILER_STRIP_REFLECTION_DATA");
    return;
}

void efxc2Cmds::cmd_Qstrip_rootsignature(efxc2CompilerParams::CompilerParams& params) {
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE;
    params.set_strip_flags(strip_flags);
    print_flag_parameter(params, "Qstrip_rootsignature", M_STRIP_FLAGS_VAR, "D3DCOMPILER_STRIP_ROOT_SIGNATURE");
    return;
}

void efxc2Cmds::cmd_res_may_alias(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
    params.set_sflags(sflags);
    print_flag_parameter(params, "res_may_alias", M_SFLAGS_VAR, "D3DCOMPILE_RESOURCES_MAY_ALIAS");
    return;
}

void efxc2Cmds::cmd_setprivate(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW inputfile) {
    files.set_privateDataFile(inputfile);
    files.LoadPrivateDataFile(params);
    print_string_parameter(params, M_SETPRIVATE, M_SETPRIVATE_DESCR, inputfile);
    return;
}

void efxc2Cmds::cmd_T(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _model) {
#ifdef _WIN32
    std::string model = efxc2Utils::wstring_to_utf8({ _model.data(), _model.size() });
#else
    std::string model = { _model.data(), _model.size() };
#endif
    params.set_model(model);
    print_string_parameter(params, M_T, M_T_DESCR, _model);
    return;
}

void efxc2Cmds::cmd_Vd(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_SKIP_VALIDATION;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Vd", M_SFLAGS_VAR, "D3DCOMPILE_SKIP_VALIDATION");
    return;
}

void efxc2Cmds::cmd_Vn(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _variableName) {
#ifdef _WIN32
    std::string variableName = efxc2Utils::wstring_to_utf8({ _variableName.data(), _variableName.size() });
#else
    std::string variableName = { _variableName.data(), _variableName.size() };
#endif
    params.set_variableName(variableName);
    print_string_parameter(params, M_VN, M_P_DESCR, _variableName);
    return;
}

void efxc2Cmds::cmd_WX(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
    params.set_sflags(sflags);
    print_flag_parameter(params, "WX", M_SFLAGS_VAR, "D3DCOMPILE_WARNINGS_ARE_ERRORS");
    return;
}

void efxc2Cmds::cmd_Zi(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Zi", M_SFLAGS_VAR, "D3DCOMPILE_DEBUG");
    return;
}

void efxc2Cmds::cmd_Zpc(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Zpc", M_SFLAGS_VAR, "D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR");
    return;
}

void efxc2Cmds::cmd_Zpr(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Zpr", M_SFLAGS_VAR, "D3DCOMPILE_PACK_MATRIX_ROW_MAJOR");
    return;
}

void efxc2Cmds::cmd_Zsb(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Zsb", M_SFLAGS_VAR, "D3DCOMPILE_DEBUG_NAME_FOR_BINARY");
    return;
}

void efxc2Cmds::cmd_Zss(efxc2CompilerParams::CompilerParams& params) {
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
    params.set_sflags(sflags);
    print_flag_parameter(params, "Zss", M_SFLAGS_VAR, "D3DCOMPILE_DEBUG_NAME_FOR_SOURCE");
    return;
}

bool  efxc2Cmds::parseCompilerOnlyCall(
    _In_ const efxc2Utils::M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    efxc2CompilerParams::CompilerParams& params) {
    bool terminate_routine = false;
    bool result = false;
    if (index == nullptr || *index >= args.size()) {
        terminate_routine = true;
    }
    efxc2Utils::M_STRING argument;
    size_t arg_idx = 0;
    if (terminate_routine == false) {
        /* Disable warning V1004 because we checked the pointer for nullptr.*/
        argument = args[*index];  //-V1004
        if (argument[0] == '-' || argument[0] == '/') {
            arg_idx++;
            if (argument[arg_idx] == '-') {
                arg_idx++;
            }
        }
        else {
            terminate_routine = true;
        }
    }
    if (terminate_routine == false) {
        for (CompilerOnlyEntry currentEntry : g_CompilerOnlyCall) {
            if (argument.compare(arg_idx, currentEntry.Param.size(), currentEntry.Param) == 0) {
                currentEntry.method(params);
                *index += 1;
                result = true;
                break;
            }
        }
    }
    return result;
}

bool efxc2Cmds::parseCompilerFileCall(
    _In_ const efxc2Utils::M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    efxc2CompilerParams::CompilerParams& params,
    efxc2Files::Files& files) {
    bool terminate_routine = false;
    bool result = false;

    efxc2Utils::M_STRING argumentOption = efxc2Utils::M_STRING_INIT;
    efxc2Utils::M_STRING_VIEW argument;
    size_t arg_idx = 0;
    if (index == nullptr || *index >= args.size()) {
        terminate_routine = true;
    }
    /* Disable warning V1004 because we checked the pointer for nullptr.*/
    if (terminate_routine == false) {
        argument = args[*index];  //-V1004
        if (argument[0] == '-' || argument[0] == '/') {
            arg_idx++;
            if (argument[arg_idx] == '-') {
                arg_idx++;
            }
        }
        else {
            terminate_routine = true;
        }
    }
    if (terminate_routine == false) {
        for (CompileFileEntry currentEntry : g_CompilerFileCall) {
            if (argument.compare(arg_idx, currentEntry.Param.size(), currentEntry.Param) != 0) { 
                continue;
            }
            arg_idx += currentEntry.Param.size(); 
            if (arg_idx >= argument.size()) {
                *index += 1;
                efxc2Utils::checkForMissingArg(currentEntry.Param, *index, args);
                argumentOption = args[*index];
            }
            else {
                argumentOption = argument.substr(arg_idx, argument.size());
            }
            currentEntry.method(params, files, argumentOption);
            *index += 1;
            result = true;
        }
    }
    return result;
}

bool efxc2Cmds::parseIgnoredOptions(
    _In_ const efxc2Utils::M_CMD_PARAMS& args,
    _Inout_	const size_t* index,
    const efxc2CompilerParams::CompilerParams& params) {
    /*supress warning /V1004 because we check the pointer for nullptr before using *index.*/
    bool terminate_routine = false;
    bool result = false;
    if ((index == nullptr) || (*index >= args.size())) {
        terminate_routine = true;
    }
    if (terminate_routine == false) {
        const efxc2Utils::M_STRING_VIEW argument = args[*index]; //-V1004
        size_t arg_idx = 0;
        if (argument[0] == '-' || argument[0] == '/') {
            arg_idx++;
            if (argument[arg_idx] == '-') {
                arg_idx++;
            }
        }
        else {
            terminate_routine = true;
        }
        if (terminate_routine == false) {
            if (efxc2Utils::M_STRING_VIEW toFind = argument.substr(arg_idx, std::string::npos); 
#ifdef RANGES_SUPPORTED
                std::ranges::find(g_IgnoredOptions, toFind) != g_IgnoredOptions.end()) {
#else
                std::find(g_IgnoredOptions.begin(), g_IgnoredOptions.end(), toFind) != g_IgnoredOptions.end()) {
#endif
                option_ignored(argument, params);
                result = true;
            }
        }
    }
    return result;
}

bool efxc2Cmds::parseNotSupportedOptions(
    _In_ const efxc2Utils::M_CMD_PARAMS& args,
    _In_ const size_t* index) {
    /*supress warning /V1004 because we check the pointer for nullptr before using *index.*/
    bool terminate_routine = false;
    bool result = false;
    if (!index || *index >= args.size()) {
        terminate_routine = true;
    }
    if (terminate_routine == false) {
        const efxc2Utils::M_STRING_VIEW argument = args[*index]; //-V1004
        size_t arg_idx = 0;
        if (argument[0] == '-' || argument[0] == '/') {
            arg_idx++;
            if (argument[arg_idx] == '-') {
                arg_idx++;
            }
        }
        else {
            terminate_routine = true;
        }
        if (terminate_routine == false) {
            if (efxc2Utils::M_STRING_VIEW toFind = argument.substr(arg_idx, std::string::npos);
#ifdef RANGES_SUPPORTED 
                std::ranges::find(g_NotSupportedArgs, toFind) != g_NotSupportedArgs.end()) {
#else
                std::find(g_NotSupportedArgs.begin(), g_NotSupportedArgs.end(), toFind) != g_NotSupportedArgs.end()) {
#endif
                efxc2Console::Console console = efxc2Console::console; 
                console.std_err_pink();
#ifdef _WIN32
                std::wcerr << M_FORMAT(L"option -{} not supported", argument);
#else
                std::cerr << M_FORMAT("option -{} not supported", argument);
#endif
                std::cout << efxc2Utils::print_unsupported_arg_help();
                console.std_err_reset();
                result = true;
            }
        }
    }
    return result;
}