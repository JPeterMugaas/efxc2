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

void option_ignored(_In_ const M_STRING_VIEW& Opt, _In_ const CompilerParams& params) {
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"Option {} ignored", Opt);
#else
        std::cout << std::format("Option {} ignored", Opt);
#endif
    }
    return;
}

void parseInputFile(_In_ const M_STRING_VIEW& parameter, CompilerParams& params, Files& files) {
#ifdef _WIN32
    std::wstring inputFile = L"";
    std::string c_inputFile = "";
#else
    std::string inputFile = "";
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif
    if (files.get_inputFile().empty()) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
        if (params.get_verbose()) {
            std::cout << "Parse input file name\n";
        }
        inputFile = parameter;
#ifdef _WIN32
        FixupFileName(inputFile);
        c_inputFile = utf8_encode(inputFile);
        params.set_inputFile(c_inputFile);
#else  /* _WIN32 */
        params.set_inputFile(inputFile);
#endif /* _WIN32 */
        files.set_inputFile(inputFile);
        if (params.get_verbose()) {
#ifdef _WIN32
            std::wcout << std::format(L"input file: {}\n", inputFile);
#else
            std::cout << std::format("input file: {}\n", inputFile);
#endif
        }
        files.LoadInputFile(params);
    }
    else {
        print_usage_toomany();
    }
}

void cmd_all_resources_bound(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -all_resources_bound sflags | D3DCOMPILE_ALL_RESOURCES_BOUND\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
    params.set_sflags(sflags);
    return;
}

void cmd_Cc(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Cc | D3D_DISASM_ENABLE_COLOR_CODED\n";
    }
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_COLOR_CODE;
    params.set_disassembly_flags(disassembly_flags);
    return;
}

void cmd_D(CompilerParams& params, _In_ const M_STRING& _defineOption) {
#ifdef _WIN32
    std::string defineOption = utf8_encode(_defineOption);
#else
    std::string defineOption = _defineOption;
#endif
    params.add_define(defineOption);
    if (params.get_verbose()) {
        std::cout << std::format("option -D with arg {}\n", defineOption);
    }
}

void cmd_E(CompilerParams& params, _In_ const M_STRING_VIEW& _entryPoint) {
#ifdef _WIN32
    std::string entryPoint = utf8_encode(_entryPoint);
#else
    std::string entryPoint = _entryPoint;
#endif
    params.set_entryPoint(entryPoint);
    if (params.get_verbose()) {
        std::cout << std::format("option -E (Entry Point) with arg {}'\n", entryPoint);
    }
    return;
}

void cmd_enable_unbounded_descriptor_tables(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -enable_unbounded_descriptor_tables sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
    params.set_sflags(sflags);
    return;
}

void cmd_Fc(CompilerParams& params, Files& files, _In_ const M_STRING& assemblyCodeFile) {
    files.set_DisassemblyFile(assemblyCodeFile);
    UINT cmd = params.get_commands();
    cmd = cmd | CMD_WRITE_ASSEMBLY_CODE;
    params.set_commands(cmd);
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -Fc (assembly code) with arg {}\n", assemblyCodeFile);
#else
        std::cout << std::format("option -Fc (assembly code) with arg {}\n", assemblyCodeFile);
#endif
    }
    return;
}

void cmd_Fd(CompilerParams& params, Files& files, _In_ const M_STRING& pdbFile) {
#ifdef _WIN32
    files.set_c_pdbFile(utf8_encode(pdbFile));
#endif /* _WIN32 */
    files.set_pdbFile(pdbFile);

    UINT cmd = params.get_commands();
    cmd = cmd | CMD_WRITE_PDB_FILE;
    params.set_commands(cmd);

    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << format(L"option -Fd (.PDB) with arg {}\n", pdbFile);
#else
        std::cout << format("option -Fd (.PDB) with arg {}\n", pdbFile);
#endif
    }
    return;
}

void cmd_Fh(CompilerParams& params, Files& files, _In_ const M_STRING& outputFile) {
    files.set_IncludeFile(outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | CMD_WRITE_HEADER;
    params.set_commands(cmd);
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -Fh (Output File) with arg {}\n", outputFile);
#else
        std::cout << std::format("option -Fh (Output File) with arg {}\n", outputFile);
#endif
    }
    return;
}

void cmd_Fo(CompilerParams& params, Files& files, _In_ const M_STRING& outputFile) {
    files.set_ObjectFile(outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | CMD_WRITE_OBJECT;
    params.set_commands(cmd);
    if (params.get_verbose()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -FO (Output File) with arg {}\n", outputFile);
#else
        std::cout << std::format("option -FO (Output File) with arg {}\n", outputFile);
#endif
    }
    return;
}

void cmd_Gch(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Gch eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT";
    }
    UINT eflags = params.get_eflags();
    eflags = eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
    params.set_eflags(eflags);
    return;
}

