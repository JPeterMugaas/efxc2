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

static void print_copyright() {
    printf(PROGRAM_DESCRIPTION " " PROGRAM_VERSION "\n");
    printf(PROGRAM_COPYRIGHT "\n");
    printf("This program is licensed under the Mozilla Public License, v. 2.0.\n");
    return;
}

static void print_usage_arg() {
    printf("\n");
    printf("More information about valid parameters is available at Microsoft's website \n");
    printf("\n");
    printf("https://msdn.microsoft.com/en-us/library/windows/desktop/bb509709(v=vs.85).aspx\n");
    return;
}

static void print_unsupported_arg_help() {
    printf("This isn't a sign of disaster, odds are it will be very easy to add support for\n");
    printf("this argument.  Review the meaning of the argument in the real fxc program, and\n");
    printf("then add it into efxc2.\n");
    printf("\n");
    print_usage_arg();
    return;
}

[[noreturn]] static void print_version() {
    printf(PROGRAM_DESCRIPTION " version "  PROGRAM_VERSION "\n");
    printf(PROGRAM_COPYRIGHT "\n");
    exit(0);
}

[[noreturn]]  static void print_usage_missing(const char* arg) {
    fprintf(stderr, "efxc2 is missing the %s argument.\n", arg);
    printf("We expected to receive this, and it's likely things will not work correctly\n");
    printf("without it.  Review efxc2 and make sure things will work.\n");
    printf("\n");
    print_usage_arg();
    exit(1);
}

[[noreturn]] static void print_usage_toomany() {
    fprintf(stderr, "You specified multiple input files.\n");
    printf("We did not expect to receive this, and aren't prepared to handle multiple input\n");
    printf("files. You'll have to edit the source to behave the way you want.\n");
    printf("\n");
    print_usage_arg();
    exit(1);
}

[[noreturn]] static void print_hresult_error(const HRESULT hr) {
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);
    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    fprintf(stderr, "Windows Error Message: %s\n", messageBuffer);
    printf("Error Code: 0x%lx", hr);
    exit(1);
}

[[noreturn]] static void print_windows_error() {
    /*from: https://gist.github.com/Aaronontheweb/7461004#file-getlasterror-cpp */
    DWORD dLastError = GetLastError();
    LPCTSTR strErrorMessage = nullptr;

    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        nullptr,
        dLastError,
        0,
        (LPWSTR)&strErrorMessage,
        0,
        nullptr);

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4477)
#endif /* _MSC_VER */
#ifdef _WIN32
    fwprintf(stderr, L"Windows error: %ls\n", strErrorMessage);
#else  /* _WIN32 */
    fprintf(stderr, "Windows error: %ls\n", strErrorMessage);
#endif /* _WIN32 */
#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */
    exit(1);
}

static void print_help_screen() {
    print_copyright();
    print_usage_arg();
    return;
}

static void WriteByteArrayConst(_In_ FILE* f, _In_reads_bytes_(len) const unsigned char* outString, _In_ const size_t len, _In_z_ const char* variableName, _In_ const int outputHex) {
    fprintf(f, "const BYTE %s[] =\n{\n", variableName);
    for (size_t i = 0; i < len; i++) {
        if (outputHex) {
            fprintf(f, " 0x%02" PRIx8, outString[i]);
        }
        else {
            fprintf(f, "%4" PRIu8, outString[i]);
        }
        if (i != len - 1) {
            fprintf(f, ",");
        }
        if ((i % 6 == 5) && (i != len - 1)) {
            fprintf(f, "\n");
        }
    }
    fprintf(f, "\n};\n");
}

#define M_WINDOWS_ERROR \
    print_windows_error(); \
    M_TAREDOWN_PROG \
    return 1;

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

