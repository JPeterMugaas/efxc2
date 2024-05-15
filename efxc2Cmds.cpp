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

void efxc2Cmds::option_ignored(_In_ const efxc2Utils::M_STRING_VIEW Opt, _In_ const efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"Option {} ignored", Opt);
#else
        std::cout << std::format("Option {} ignored", Opt);
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
        efxc2Utils::M_STRING inputFile = efxc2Utils::M_STRING_INIT;
#ifdef _WIN32
        std::string c_inputFile = "";
#endif
        inputFile = parameter;
#ifdef _WIN32
        efxc2Utils::FixupFileName(inputFile);
        c_inputFile = efxc2Utils::wstring_to_utf8(inputFile);
        params.set_inputFile(c_inputFile);
#endif /* _WIN32 */
        files.set_inputFile(inputFile);
        if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
            std::wcout << std::format(L"input file: {}\n", inputFile);
#else
            std::cout << std::format("input file: {}\n", inputFile);
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
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -all_resources_bound sflags | D3DCOMPILE_ALL_RESOURCES_BOUND\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Cc(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Cc | D3D_DISASM_ENABLE_COLOR_CODED\n";
    }
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_COLOR_CODE;
    params.set_disassembly_flags(disassembly_flags);
    return;
}

void efxc2Cmds::cmd_D(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _defineOption) {
#ifdef _WIN32
    std::string defineOption = efxc2Utils::wstring_to_utf8({ _defineOption.data(), _defineOption.size() });
#else
    std::string defineOption = { _defineOption.data(), _defineOption.size()};
#endif
    params.add_define(defineOption);
    if (params.get_verbose() && params.get_debug()) {
        std::cout << std::format("option -D with arg {}\n", defineOption);
    }
}

void efxc2Cmds::cmd_E(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _entryPoint) {
#ifdef _WIN32
    std::string entryPoint = efxc2Utils::wstring_to_utf8({ _entryPoint.data(), _entryPoint.size()});
#else
    std::string entryPoint = { _entryPoint.data(), _entryPoint.size()};
#endif
    params.set_entryPoint(entryPoint);
    if (params.get_verbose() && params.get_debug()) {
        std::cout << std::format("option -E (Entry Point) with arg {}'\n", entryPoint);
    }
    return;
}

void efxc2Cmds::cmd_enable_unbounded_descriptor_tables(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose()) {
        std::cout << "option -enable_unbounded_descriptor_tables sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Fc(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW assemblyCodeFile) {
    files.set_DisassemblyFile(assemblyCodeFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_ASSEMBLY_CODE;
    params.set_commands(cmd);
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -Fc (assembly code) with arg {}\n", assemblyCodeFile);
#else
        std::cout << std::format("option -Fc (assembly code) with arg {}\n", assemblyCodeFile);
#endif
    }
    return;
}

void efxc2Cmds::cmd_Fd(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW pdbFile) {
    files.set_pdbFile(pdbFile);

    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_PDB_FILE;
    params.set_commands(cmd);

    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -Fd (.PDB) with arg {}\n", pdbFile);
#else
        std::cout << std::format("option -Fd (.PDB) with arg {}\n", pdbFile);
#endif
    }
    return;
}

void efxc2Cmds::cmd_Fh(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile) {
    files.set_IncludeFile(outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_HEADER;
    params.set_commands(cmd);
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -Fh (Output File) with arg {}\n", outputFile);
#else
        std::cout << std::format("option -Fh (Output File) with arg {}\n", outputFile);
#endif
    }
    return;
}

void efxc2Cmds::cmd_Fo(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile) {
    files.set_ObjectFile(outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_WRITE_OBJECT;
    params.set_commands(cmd);
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -FO (Output File) with arg {}\n", outputFile);
#else
        std::cout << std::format("option -FO (Output File) with arg {}\n", outputFile);
#endif
    }
    return;
}

void efxc2Cmds::cmd_Gch(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Gch eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT";
    }
    UINT eflags = params.get_eflags();
    eflags = eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
    params.set_eflags(eflags);
    return;
}