void cmd_Gdp(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Gdp eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS";
    }
    UINT eflags = params.get_eflags();
    eflags = eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
    params.set_eflags(eflags);
    return;
}

void cmd_Gec(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Gec sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
    params.set_sflags(sflags);
    return;
}

void cmd_Ges(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Ges sflags | D3DCOMPILE_ENABLE_STRICTNESS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_STRICTNESS;
    params.set_sflags(sflags);
    return;
}

void cmd_Gfa(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Gfa sflags | D3DCOMPILE_AVOID_FLOW_CONTROL\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
    params.set_sflags(sflags);
    return;
}

void cmd_Gis(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Gis sflags | D3DCOMPILE_IEEE_STRICTNESS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_IEEE_STRICTNESS;
    params.set_sflags(sflags);
    return;
}

void cmd_Gpp(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Gpp sflags | D3DCOMPILE_PARTIAL_PRECISION\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PARTIAL_PRECISION;
    params.set_sflags(sflags);
    return;
}

void cmd_Lx(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Lx - output hexidecimal literals\n";
    }
    params.set_outputHex(TRUE);
    return;
}

void cmd_Ni(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Cc | D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING\n";
    }
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING;
    params.set_disassembly_flags(disassembly_flags);
    return;
}

void cmd_No(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Cc | D3D_DISASM_ENABLE_INSTRUCTION_OFFSET\n";
    }
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_OFFSET;
    params.set_disassembly_flags(disassembly_flags);
    return;
}

void cmd_O0(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -O0 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
    params.set_sflags(sflags);
    return;
}

void cmd_O1(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
    params.set_sflags(sflags);
    return;
}

void cmd_O2(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
    params.set_sflags(sflags);
    return;
}

void cmd_O3(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
    params.set_sflags(sflags);
    return;
}

void cmd_Od(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Od sflags | D3DCOMPILE_SKIP_OPTIMIZATION\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_SKIP_OPTIMIZATION;
    params.set_sflags(sflags);
    return;
}

void cmd_Op(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Op sflags | D3DCOMPILE_NO_PRESHADER\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_NO_PRESHADER;
    params.set_sflags(sflags);
    return;
}

void cmd_Qstrip_debug(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Qstrip_debug strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO;
    params.set_strip_flags(strip_flags);
    return;
}

void cmd_Qstrip_priv(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Qstrip_priv strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA;
    params.set_strip_flags(strip_flags);
    return;
}

void cmd_Qstrip_reflect(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Qstrip_reflect strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA;
    params.set_strip_flags(strip_flags);
    return;
}

void cmd_Qstrip_rootsignature(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Qstrip_rootsignature strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATUR\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE;
    params.set_strip_flags(strip_flags);
    return;
}

void cmd_res_may_alias(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -res_may_alias sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
    params.set_sflags(sflags);
    return;
}

void cmd_T(CompilerParams& params, _In_ const M_STRING_VIEW& _model) {
#ifdef _WIN32
    std::string model = utf8_encode(_model);
#else
    std::string model = _model;
#endif
    if (params.get_verbose()) {
        std::cout << std::format("option -T (Shader Model/Profile) with arg {}\n", model);
    }
    params.set_model(model);
    return;
}

void cmd_Vd(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Vd sflags | D3DCOMPILE_SKIP_VALIDATION\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_SKIP_VALIDATION;
    params.set_sflags(sflags);
    return;
}

void cmd_Vn(CompilerParams& params, _In_ const M_STRING_VIEW& _variableName) {
#ifdef _WIN32
    std::string variableName = utf8_encode(_variableName);
#else
    std::string variableName = _variableName;
#endif
    if (params.get_verbose()) {
        std::cout << std::format("option -Vn (Variable Name) with arg {}'\n", variableName);
    }
    params.set_variableName(variableName);
    return;
}

void cmd_WX(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -WX sflags |  D3DCOMPILE_WARNINGS_ARE_ERRORS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
    params.set_sflags(sflags);
    return;
}

void cmd_Zi(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Zi sflags | D3DCOMPILE_DEBUG\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG;
    params.set_sflags(sflags);
    return;
}

void cmd_Zpc(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Zpc sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
    params.set_sflags(sflags);
    return;
}

void cmd_Zpr(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Zpr sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
    params.set_sflags(sflags);
    return;
}

