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
    if (IncludeFile) { \
        delete[] IncludeFile; \
        IncludeFile = nullptr; \
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
    wchar_t* IncludeFile = nullptr;
    wchar_t* ObjectFile = nullptr;
    wchar_t* pdbFile = nullptr;
    wchar_t* w_temp = nullptr;
    char* c_inputFile = nullptr;
    char* c_pdbFile = nullptr;
#else  /* _WIN32 */
    char* inputFile = nullptr;
    char* IncludeFile = nullptr;
    char* ObjectFile = nullptr;
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
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif /* _MSC_VER*/
        else if (parseOpt(M_D, argc, argv, &index, &w_temp)) {
            cmd_D(verbose, &numDefines, w_temp, defines);
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
#else  /* _WIN32 */
        else if (parseOpt(M_D, argc, argv, &index, &defineOption)) {
            cmd_D(verbose, &numDefines, defineOption, defines);
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
#ifdef _WIN32
            cmd_Fd(verbose, pdbFile, &c_pdbFile);
#else
            cmd_Fd(verbose, pdbFile);
#endif
            continue;
        }
        else if (parseOpt(M_FE, argc, argv, &index, nullptr)) {
            option_ignored(M_FE, verbose);
            continue;
        }
        else if (parseOpt(M_FH, argc, argv, &index, &IncludeFile)) {
            cmd_Fh(verbose, &cmd, IncludeFile);
            continue;
        }
        else if (parseOpt(M_FL, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -Fl not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_FO, argc, argv, &index, &ObjectFile)) {
            cmd_Fo(verbose, &cmd, ObjectFile);
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
    if (IncludeFile == nullptr)
        print_usage_missing("IncludeFile");

    // ====================================================================================
    // Shader Compilation
    size_t SourceLen = 0;
    char* SourceCode = LoadSource(inputFile, &SourceLen);
    Compiler compiler;
    compiler.set_verbose(verbose);
    compiler.set_outputHex(outputHex);
    compiler.set_sflags(sflags);
    compiler.set_eflags(eflags);
    compiler.set_secondary_flags(secondary_flags);
    compiler.set_strip_flags(strip_flags);
    compiler.set_entryPoint(entryPoint);
    compiler.set_model(model);
    ID3DBlob* output = nullptr;
#ifdef _WIN32
    compiler.set_inputFile(c_inputFile);
#else
    compiler.set_inputFile(inputFile);
#endif
    compiler.Compile(SourceCode, SourceLen, numDefines, defines);   
#ifdef _WIN32
    errno_t err = 0;
#endif
    FILE* f;
    size_t  outputLen = 0;
    /*write .PDB data if applicable*/
#ifdef _WIN32 
    if (c_pdbFile != nullptr) {
        auto pPDBFileName = GetFileName(c_pdbFile);
#else
    if (pdbFile != nullptr) {
        auto pPDBFileName = GetFileName(pdbFile);
#endif
        if (pPDBFileName != nullptr) {
            compiler.SetPDBFileName(pPDBFileName);
        }
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
        err = _wfopen_s(&f, pdbFile, L"w");
        if (err != 0) {
            print_errno(err);
        }
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
        f = fopen(IncludeFile, "w");
        if (f == nullptr) {
            print_errno();
        }
#endif
        outputLen = compiler.WritePDBFile(f);
        fclose(f);
#ifdef _WIN32
        wprintf(L"Wrote %zu bytes of .PDB data to %ls\n", outputLen, pdbFile);
#else   /* _WIN32 */
        printf("Wrote %zu", outputLen);
        printf(" bytes of .PDB data to %ls\n", pdbFile);
#endif  /* WIN32 */
    }
    // ====================================================================================
    // Output (or errors)
    compiler.StripShader();
    if ((cmd & CMD_WRITE_HEADER) == CMD_WRITE_HEADER) {
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
        err = _wfopen_s(&f, IncludeFile, L"w");
        if (err != 0) {
            print_errno(err);
        }
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
        f = fopen(IncludeFile, "w");
        if (f == nullptr) {
            print_errno();
        }
#endif
        outputLen = compiler.WriteIncludeFile(f);
        fclose(f);
        if (verbose) {
#ifdef _WIN32
            wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, IncludeFile);
#else   /* _WIN32 */
            printf("Wrote %zu", outputLen);
            printf(" bytes of shader output to %ls\n", IncludeFile);
#endif  /* WIN32 */
        }
    }
    if ((cmd & CMD_WRITE_OBJECT) == CMD_WRITE_OBJECT) {
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
        err = _wfopen_s(&f, ObjectFile, L"w");
        if (err != 0) {
            print_errno(err);
        }
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
#else
        f = fopen(ObjectFile, "w");
        if (f == nullptr) {
            print_errno();
         }
#endif
        outputLen = compiler.WriteObjectFile(f);
        fclose(f);
        if (verbose) {
#ifdef _WIN32
            wprintf(L"Wrote %zu bytes of shader output to %ls\n", outputLen, ObjectFile);
#else   /* _WIN32 */
            printf("Wrote %zu", outputLen);
            printf(" bytes of shader output to %ls\n", ObjectFile);
#endif
        }
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