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

static void print_version() {
    printf(PROGRAM_DESCRIPTION " version "  PROGRAM_VERSION "/n");
    printf(PROGRAM_COPYRIGHT "/n");
    exit(0);
}

static void print_copyright() {
    printf(PROGRAM_DESCRIPTION " " PROGRAM_VERSION "\n");
    printf(PROGRAM_COPYRIGHT "  This program is licensed under the Mozilla Public License, v. 2.0.\n");
}

static void print_usage_arg() {
    printf("\n");
    printf("More information about valid parameters is available at Microsoft's website \n");
    printf("\n");
    printf("https://msdn.microsoft.com/en-us/library/windows/desktop/bb509709(v=vs.85).aspx\n");
}

static void print_unsupported_arg_help() {
    printf("This isn't a sign of disaster, odds are it will be very easy to add support for\n");
    printf("this argument.  Review the meaning of the argument in the real fxc program, and\n");
    printf("then add it into efxc2.\n");
    printf("\n");
    print_usage_arg();
}

static void print_invalid_arg() {
    fprintf(stderr, "You have specified an argument that is not handled by efxc2\n");
    print_unsupported_arg_help();
    exit(1);
}

static void print_usage_missing(const char* arg) {
    fprintf(stderr, "efxc2 is missing the %s argument.\n", arg);
    printf("We expected to receive this, and it's likely things will not work correctly\n");
    printf("without it.  Review efxc2 and make sure things will work.\n");
    printf("\n");
    print_usage_arg();
    exit(1);
}
static void print_usage_toomany() {
    fprintf(stderr, "You specified multiple input files.\n");
    printf("We did not expect to receive this, and aren't prepared to handle multiple input\n"); 
    printf("files. You'll have to edit the source to behave the way you want.\n");
    printf("\n");
    print_usage_arg();
    exit(1);
}

static void print_hresult_error(const HRESULT hr) {
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    fprintf(stderr, "Windows Error Message: %s\n", messageBuffer);
    printf("Error Code: 0x%x", hr);
    exit(1);
}

static void print_windows_error() {
/*from: https://gist.github.com/Aaronontheweb/7461004#file-getlasterror-cpp */
    DWORD dLastError = GetLastError();
    LPCTSTR strErrorMessage = NULL;

    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL,
        dLastError,
        0,
        (LPWSTR)&strErrorMessage,
        0,
        NULL);
    ;
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4477)
#endif
    fprintf(stderr, "Windows error %s/n", strErrorMessage);
#if defined(_MSC_VER)
# pragma warning(pop)
#endif
    exit(1);
}


#define M_WINDOWS_ERROR \
   print_windows_error(); \
   M_TAREDOWN_PROG \
   return 1;

#define M_TAREDOWN_PROG \
    delete[] inputFile; \
    delete[] outputFile; \
\
    if (defineOption != NULL) { \
       free(defineOption); \
    } \
    if (entryPoint != NULL) { \
       free(entryPoint); \
     } \
     if (model != NULL) { \
       free(model); \
     } \
     if (variableName != NULL) { \
       free(variableName); \
     }