void cmd_Zsb(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Zsb sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
    params.set_sflags(sflags);
    return;
}

void cmd_Zss(CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -Zss sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
    params.set_sflags(sflags);
    return;
}

bool  parseCompilerOnlyCall(
    _In_ const M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    CompilerParams& params) {
    if (!index || *index >= args.size()) {
        return false;
    }
    const M_STRING argument = args[*index];
    size_t arg_idx = 0;
    if (argument[0] == '-' || argument[0] == '/') {
        arg_idx++;
        if (argument[arg_idx] == '-') {
            arg_idx++;
        }
    }
    else {
        return false;
    }

    for (int i = 0; i < COMPILER_ONLY_ENTRIES_LENGTH; i++) {
        if (argument.compare(arg_idx, g_CompilerOnlyCall[i].Param.size(), g_CompilerOnlyCall[i].Param) == 0) {
            auto ptr = (gCompilerp*)g_CompilerOnlyCall[i].method;
            ptr(params);
            *index += 1;
            return true;
        }
    }
    return false;
}

bool parseCompilerFileCall(
    _In_ const M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    CompilerParams& params,
    Files& files) {
    if (!index || *index >= args.size()) {
        return false;
    }
#ifdef _WIN32
    std::wstring argumentOption = L"";
#else
    std::string argumentOption = "";
#endif
    if (!index || *index >= args.size()) {
       return false;
    }

    M_STRING argument = args[*index];
    size_t arg_idx = 0;
    if (argument[0] == '-' || argument[0] == '/') {
        arg_idx++;
        if (argument[arg_idx] == '-') {
           arg_idx++;
        }
    }
    else {
        return false;
    }

    for (size_t i = 0; i < COMPILER_FILE_ENTRIES_LENGTH; i++) {
        if (argument.compare(arg_idx, g_CompilerFileCall[i].Param.size(), g_CompilerFileCall[i].Param) != 0) {
            continue;
        }
        arg_idx += g_CompilerFileCall[i].Param.size();
        if (arg_idx >= argument.size()) {
            *index += 1;
            if (*index >= args.size()) {
#ifdef _WIN32
                std::wcerr << std::format(L"Error: missing required argument for option {}\n", g_CompilerFileCall[i].Param);
#else
                std::cerr << std::format("Error: missing required argument for option {}\n", g_CompilerFileCall[i].Param );
#endif
                return false;
            }
            argumentOption = args[*index];
        }
        else {
            argumentOption = argument.substr(arg_idx, argument.size());
        }
        auto ptr = (gCompilerFilep*)g_CompilerFileCall[i].method;
        ptr(params, files, argumentOption);
        *index += 1;
        return true;
    }
    return false;
}

bool parseIgnoredOpts(
    _In_ const M_CMD_PARAMS& args,
    _Inout_	const size_t* index,
    const CompilerParams& params) {
    if (!index || *index >= args.size()) {
        return false;
    }
#ifdef _WIN32
    const std::wstring_view argument = args[*index];
#else  /* _WIN32 */
    const std::string_view argument = args[*index];
#endif /* _WIN32 */
    size_t arg_idx = 0;
    if (argument[0] == '-' || argument[0] == '/') {
        arg_idx++;
        if (argument[arg_idx] == '-') {
            arg_idx++;
        }
    }
    else {
        return false;
    }
    for (int i = 0; i < IGNORED_OPTS_LENGTH; i++) {
        if (argument.compare(arg_idx, std::string::npos, g_IgnoredOpts[i]) == 0) {
            option_ignored(argument, params);
            return true;
        }
    }
    return false;
}

bool parseNotSupportedOpts(
    _In_ const M_CMD_PARAMS& args,
    _In_ const size_t* index) {
    if (!index || *index >= args.size()) {
        return false;
    }
#ifdef _WIN32
    const std::wstring_view argument = args[*index];
#else  /* _WIN32 */
    const std::string_view argument = args[*index];
#endif /* _WIN32 */
    size_t arg_idx = 0;
    if (argument[0] == '-' || argument[0] == '/') {
        arg_idx++;
        if (argument[arg_idx] == '-') {
            arg_idx++;
        }
    }
    else {
        return false;
    }
    for (int i = 0; i < NOT_SUPPORTED_LENGTH; i++) {
        if (argument.compare(arg_idx, std::string::npos, g_NotSupportedArgs[i]) == 0) {
#ifdef _WIN32
            std::wcerr << std::format(L"option -{} not supported", argument);
#else
            std::cerr << std::format("option -{} not supported", argument);
#endif
            print_unsupported_arg_help();
            return true;
        }
    }
    return false;
}
