//--------------------------------------------------------------------------------------
// File: efxc2.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2Utils.h"
#include "efxc2Cmds.h"
#include "efxc2Compiler.h"

#define M_TAREDOWN_PROG \
    if (inputFile) { \
        delete[] inputFile; \
        inputFile = nullptr; \
    } \
    if (outputFile) { \
        delete[] outputFile; \
        outputFile = nullptr; \
    } \
    \
    if (defineOption != nullptr) { \
        free(defineOption); \
        defineOption = nullptr; \
    } \
    if (entryPoint != nullptr) {  \
        free(entryPoint); \
        entryPoint = nullptr; \
    } \
    if (model != nullptr) { \
        free(model); \
        model = nullptr; \
    } \
    if (variableName != nullptr) { \
        free(variableName); \
        variableName = nullptr; \
    }

/*Cygwin and MSYS2 compilers amd linkers don't support
the wmain -Municode entry-point*/

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
#else  /* _WIN32 */
int main(int argc, char* argv[]) {
#endif /* _WIN32 */
    // ====================================================================================
    // Process Command Line Arguments
#ifdef _WIN32
    wchar_t* inputFile = nullptr;
    wchar_t* outputFile = nullptr;
    wchar_t* pdbFile = nullptr;
    wchar_t* w_temp = nullptr;
    char* c_inputFile = nullptr;
#else  /* _WIN32 */
    char* inputFile = nullptr;
    char* outputFile = nullptr;
    char* pdbFile = nullptr;
#endif /* _WIN32 */

    char* defineOption = nullptr;
    char* entryPoint = nullptr;
    char* model = nullptr;
    char* variableName = nullptr;

    UINT sflags = 0;
    UINT eflags = 0;
    UINT secondary_flags = 0;
    UINT strip_flags = 0;
    int verbose = 1;
    int outputHex = 0;

    UINT cmd = 0;

    size_t numDefines = 1;
    auto* defines = new D3D_SHADER_MACRO[numDefines];
    defines[numDefines - 1].Name = nullptr;
    defines[numDefines - 1].Definition = nullptr;

    /*first scan specifically for the nologo argument so no output
    is given regardless of parameter order*/
    int index = 1;
    while (index < argc) {
        /* Detect the end of the options. */
        if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
            verbose = false;
            break;
        }
        else {
            index++;
        }
    }

    /*now scan for all arguments and input file name*/
    index = 1;
    while (TRUE) {
        D3D_SHADER_MACRO* newDefines;

        /* Detect the end of the options. */
        if (index >= argc)
            break;
        else if (parseOpt(M_AT_SYMBOL, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -@ not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_QUESTION_MARK, argc, argv, &index, nullptr)) {
            print_help_screen();
            M_TAREDOWN_PROG
                return 0;
        }
        else if (parseOpt(M_QUESTION_MARK, argc, argv, &index, nullptr)) {
            cmd_all_resources_bound(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_CC, argc, argv, &index, nullptr)) {
            option_ignored(M_CC, verbose);
            continue;
        }
        else if (parseOpt(M_COMPRESS, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -compress not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
#ifdef _WIN32
        else if (parseOpt(M_D, argc, argv, &index, &w_temp)) {
            cmd_D(verbose, &numDefines, w_temp, defines);
#else  /* _WIN32 */
        else if (parseOpt(M_D, argc, argv, &index, &defineOption)) {
            cmd_D(verbose, &numdefines, defineOption, defines);
#endif /* _WIN32 */
            continue;
        }
        else if (parseOpt(M_DECOMPRESS, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -decompress not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_DUMPBIN, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -dumpbin not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
#ifdef _WIN32
        else if (parseOpt(M_E_, argc, argv, &index, &w_temp)) {
            cmd_E(verbose, &entryPoint, w_temp);
            delete[] w_temp;
#else  /* _WIN32 */
        else if (parseOpt(M_E_, argc, argv, &index, &entryPoint)) {
            cmd_E(verbose, entryPoint);
#endif /* _WIN32 */
            continue;
        }
        else if (parseOpt(M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES, argc, argv, &index, nullptr)) {
            cmd_enable_unbounded_descriptor_tables(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_FD, argc, argv, &index, &pdbFile)) {
            cmd_Fd(verbose, pdbFile);
            continue;
        }
        else if (parseOpt(M_FE, argc, argv, &index, nullptr)) {
            option_ignored(M_FE, verbose);
            continue;
        }
        else if (parseOpt(M_FH, argc, argv, &index, &outputFile)) {
            cmd_Fh(verbose, &cmd, outputFile);
            continue;
        }
        else if (parseOpt(M_FL, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -Fl not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_FO, argc, argv, &index, nullptr)) {
            cmd_Fo(verbose, &cmd, outputFile);
            continue;
        }
        else if (parseOpt(M_FORCE_ROOTSIG_VER, argc, argv, &index, nullptr)) {
            option_ignored(argv[index], verbose);
            continue;
        }
        else if (parseOpt(M_FX, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -Fx not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_GCH, argc, argv, &index, nullptr)) {
            cmd_Gch(&eflags, verbose);
            continue;
        }
        else if (parseOpt(M_GDP, argc, argv, &index, nullptr)) {
            cmd_Gdp(&eflags, verbose);
            continue;
        }
        else if (parseOpt(M_GEC, argc, argv, &index, nullptr)) {
            cmd_Gec(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_GES, argc, argv, &index, nullptr)) {
            cmd_Ges(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_GETPRIVATE, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -getprivate not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_GFA, argc, argv, &index, nullptr)) {
            cmd_Gfa(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_GIS, argc, argv, &index, nullptr)) {
            cmd_Gis(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_GPP, argc, argv, &index, nullptr)) {
            cmd_Gpp(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_HELP, argc, argv, &index, nullptr)) {
            print_help_screen();
            M_TAREDOWN_PROG
                return 0;
        }
        else if (parseOpt(M_I, argc, argv, &index, nullptr)) {
            option_ignored(M_I, verbose);
            continue;
        }
        else if (parseOpt(M_LX, argc, argv, &index, nullptr)) {
            cmd_Lx(verbose, &outputHex);
            continue;
        }
        else if (parseOpt(M_MATCHUAVS, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -matchUAVs not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_MERGEUAVS, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -mergeUAVs not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_NI, argc, argv, &index, nullptr)) {
            option_ignored(M_NI, verbose);
            continue;
        }
        else if (parseOpt(M_NO, argc, argv, &index, nullptr)) {
            option_ignored(M_NO, verbose);
            continue;
        }
        else if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
            continue;
        }
        else if (parseOpt(M_O0, argc, argv, &index, nullptr)) {
            cmd_O0(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_O1, argc, argv, &index, nullptr)) {
            cmd_O1(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_O2, argc, argv, &index, nullptr)) {
            cmd_O2(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_O3, argc, argv, &index, nullptr)) {
            cmd_O3(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_OD, argc, argv, &index, nullptr)) {
            cmd_Od(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_OP, argc, argv, &index, nullptr)) {
            cmd_Op(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_P, argc, argv, &index, nullptr)) {
            option_ignored(M_P, verbose);
            continue;
        }
        else if (parseOpt(M_QSTRIP_DEBUG, argc, argv, &index, nullptr)) {
            cmd_Qstrip_debug(&strip_flags, verbose);
            continue;
        }
        else if (parseOpt(M_QSTRIP_PRIV, argc, argv, &index, nullptr)) {
            cmd_Qstrip_priv(&strip_flags, verbose);
            continue;
        }
        else if (parseOpt(M_QSTRIP_REFLECT, argc, argv, &index, nullptr)) {
            cmd_Qstrip_reflect(&strip_flags, verbose);
            continue;
        }
        else if (parseOpt(M_QSTRIP_ROOTSIGNATURE, argc, argv, &index, nullptr)) {
            cmd_Qstrip_rootsignature(&strip_flags, verbose);
            continue;
        }
        else if (parseOpt(M_RES_MAY_ALIAS, argc, argv, &index, nullptr)) {
            cmd_res_may_alias(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_SETPRIVATE, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -setprivate not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
#ifdef _WIN32
        else if (parseOpt(M_T, argc, argv, &index, &w_temp)) {
            cmd_T(verbose, &model, w_temp);
            delete[] w_temp;
#else  /* _WIN32 */
        else if (parseOpt(M_T, argc, argv, &index, &model)) {
            cmd_T(verbose, model);
#endif /* _WIN32 */
            continue;
        }
        else if (parseOpt(M_VD, argc, argv, &index, nullptr)) {
            cmd_Vd(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_VERSION, argc, argv, &index, nullptr)) {
            print_version();
            M_TAREDOWN_PROG
                return 0;
        }
        else if (parseOpt(M_VI, argc, argv, &index, nullptr)) {
            option_ignored(M_VI, verbose);
            continue;
        }
#ifdef _WIN32
        else if (parseOpt(M_VN, argc, argv, &index, &w_temp)) {
            cmd_Vn(verbose, &variableName, w_temp);
            delete[] w_temp;
#else  /* _WIN32 */
        else if (parseOpt(M_VN, argc, argv, &index, &variableName)) {
            cmd_Vn(verbose, variableName);
#endif /* _WIN32 */
            continue;
        }
        else if (parseOpt(M_WX, argc, argv, &index, nullptr)) {
            cmd_WX(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_ZI, argc, argv, &index, nullptr)) {
            cmd_Zi(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_ZPC, argc, argv, &index, nullptr)) {
            cmd_Zpc(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_ZPR, argc, argv, &index, nullptr)) {
            cmd_Zpr(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_ZSB, argc, argv, &index, nullptr)) {
            cmd_Zsb(&sflags, verbose);
            continue;
        }
        else if (parseOpt(M_ZSS, argc, argv, &index, nullptr)) {
            cmd_Zss(&sflags, verbose);
            continue;
        }
        else {
#ifdef _WIN32
            parseInputFile(argv[index], &inputFile, &c_inputFile, verbose);
#else
            parseInputFile(argv[index], &inputFile, verbose);
#endif
            index += 1;
        }
    }

    if (inputFile == nullptr)
        print_usage_missing("inputFile");
    if (model == nullptr)
        print_usage_missing("model");
    if (entryPoint == nullptr)
        print_usage_missing("entryPoint");
    if (defines == nullptr)
        print_usage_missing("defines");
    if (outputFile == nullptr)
        print_usage_missing("outputFile");

    //Default output variable name
    if (variableName == nullptr) {
        const char* prefix = "g";
        for (int i = 0; g_profilePrefixTable[i].name != nullptr; i++) {
            if (strcmp(g_profilePrefixTable[i].name, model) == 0) {
                prefix = g_profilePrefixTable[i].prefix;
                break;
            }
        }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( suppress : 6387 )
#endif /* _MSC_VER */
        variableName = (char*)malloc(strlen(prefix) + strlen(entryPoint) + 2);
#ifdef _WIN32
        sprintf_s(variableName, strlen(prefix) + strlen(entryPoint) + 2, "%s_%s", prefix, entryPoint);
#else  /* _WIN32 */
        sprintf(variableName, "%s_%s", prefix, entryPoint);
#endif /* _WIN32 */
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
    }

    // ====================================================================================
    // Shader Compilation
    size_t SourceLen = 0;
    char* SourceCode = LoadSource(inputFile, &SourceLen);
    Compiler compiler;

    ID3DBlob* output = nullptr;
    compiler.set_sflags(sflags);
    compiler.set_eflags(eflags);
    compiler.set_secondary_flags(secondary_flags);

#ifdef _WIN32
    compiler.Compile(SourceCode, SourceLen, c_inputFile, numDefines, defines, entryPoint, model, &output);
#else
    compiler.Compile(SourceCode, SourceLen, inputFile, numDefines, defines, entryPoint, model, &output);
#endif
    // ====================================================================================
    // Output (or errors)
    FILE* f;
    auto* compiledString = (unsigned char*)output->GetBufferPointer();
    size_t compiledLen = output->GetBufferSize();
    auto* outputString = compiledString;
    size_t outputLen = compiledLen;

    /* strip compiled shader*/
    ID3DBlob* strippedBlob = nullptr;
    compiler.StripShader(compiledString, compiledLen, &strippedBlob);
    if (strippedBlob != nullptr) {
        auto* strippedString = (unsigned char*)strippedBlob->GetBufferPointer();
        size_t strippedLen = strippedBlob->GetBufferSize();
        outputString = strippedString;
        outputLen = strippedLen;
    }
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
    errno_t err = _wfopen_s(&f, outputFile, L"w");
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
    if (err != 0) {
        free(SourceCode);
        print_errno(err);
    }
#else  /*_WIN32 */
    f = fopen(outputFile, "w");
    if (f == nullptr) {
        free(SourceCode);
        print_errno();
    }
#endif /* _WIN32 */

    if (cmd == CMD_WRITE_HEADER) {
        WriteByteArrayConst(f, outputString, outputLen, variableName, outputHex);
    }
    if (cmd == CMD_WRITE_OBJECT) {
        fwrite(outputString, outputLen, 1, f);
    }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif /* _MSC_VER */
    fclose(f);
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
    if (errno != 0) {
        print_errno();
    }
    if (verbose) {
#ifdef _WIN32
        wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, outputFile);
#else   /* _WIN32 */
        printf("Wrote %zu", outputLen);
        printf(" bytes of shader output to %ls\n", outputFile);
#endif  /* WIN32 */
    }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
#ifdef _WIN32
    if (c_inputFile != nullptr) {
        free(c_inputFile);
    }
#endif /* _WIN32 */
    if (SourceCode != nullptr) {
        free(SourceCode);
    }
    M_TAREDOWN_PROG
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
        return 0;
}