#ifdef _WIN32
static char* LoadSource(const wchar_t* filename, size_t* len) {
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif  /* _MSC_VER */
    FILE* f;
    errno_t err = _wfopen_s(&f, filename, L"r");
#ifdef _MSC_VER
#pragma warning( pop )
#endif  /* _MSC_VER */
#else   /* _WIN32 */
static char* LoadSource(const char* filename, size_t * len) {
    FILE* f;
    f = fopen(filename, "r");
#endif  /* _WIN32*/
    if (f == NULL) {
        print_errno();
        exit(1);
    }
    char* source;
    readall(f, &source, len);
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif /* _MSC_VER */
    fclose(f);
#ifdef _MSC_VER
#pragma warning( pop )
#endif /* _MSC_VER*/
    return source;
}

/*Cygwin and MSYS2 compilers amd linkers don't support 
the wmain -Municode entry-point*/

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
#else
int main(int argc, char* argv[]) {
#endif
    // ====================================================================================
    // Process Command Line Arguments
#ifdef _WIN32
    wchar_t* inputFile = NULL;
    wchar_t* outputFile = NULL;
    char* c_inputFile = NULL;
#else
    char* inputFile = NULL;
    char* outputFile = NULL;
#endif
#ifdef _WIN32
    wchar_t* w_temp = NULL;
#endif

    char* defineOption = NULL;
    char* entryPoint = NULL;
    char* model = NULL;
    char* variableName = NULL;

    UINT sflags = 0;
    UINT eflags = 0;
    UINT secondary_flags = 0;
    int verbose = 1;
    int outputHex = 0;

    BYTE cmd = 0;

    int numDefines = 1;
    D3D_SHADER_MACRO* defines = new D3D_SHADER_MACRO[numDefines];
    defines[numDefines - 1].Name = NULL;
    defines[numDefines - 1].Definition = NULL;

    wchar_t* ptr_cmd = (wchar_t*)argv;

    /*first scan specifically for the nologo argument so no output
    is given regardless of parameter order*/
    int index = 1;
    while (1) {
        /* Detect the end of the options. */
        if (index >= argc)
            break;
        if (parseOpt(M_NOLOGO, argc, argv, &index, NULL)) {
            verbose = false;
            break;
        }
        else {
            index++;
        }
    }

    /*now scan for all arguments and input file name*/
    index = 1;
    while (1) {
        D3D_SHADER_MACRO* newDefines;

        /* Detect the end of the options. */
        if (index >= argc)
            break;
        else if (parseOpt(M_AT_SYMBOL, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -@ not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
        else if (parseOpt(M_QUESTION_MARK, argc, argv, &index, NULL)) {
            print_copyright();
            print_usage_arg();
            M_TAREDOWN_PROG
            return 0;
        }
        else if (parseOpt(M_ALL_RESOURCES_BOUND, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -all_resources_bound D3DCOMPILE_ALL_RESOURCES_BOUND\n");
            }
            sflags = sflags | D3DCOMPILE_ALL_RESOURCES_BOUND;
            continue;
        }
        else if (parseOpt(M_CC, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Cc ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_COMPRESS, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -compress not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
#ifdef _WIN32
        else if (parseOpt(M_D, argc, argv, &index, &w_temp)) {
            defineOption = wcharToChar(w_temp);
            assert(defineOption == NULL);
#else
        else if (parseOpt(M_D, argc, argv, &index, &defineOption)) {
#endif
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
        else if (parseOpt(M_DECOMPRESS, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -decompress not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
        else if (parseOpt(M_DUMPBIN, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -dumpbin not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
                return 1;
        }
#ifdef _WIN32
        else if (parseOpt(M_E_, argc, argv, &index, &w_temp)) {
            entryPoint = wcharToChar(w_temp);
            delete[] w_temp;
#else
        else if (parseOpt(M_E_, argc, argv, &index, &entryPoint)) {
#endif
            if (verbose) {
                printf("option -E (Entry Point) with arg '%s'\n", entryPoint);
            }
            continue;
        }
        else if (parseOpt(M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -enable_unbounded_descriptor_tables D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n");
            }
            sflags = sflags | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
            continue;
        }
        else if (parseOpt(M_FD, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Fd ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_FE, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Fe ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_FH, argc, argv, &index, &outputFile)) {
#ifdef _WIN32
            FixupFileName(outputFile);
#endif
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
        else if (parseOpt(M_FL, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -Fl not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
        else if (parseOpt(M_FO, argc, argv, &index, NULL)) {
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
        else if (parseOpt(M_FORCE_ROOTSIG_VER, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -force_rootsig_ver ignored");
            }
            continue;
            }
        else if (parseOpt(M_FX, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -Fx not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
        else if (parseOpt(M_GCH, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gch D3DCOMPILE_EFFECT_CHILD_EFFECT");
            }
            eflags = eflags | D3DCOMPILE_EFFECT_CHILD_EFFECT;
            continue;
        }
        else if (parseOpt(M_GDP, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gdp D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS");
            }
            eflags = eflags | D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS;
            continue;
        }
        else if (parseOpt(M_GEC, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gec D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n");
            }
            sflags = sflags | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
            continue;
        }
        else if (parseOpt(M_GES, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Ges D3DCOMPILE_ENABLE_STRICTNESS\n");
            }
            sflags = sflags | D3DCOMPILE_ENABLE_STRICTNESS;
            continue;
        }
        else if (parseOpt(M_GETPRIVATE, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -getprivate not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
        else if (parseOpt(M_GFA, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gfa D3DCOMPILE_AVOID_FLOW_CONTROL\n");
            }
            sflags = sflags | D3DCOMPILE_AVOID_FLOW_CONTROL;
            continue;
        }
        else if (parseOpt(M_GIS, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gis D3DCOMPILE_IEEE_STRICTNESS\n");
            }
            sflags = sflags | D3DCOMPILE_IEEE_STRICTNESS;
            continue;
        }
        else if (parseOpt(M_GPP, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gpp D3DCOMPILE_PARTIAL_PRECISION\n");
            }
            sflags = sflags | D3DCOMPILE_PARTIAL_PRECISION;
            continue;
        }
        else if (parseOpt(M_HELP, argc, argv, &index, NULL)) {
            print_copyright();
            print_usage_arg();
            M_TAREDOWN_PROG
            return 0;
         }
        else if (parseOpt(M_I, argc, argv, &index, NULL)) {
            if (verbose) {
                 printf("option -I ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_LX, argc, argv, &index, NULL)) {
            outputHex = 1;
            if (verbose) {
                printf("option -Lx - output hexidecimal litterals\n");
            }
            continue;
        }
        else if (parseOpt(M_MATCHUAVS, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -matchUAVs not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
         }
        else if (parseOpt(M_MERGEUAVS, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -mergeUAVs not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
        else if (parseOpt(M_NI, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Ni ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_NO, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -No ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_NOLOGO, argc, argv, &index, NULL)) {
            continue;
        }
        else if (parseOpt(M_O0, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O0 D3DCOMPILE_OPTIMIZATION_LEVEL0\n");
            }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL0;
            continue;
        }
        else if (parseOpt(M_O1, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL1\n");
            }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL1;
            continue;
        }
        else if (parseOpt(M_O2, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL2\n");
            }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL2;
            continue;
        }
        else if (parseOpt(M_O3, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL3\n");
             }
            sflags = sflags | D3DCOMPILE_OPTIMIZATION_LEVEL3;
            continue;
        }
        else if (parseOpt(M_OP, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Op D3DCOMPILE_NO_PRESHADER\n");
            }
            sflags = sflags | D3DCOMPILE_NO_PRESHADER;
            continue;
        }
        else if (parseOpt(M_P, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -P ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_DEBUG, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Qstrip_debug ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_PRIV, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Qstrip_priv ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_REFLECT, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Qstrip_reflect ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_ROOTSIGNATURE, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Qstrip_rootsignature ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_RES_MAY_ALIAS, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -res_may_alias D3DCOMPILE_RESOURCES_MAY_ALIAS\n");
            }
            sflags = sflags | D3DCOMPILE_RESOURCES_MAY_ALIAS;
            continue;
        }
        else if (parseOpt(M_SETPRIVATE, argc, argv, &index, NULL)) {
            fprintf(stderr, "option -setprivate not supported");
            print_unsupported_arg_help();
            M_TAREDOWN_PROG
            return 1;
        }
#ifdef _WIN32
        else if (parseOpt(M_T, argc, argv, &index, &w_temp)) {
            model = wcharToChar(w_temp);
            delete[] w_temp;
#else
        else if (parseOpt(M_T, argc, argv, &index, &model)) {
#endif
            if (verbose) {
                printf("option -T (Shader Model/Profile) with arg '%s'\n", model);
            }
            continue;
        }
        else if (parseOpt(M_VD, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zi D3DCOMPILE_DEBUG\n");
            }
            sflags = sflags | D3DCOMPILE_DEBUG;
            continue;
         }
        else if (parseOpt(M_VERSION, argc, argv, &index, NULL)) {
            print_version();
            M_TAREDOWN_PROG
            return 0;
        }
        else if (parseOpt(M_VI, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Vi (Output include process details) acknowledged but ignored.\n");
            }
            continue;
        }
#ifdef _WIN32
        else if (parseOpt(M_VN, argc, argv, &index, &w_temp)) {
            variableName = wcharToChar(w_temp);
            delete[] w_temp;
#else
        else if (parseOpt(M_VN, argc, argv, &index, &variableName)) {
#endif
            if (verbose) {
                printf("option -Vn (Variable Name) with arg '%s'\n", variableName);
            }
            continue;
        }
        else if (parseOpt(M_WX, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -WX D3DCOMPILE_WARNINGS_ARE_ERRORS\n");
            }
            sflags = sflags | D3DCOMPILE_WARNINGS_ARE_ERRORS;
            continue;
        }
        else if (parseOpt(M_ZD, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zd D3DCOMPILE_SKIP_VALIDATION\n");
            }
            sflags = sflags | D3DCOMPILE_SKIP_VALIDATION;
            continue;
         }
        else if (parseOpt(M_ZI, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zi D3DCOMPILE_DEBUG\n");
            }
            sflags = sflags | D3DCOMPILE_DEBUG;
            continue;
        }
        else if (parseOpt(M_ZPC, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zpc D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR\n");
            }
            sflags = sflags | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
            continue;
        }
        else if (parseOpt(M_ZPR, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zpr D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n");
            }
            sflags = sflags | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
            continue;
            }
        else if (parseOpt(M_ZSB, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zsb D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n");
            }
            sflags = sflags | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
            continue;
        }
        else if (parseOpt(M_ZSS, argc, argv, &index, NULL)) {
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
#else
                inputFile = new char[strlen(argv[index]) + 1];
                strncpy(inputFile, argv[index], strlen(argv[index]) + 1);
#endif

                if (verbose) {
#ifdef _WIN32
                    wprintf(L"input file: %ls\n", inputFile);
#else
                    printf("input file: %ls\n", inputFile);
#endif
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

    if (inputFile == NULL)
        print_usage_missing("inputFile");
    if (model == NULL)
        print_usage_missing("model");
    if (entryPoint == NULL)
        print_usage_missing("entryPoint");
    if (defines == NULL)
        print_usage_missing("defines");
    if (outputFile == NULL)
        print_usage_missing("outputFile");

    //Default output variable name
    if (variableName == NULL) {
        const char* prefix = "g";
        for (int i = 0; g_profilePrefixTable[i].name != NULL; i++) {
            if (strcmp(g_profilePrefixTable[i].name, model) == 0) {
                prefix = g_profilePrefixTable[i].prefix;
                break;
            }
        }
        variableName = (char*)malloc(strlen(prefix) + strlen(entryPoint) + 2);
#ifdef _WIN32
        sprintf_s(variableName, strlen(prefix) + strlen(entryPoint) + 2, "%s_%s", prefix, entryPoint);
#else
        sprintf(variableName, "%s_%s", prefix, entryPoint);
#endif
    }

    // ====================================================================================
    // Shader Compilation

    HMODULE h = LoadLibrary(DLL_NAME);
    if (h == NULL) {
#ifdef _WIN32
        //Find the WINDOWS dll
        wchar_t dllPath[MAX_PATH];
        size_t bytes = GetModuleFileName(NULL, dllPath, MAX_PATH);
        if (bytes == 0) {
            fprintf(stderr, "Could not retrieve the directory of the running executable.\n");
            print_windows_error();
            M_TAREDOWN_PROG
        }
        //Fill rest of the buffer with NULLs
        memset(dllPath + bytes, '\0', MAX_PATH - bytes);
        //Copy the dll location over top efxc2.exe
        wcscpy_s(dllPath + bytes, MAX_PATH, DLL_NAME);

        HMODULE h = LoadLibrary(dllPath);
        if (h == NULL) {
            wprintf("Error: could not load " DLL_NAME " from %s\n", dllPath);
        M_WINDOWS_ERROR
        }
#else
        M_WINDOWS_ERROR
#endif
     }
    size_t SourceLen;
    char* SourceCode = LoadSource(inputFile, &SourceLen);
    pD3DCompile2g ptr = (pD3DCompile2g)GetProcAddress(h, "D3DCompile2");
    if (ptr == NULL) {
        printf("Error: could not get the address of D3DCompileFromFile.\n");
        M_WINDOWS_ERROR
    }

    HRESULT hr;
    ID3DBlob* output = NULL;
    ID3DBlob* errors = NULL;

    if (verbose) {
        printf("Calling D3DCompile2(\n");

        printf("\t SourceCode,\n");
#ifdef _WIN32
#ifdef _WIN64
        wprintf(L"\t %" PRIu64 ",\n", SourceLen);
#else   /* _WIN64 */
        wprintf(L"\t %u ,\n", SourceLen);
#endif  /* _WIN64 */
        printf("\t %s, \n", c_inputFile);
#else   /* _WIN32 */
        printf("\t %u ,\n", SourceLen);
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
        printf("\t 0x%016" PRIx64 ", \n",(INT64) sflags);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)eflags);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)secondary_flags);
        printf("\t NULL,\n");
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
    hr = ptr(
        SourceCode,
        SourceLen,
#ifdef _WIN32
        c_inputFile,
#else
        inputFile,
#endif
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        model,
        sflags,
        eflags,
        secondary_flags,
        NULL,
        0,
        &output,
        &errors
    );

    // ====================================================================================
    // Output (or errors)

    if (FAILED(hr)) {
        if (errors) {
            char* error = (char*)errors->GetBufferPointer();
            fprintf(stderr, "Got an error while compiling:\n%s\n", error);
            errors->Release();
            printf("Error Code: 0x%x", hr);
        }
        else {
            fprintf(stderr, "Got an error (%x) while compiling, but no error message from the function.\n", hr);
            print_hresult_error(hr);
        }

        if (output)
            output->Release();
        free(SourceCode);
        M_TAREDOWN_PROG
        return 1;
    }
    else {
        unsigned char* outString = (unsigned char*)output->GetBufferPointer();
        size_t len = output->GetBufferSize();

        FILE* f;
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
        errno_t err = _wfopen_s(&f, outputFile, L"w");
#ifdef _MSC_VER
#pragma warning( pop )
#endif
        if (errno != 0) {
            free(SourceCode);
            print_errno();
        }
#else
        f = fopen(outputFile, "w");
        if (f = NULL) {
            free(SourceCode);
            print_errno();
        }
#endif

        if (cmd == CMD_WRITE_HEADER) {
            fprintf(f, "const BYTE %s[] =\n{\n", variableName);
            for (int i = 0; i < len; i++) {
                if (outputHex) {
                    fprintf(f, " 0x%02" PRIx8, outString[i]);
                } else {
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
        if (cmd == CMD_WRITE_OBJECT) {
            fwrite(outString, len, 1, f);
        }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
        fclose(f);
#ifdef _MSC_VER
#pragma warning( pop )
#endif
        if (errno != 0) {
            print_errno();
        }
        if (verbose) {
#ifdef _WIN32
#ifdef _WIN64
            wprintf(L"Wrote %" PRIu64 " bytes of shader output to %ls\n", len, outputFile);
#else
            wprintf(L"Wrote %u bytes of shader output tp %ls\n", len, outputFile);
#endif
#else
            printf("Wrote %u bytes of shader output tp %ls\n", len, outputFile);
#endif
        }
    }
#ifdef _WIN32
    free(c_inputFile);
#endif
    free(SourceCode);
    M_TAREDOWN_PROG
  return 0;
}