void efxc2Cmds::cmd_Gdp(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Gdp eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS";
    }
    UINT eflags = params.get_eflags();
    eflags = eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
    params.set_eflags(eflags);
    return;
}

void efxc2Cmds::cmd_Gec(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Gec sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Ges(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Ges sflags | D3DCOMPILE_ENABLE_STRICTNESS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_ENABLE_STRICTNESS;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Gfa(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Gfa sflags | D3DCOMPILE_AVOID_FLOW_CONTROL\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Gis(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Gis sflags | D3DCOMPILE_IEEE_STRICTNESS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_IEEE_STRICTNESS;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Gpp(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Gpp sflags | D3DCOMPILE_PARTIAL_PRECISION\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PARTIAL_PRECISION;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_I(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _includeDir) {
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -I (Shader Include Dir) with arg {}\n", _includeDir);
#else
        std::cout << std::format("option -I (Shader Include Dir) with arg {}\n", _includeDir);
#endif
    }
    params.get_includeDirs()->AddIncludeDir(_includeDir);
    return;
}

void efxc2Cmds::cmd_Lx(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Lx - output hexidecimal literals\n";
    }
    params.set_outputHex(TRUE);
    return;
}

void efxc2Cmds::cmd_Ni(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Cc | D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING\n";
    }
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING;
    params.set_disassembly_flags(disassembly_flags);
    return;
}

void efxc2Cmds::cmd_No(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Cc | D3D_DISASM_ENABLE_INSTRUCTION_OFFSET\n";
    }
    UINT disassembly_flags = params.get_disassembly_flags();
    disassembly_flags = disassembly_flags | D3D_DISASM_ENABLE_INSTRUCTION_OFFSET;
    params.set_disassembly_flags(disassembly_flags);
    return;
}

void efxc2Cmds::cmd_O0(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -O0 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_O1(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_O2(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_O3(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -O1 sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Od(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Od sflags | D3DCOMPILE_SKIP_OPTIMIZATION\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_SKIP_OPTIMIZATION;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Op(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Op sflags | D3DCOMPILE_NO_PRESHADER\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_NO_PRESHADER;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_P(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW outputFile) {
    files.set_preprocessFile (outputFile);
    UINT cmd = params.get_commands();
    cmd = cmd | efxc2Utils::CMD_PREPROCESS_FILE;
    params.set_commands(cmd);
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -P (Output File) with arg {}\n", outputFile);
#else
        std::cout << std::format("option -P (Output File) with arg {}\n", outputFile);
#endif
    }
    return;
}

void efxc2Cmds::cmd_Qstrip_debug(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Qstrip_debug strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO;
    params.set_strip_flags(strip_flags);
    return;
}

void efxc2Cmds::cmd_Qstrip_priv(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Qstrip_priv strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA;
    params.set_strip_flags(strip_flags);
    return;
}

void efxc2Cmds::cmd_Qstrip_reflect(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Qstrip_reflect strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA;
    params.set_strip_flags(strip_flags);
    return;
}

void efxc2Cmds::cmd_Qstrip_rootsignature(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Qstrip_rootsignature strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATUR\n";
    }
    UINT strip_flags = params.get_strip_flags();
    strip_flags = strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE;
    params.set_strip_flags(strip_flags);
    return;
}

void efxc2Cmds::cmd_res_may_alias(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -res_may_alias sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_setprivate(efxc2CompilerParams::CompilerParams& params, efxc2Files::Files& files, _In_ const efxc2Utils::M_STRING_VIEW inputfile) {
    files.set_privateDataFile(inputfile);
    if (params.get_verbose() && params.get_debug()) {
#ifdef _WIN32
        std::wcout << std::format(L"option -setprivate (Input File) with arg {}\n", inputfile);
#else
        std::cout << std::format("option -setprivate (Input File) with arg {}\n", inputfile);
#endif
    }
    files.LoadPrivateDataFile(params);
    return;
}

void efxc2Cmds::cmd_T(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _model) {
#ifdef _WIN32
    std::string model = efxc2Utils::wstring_to_utf8({ _model.data(), _model.size() });
#else
    std::string model = { _model.data(), _model.size() };
#endif
    if (params.get_verbose() && params.get_debug()) {
        std::cout << std::format("option -T (Shader Model/Profile) with arg {}\n", model);
    }
    params.set_model(model);
    return;
}

void efxc2Cmds::cmd_Vd(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Vd sflags | D3DCOMPILE_SKIP_VALIDATION\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_SKIP_VALIDATION;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Vn(efxc2CompilerParams::CompilerParams& params, _In_ const efxc2Utils::M_STRING_VIEW _variableName) {
#ifdef _WIN32
    std::string variableName = efxc2Utils::wstring_to_utf8({ _variableName.data(), _variableName.size() });
#else
    std::string variableName = { _variableName.data(), _variableName.size()};
#endif
    if (params.get_verbose() && params.get_debug()) {
        std::cout << std::format("option -Vn (Variable Name) with arg {}'\n", variableName);
    }
    params.set_variableName(variableName);
    return;
}

void efxc2Cmds::cmd_WX(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -WX sflags |  D3DCOMPILE_WARNINGS_ARE_ERRORS\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Zi(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Zi sflags | D3DCOMPILE_DEBUG\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Zpc(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Zpc sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Zpr(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Zpr sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Zsb(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Zsb sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
    params.set_sflags(sflags);
    return;
}

void efxc2Cmds::cmd_Zss(efxc2CompilerParams::CompilerParams& params) {
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "option -Zss sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n";
    }
    UINT sflags = params.get_sflags();
    sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
    params.set_sflags(sflags);
    return;
}

bool  efxc2Cmds::parseCompilerOnlyCall( 
    _In_ const efxc2Utils::M_CMD_PARAMS& args,
    _Inout_	size_t* index,
    efxc2CompilerParams::CompilerParams& params) {
    bool terminate_routine = false;
    bool result = false;
    if (!index || *index >= args.size()) {
        terminate_routine = true;
    }
    efxc2Utils::M_STRING argument;
    size_t arg_idx = 0;
    if (terminate_routine == false) {
        argument = args[*index];
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
    if (!index || *index >= args.size()) {
        terminate_routine = true;
    }
    efxc2Utils::M_STRING argumentOption = efxc2Utils::M_STRING_INIT;
    efxc2Utils::M_STRING_VIEW argument;
    size_t arg_idx = 0;
    if (terminate_routine == false) {
        argument = args[*index];
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
            if (argument.compare(arg_idx, currentEntry.Param.size(), currentEntry.Param) != 0) { //-V2571 //V3546 //-V3546	
                continue;
            }
            arg_idx += currentEntry.Param.size(); //-V2571 //V3546 //-V3546	
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
    bool terminate_routine = false;
    bool result = false;
    if ((index == nullptr) || (*index >= args.size())) {
        terminate_routine = true;
    }
    if (terminate_routine == false) {
        const efxc2Utils::M_STRING_VIEW argument = args[*index];
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
            if (efxc2Utils::M_STRING_VIEW toFind = argument.substr(arg_idx, std::string::npos); //-V2571 //V3546 //-V3546	
                std::ranges::find(g_IgnoredOptions.begin(), g_IgnoredOptions.end(), toFind) != g_IgnoredOptions.end()) {
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
    bool terminate_routine = false;
    bool result = false;
    if (!index || *index >= args.size()) {
        terminate_routine = true;
    }
    if (terminate_routine == false) {
        const efxc2Utils::M_STRING_VIEW argument = args[*index];
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
            if (efxc2Utils::M_STRING_VIEW toFind = argument.substr(arg_idx, std::string::npos); //-V2571 //V3546 //-V3546
                std::ranges::find(g_NotSupportedArgs.begin(), g_NotSupportedArgs.end(), toFind) != g_NotSupportedArgs.end()) {
#ifdef _WIN32
                  std::wcerr << std::format(L"option -{} not supported", argument);
#else
                  std::cerr << std::format("option -{} not supported", argument);
#endif
                  std::cout << efxc2Utils::print_unsupported_arg_help();
                  result = true;
            }
        }
    }
    return result;
}