#ifdef _WIN32
static char* LoadSource(_In_ const wchar_t* filename, _Out_ size_t* len) {
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif  /* _MSC_VER */
    FILE* f = nullptr;
    errno_t err = _wfopen_s(&f, filename, L"r");
#ifdef _MSC_VER
#pragma warning( pop )
#endif  /* _MSC_VER */
    if (err != 0) {
        print_errno(err);
#else   /* _WIN32 */
static char* LoadSource(_In_ const char* filename, _Out_ size_t * len) {
    FILE* f = fopen(filename, "r");
    if (f == nullptr) {
        print_errno();
#endif  /* _WIN32 */
    }
    char* source;
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001)
#pragma warning( disable : 6387)
#endif /* _MSC_VER */
    readall(f, &source, len);
    fclose(f);
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
    return source;
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

    BYTE cmd = 0;

    int numDefines = 1;
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
        else if (parseOpt(M_ALL_RESOURCES_BOUND, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -all_resources_bound D3DCOMPILE_ALL_RESOURCES_BOUND\n");
            }
            sflags = sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
            continue;
        }
        else if (parseOpt(M_CC, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Cc ignored\n");
            }
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
            defineOption = wcharToChar(w_temp);
            assert(defineOption == nullptr);
#else  /* _WIN32 */
        else if (parseOpt(M_D, argc, argv, &index, &defineOption)) {
#endif /* _WIN32 */
            numDefines++;
            //Copy the old array into the new array, but put the new definition at the beginning
            newDefines = new D3D_SHADER_MACRO[numDefines];
            for (int i = 1; i < numDefines; i++)
                newDefines[i] = defines[i - 1];
            delete[] defines;
            defines = newDefines;
            defines[0].Name = defineOption;
            defines[0].Definition = "1";
            if (verbose) {
                printf("option -D with arg %s\n", defineOption);
            }
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
            entryPoint = wcharToChar(w_temp);
            delete[] w_temp;
#else  /* _WIN32 */
        else if (parseOpt(M_E_, argc, argv, &index, &entryPoint)) {
#endif /* _WIN32 */
            if (verbose) {
                printf("option -E (Entry Point) with arg '%s'\n", entryPoint);
            }
            continue;
        }
        else if (parseOpt(M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -enable_unbounded_descriptor_tables D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n");
            }
            sflags = sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
            continue;
        }
        else if (parseOpt(M_FD, argc, argv, &index, &pdbFile)) {
#ifdef _WIN32
            FixupFileName(pdbFile);
#endif /* _WIN32 */
            if (verbose) {
                printf("option -Fd (.PDB) with arg %ls\n", pdbFile);
            }

            continue;
        }
        else if (parseOpt(M_FE, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Fe ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_FH, argc, argv, &index, &outputFile)) {
#ifdef _WIN32
            FixupFileName(outputFile);
#endif /* _WIN32 */
            if (cmd != 0) {
                printf("You cannot specify both an object and header");
                M_TAREDOWN_PROG
                    return 1;
            }
            cmd = CMD_WRITE_HEADER;
            if (verbose) {
                printf("option -Fh (Output File) with arg %ls\n", outputFile);
            }
            continue;
        }
        else if (parseOpt(M_FL, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -Fl not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_FO, argc, argv, &index, nullptr)) {
            if (cmd != 0) {
                fprintf(stderr, "You cannot specify both an object and header");
                M_TAREDOWN_PROG
                    return 1;
            }
            cmd = CMD_WRITE_OBJECT;
            if (verbose) {
                printf("option -FO (Output File) with arg %ls\n", outputFile);
            }
            continue;
        }
        else if (parseOpt(M_FORCE_ROOTSIG_VER, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -force_rootsig_ver ignored");
            }
            continue;
        }
        else if (parseOpt(M_FX, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -Fx not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_GCH, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Gch D3DCOMPILE_EFFECT_CHILD_EFFECT");
            }
            eflags = eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
            continue;
        }
        else if (parseOpt(M_GDP, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Gdp D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS");
            }
            eflags = eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
            continue;
        }
        else if (parseOpt(M_GEC, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Gec D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n");
            }
            sflags = sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
            continue;
        }
        else if (parseOpt(M_GES, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Ges D3DCOMPILE_ENABLE_STRICTNESS\n");
            }
            sflags = sflags | D3DCOMPILE_ENABLE_STRICTNESS;
            continue;
        }
        else if (parseOpt(M_GETPRIVATE, argc, argv, &index, nullptr)) {
            fprintf(stderr, "option -getprivate not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
        else if (parseOpt(M_GFA, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Gfa D3DCOMPILE_AVOID_FLOW_CONTROL\n");
            }
            sflags = sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
            continue;
        }
        else if (parseOpt(M_GIS, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Gis D3DCOMPILE_IEEE_STRICTNESS\n");
            }
            sflags = sflags | D3DCOMPILE_IEEE_STRICTNESS;
            continue;
        }
        else if (parseOpt(M_GPP, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Gpp D3DCOMPILE_PARTIAL_PRECISION\n");
            }
            sflags = sflags | D3DCOMPILE_PARTIAL_PRECISION;
            continue;
        }
        else if (parseOpt(M_HELP, argc, argv, &index, nullptr)) {
            print_help_screen();
            M_TAREDOWN_PROG
                return 0;
        }
        else if (parseOpt(M_I, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -I ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_LX, argc, argv, &index, nullptr)) {
            outputHex = 1;
            if (verbose) {
                printf("option -Lx - output hexidecimal literals\n");
            }
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
            if (verbose) {
                printf("option -Ni ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_NO, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -No ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
            continue;
        }
        else if (parseOpt(M_O0, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -O0 D3DCOMPILE_OPTIMIZATION_LEVEL0\n");
            }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
            continue;
        }
        else if (parseOpt(M_O1, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL1\n");
            }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
            continue;
        }
        else if (parseOpt(M_O2, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL2\n");
            }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
            continue;
        }
        else if (parseOpt(M_O3, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL3\n");
            }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
            continue;
        }
        else if (parseOpt(M_OD, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Od D3DCOMPILE_SKIP_OPTIMIZATION\n");
            }
            sflags = sflags | D3DCOMPILE_SKIP_OPTIMIZATION;
            continue;
        }
        else if (parseOpt(M_OP, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Op D3DCOMPILE_NO_PRESHADER\n");
            }
            sflags = sflags | D3DCOMPILE_NO_PRESHADER;
            continue;
        }
        else if (parseOpt(M_P, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -P ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_DEBUG, argc, argv, &index, nullptr)) {
            strip_flags = strip_flags | D3DCOMPILER_STRIP_DEBUG_INFO;
            if (verbose) {
                printf("option -Qstrip_debug D3DCOMPILER_STRIP_DEBUG_INFO\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_PRIV, argc, argv, &index, nullptr)) {
            strip_flags = strip_flags | D3DCOMPILER_STRIP_PRIVATE_DATA;
            if (verbose) {
                printf("option -Qstrip_priv D3DCOMPILER_STRIP_PRIVATE_DATA\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_REFLECT, argc, argv, &index, nullptr)) {
            strip_flags = strip_flags | D3DCOMPILER_STRIP_REFLECTION_DATA;
            if (verbose) {
                printf("option -Qstrip_reflect D3DCOMPILER_STRIP_REFLECTION_DATA\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_ROOTSIGNATURE, argc, argv, &index, nullptr)) {
            strip_flags = strip_flags | D3DCOMPILER_STRIP_ROOT_SIGNATURE;
            if (verbose) {
                printf("option -Qstrip_rootsignature D3DCOMPILER_STRIP_ROOT_SIGNATURE\n");
            }
            continue;
        }
        else if (parseOpt(M_RES_MAY_ALIAS, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -res_may_alias D3DCOMPILE_RESOURCES_MAY_ALIAS\n");
            }
            sflags = sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
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
            model = wcharToChar(w_temp);
            delete[] w_temp;
#else  /* _WIN32 */
        else if (parseOpt(M_T, argc, argv, &index, &model)) {
#endif /* _WIN32 */
            if (verbose) {
                printf("option -T (Shader Model/Profile) with arg '%s'\n", model);
            }
            continue;
        }
        else if (parseOpt(M_VD, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Vd D3DCOMPILE_SKIP_VALIDATION\n");
            }
            sflags = sflags | D3DCOMPILE_SKIP_VALIDATION;
            continue;
        }
        else if (parseOpt(M_VERSION, argc, argv, &index, nullptr)) {
            print_version();
            M_TAREDOWN_PROG
                return 0;
        }
        else if (parseOpt(M_VI, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Vi (Output include process details) acknowledged but ignored.\n");
            }
            continue;
        }
#ifdef _WIN32
        else if (parseOpt(M_VN, argc, argv, &index, &w_temp)) {
            variableName = wcharToChar(w_temp);
            delete[] w_temp;
#else  /* _WIN32 */
        else if (parseOpt(M_VN, argc, argv, &index, &variableName)) {
#endif /* _WIN32 */
            if (verbose) {
                printf("option -Vn (Variable Name) with arg '%s'\n", variableName);
            }
            continue;
        }
        else if (parseOpt(M_WX, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -WX D3DCOMPILE_WARNINGS_ARE_ERRORS\n");
            }
            sflags = sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
            continue;
        }
        else if (parseOpt(M_ZI, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Zi D3DCOMPILE_DEBUG\n");
            }
            sflags = sflags | D3DCOMPILE_DEBUG;
            continue;
        }
        else if (parseOpt(M_ZPC, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Zpc D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n");
            }
            sflags = sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
            continue;
        }
        else if (parseOpt(M_ZPR, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Zpr D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n");
            }
            sflags = sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
            continue;
        }
        else if (parseOpt(M_ZSB, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Zsb D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n");
            }
            sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
            continue;
        }
        else if (parseOpt(M_ZSS, argc, argv, &index, nullptr)) {
            if (verbose) {
                printf("option -Zss D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n");
            }
            sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
            continue;
        }
        else {
            if (!inputFile)
            {
                if (verbose) {
                    printf("Parse input file name\n");
                }
#ifdef _WIN32
                inputFile = new wchar_t[wcslen(argv[index]) + 1];
                wcscpy_s(inputFile, wcslen(argv[index]) + 1, argv[index]);
                FixupFileName(inputFile);
                c_inputFile = wcharToChar(inputFile);
#else  /* _WIN32 */
                inputFile = new char[strlen(argv[index]) + 1];
                strncpy(inputFile, argv[index], strlen(argv[index]) + 1);
#endif /* _WIN32 */

                if (verbose) {
#ifdef _WIN32
                    wprintf(L"input file: %ls\n", inputFile);
#else  /* _WIN32 */
                    printf("input file: %ls\n", inputFile);
#endif /* _WIN32 */
                }
                index += 1;
            }
            else {
                M_TAREDOWN_PROG
                    print_usage_toomany();
                return 1;
            }
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

    HMODULE h = LoadLibrary(DLL_NAME);
    if (h == nullptr) {
#ifdef _WIN32
        //Find the WINDOWS dll
        wchar_t dllPath[MAX_PATH];
        size_t bytes = GetModuleFileName(nullptr, dllPath, MAX_PATH);
        if (bytes == 0) {
            fprintf(stderr, "Could not retrieve the directory of the running executable.\n");
            print_windows_error();
            M_TAREDOWN_PROG
        }
        //Fill rest of the buffer with NULLs
        memset(dllPath + bytes, '\0', MAX_PATH - bytes);
        //Copy the dll location over top efxc2.exe
        wcscpy_s(dllPath + bytes, MAX_PATH, DLL_NAME);

        h = LoadLibrary(dllPath);
        if (h == nullptr) {
            wprintf(L"Error: could not load " DLL_NAME L" from %s\n", dllPath);
            M_WINDOWS_ERROR
        }
#else  /* _WIN32 */
        M_WINDOWS_ERROR
#endif /* _WIN32 */
    }
    size_t SourceLen = 0;
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6387 )
#endif /* _MSC_VER */
    char* SourceCode = LoadSource(inputFile, &SourceLen);
    auto ptp_D3DCompile2 = (pD3DCompile2g)GetProcAddress(h, "D3DCompile2");
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER */
    if (ptp_D3DCompile2 == nullptr) {
        printf("Error: could not get the address of D3DCompile2.\n");
        M_WINDOWS_ERROR
    }
    auto ptr_D3DStripShader = (pD3DStripShaderg)GetProcAddress(h, "D3DStripShader");
    if (ptp_D3DCompile2 == nullptr) {
        printf("Error: could not get the address of D3DStripShader.\n");
        M_WINDOWS_ERROR
    }
    auto ptr_D3DGetBlobPart = (pD3DGetBlobPartg)GetProcAddress(h, "D3DGetBlobPart");
        if (ptr_D3DGetBlobPart == nullptr) {
            printf("Error: could not get the address of D3DGetBlobPart.\n");
            M_WINDOWS_ERROR
        }

    HRESULT hr;
    ID3DBlob* output = nullptr;
    ID3DBlob* errors = nullptr;

    if (verbose) {
        printf("Calling D3DCompile2(\n");

        printf("\t SourceCode,\n");
#ifdef _WIN32
        wprintf(L"\t %zu,\n", SourceLen);
        printf("\t %s, \n", c_inputFile);
#else   /* _WIN32 */
        printf("\t %zu,\n", SourceLen);
        printf("\t %s, \n", inputFile);
#endif  /* _WIN32 */
        /* print defines */
        printf("\t");
        for (int i = 0; i < numDefines - 1; i++)
            printf(" %s=%s", defines[i].Name, defines[i].Definition);
        printf(",\n");
        /* done printing defines */
        printf("\t D3D_COMPILE_STANDARD_FILE_INCLUDE,\n");
        printf("\t %s,\n", entryPoint);
        printf("\t %s,\n", model);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)sflags);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)eflags);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)secondary_flags);
        printf("\t nullptr,\n");
        printf("\t 0,\n");
        printf("\t &output,\n");
        printf("\t &errors);\n");
    }

    /*
    HRESULT D3DCompile2(
       [in]            LPCVOID                pSrcData,
       [in]            SIZE_T                 SrcDataSize,
       [in, optional]  LPCSTR                 pSourceName,
       [in, optional]  const D3D_SHADER_MACRO *pDefines,
       [in, optional]  ID3DInclude            *pInclude,
       [in]            LPCSTR                 pEntrypoint,
       [in]            LPCSTR                 pTarget,
       [in]            UINT                   Flags1,
       [in]            UINT                   Flags2,
       [in]            UINT                   SecondaryDataFlags,
       [in, optional]  LPCVOID                pSecondaryData,
       [in]            SIZE_T                 SecondaryDataSize,
       [out]           ID3DBlob               **ppCode,
       [out, optional] ID3DBlob               **ppErrorMsgs
     );
    */
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
    hr = ptp_D3DCompile2(
        SourceCode,
        SourceLen,
#ifdef _WIN32
        c_inputFile,
#else  /* _WIN32 */
        inputFile,
#endif /* _WIN32 */
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        model,
        sflags,
        eflags,
        secondary_flags,
        nullptr,
        0,
        &output,
        &errors
    );
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    // ====================================================================================
    // Output (or errors)

    if (FAILED(hr)) {
        if (errors) {
            auto* error = (char*)errors->GetBufferPointer();
            fprintf(stderr, "Got an error while compiling:\n%s\n", error);
            errors->Release();
            printf("Error Code: 0x%lx", hr);
        }
        else {
            fprintf(stderr, "Got an error (%lx) while compiling, but no error message from the function.\n", hr);
            print_hresult_error(hr);
        }

        if (output) {
            output->Release();
        }
        if (SourceCode) {
            free(SourceCode);
        }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6001)
#endif
        M_TAREDOWN_PROG
#ifdef _MSC_VER
#pragma warning( pop )
#endif
            return 1;
    }
    else {
        FILE* f;
        const auto* compiledString = (unsigned char*)output->GetBufferPointer();
        size_t compiledLen = output->GetBufferSize();
        auto* outputString = compiledString;
        size_t outputLen = compiledLen;

        /* strip compiled shader*/
        if (strip_flags != 0) {
            ID3DBlob* strippedBlob = nullptr;
            if (verbose) {
                printf("Calling D3DStripShader(\n");
                printf("\t compiledString,\n");
                printf("\t %zu,\n", compiledLen);
                printf("\t 0x%016" PRIx64 ", \n", (INT64)strip_flags);
                printf("\t &strippedBlob);\n");
            }
            /*
            HRESULT D3DStripShader(
              [in]  LPCVOID  pShaderBytecode,
              [in]  SIZE_T   BytecodeLength,
              [in]  UINT     uStripFlags,
              [out] ID3DBlob **ppStrippedBlob
            );
            */
            hr = ptr_D3DStripShader(compiledString, compiledLen, strip_flags, &strippedBlob);
            if (FAILED(hr)) {
                print_hresult_error(hr);
                if (output)
                    output->Release();
                if (strippedBlob)
                    strippedBlob->Release();
                free(SourceCode);
                M_TAREDOWN_PROG
                    return 1;
            }
            const auto* strippedString = (unsigned char*)strippedBlob->GetBufferPointer();
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