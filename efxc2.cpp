//--------------------------------------------------------------------------------------
// File: efxc2.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2.h"

void print_version() {
    printf(PROGRAM_DESCRIPTION " version "  PROGRAM_VERSION "\n");
    printf(PROGRAM_COPYRIGHT "\n");
    exit(0);
}
void print_usage_arg() {
    printf("More information about valid parameters is available at Microsoft's website \n");
    printf("\n");
    printf("https://msdn.microsoft.com/en-us/library/windows/desktop/bb509709(v=vs.85).aspx\n");
}

void print_invalid_arg() {
    fprintf(stderr, "You have specified an argument that is not handled by efxc2\n");
    printf("This isn't a sign of disaster, odds are it will be very easy to add support for\n");
    printf("this argument.  Review the meaning of the argument in the real fxc program, and\n");
    printf("then add it into efxc2.\n");
    printf("\n");
    print_usage_arg();
    exit(1);
}

void print_usage_missing(const char* arg) {
    fprintf(stderr, "efxc2 is missing the %s argument.\n", arg);
    printf("We expected to receive this, and it's likely things will not work correctly\n");
    printf("without it.Review efxc2 and make sure things will work.\n");
    printf("\n");
    print_usage_arg();
    exit(1);
}
void print_usage_toomany() {
    fprintf(stderr, "You specified multiple input files.\n");
    printf("We did not expect to receive this, and aren't prepared to handle multiple input\n"); 
    printf("files. You'll have to edit the source to behave the way you want.\n");
    printf("\n");
    print_usage_arg();
    exit(1);
}

void print_errno() {
    char errmsg[ERR_SIZE];
#ifdef _WIN32
    strerror_s(errmsg, ERR_SIZE, errno);
#else
    /*int strerror_r(int errnum, char *buf, size_t buflen);*/
    strerror_r(errno, errmsg, ERR_SIZE);
#endif
    fprintf(stderr, "%s\n", errmsg);
    exit(1);
}

void print_hresult_error(const HRESULT hr) {
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    fprintf(stderr, "Windows Error Message: %s\n", messageBuffer);
    exit(1);
}

void print_windows_error() {
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

#ifdef _WIN32
bool parseOpt(const wchar_t* option, int argc, wchar_t* argv[1], int* index, wchar_t** argumentOption) {
#else
bool parseOpt(const char* option, int argc, char* argv[1], int* index, char** argumentOption) {
#endif
    assert(option != NULL);
    if (!index || *index >= argc) {
        return false;
    }
#ifdef _WIN32
    const wchar_t* 
#else
    const char*
#endif
        argument = argv[*index];
    if (argument[0] == '-' || argument[0] == '/') {
        argument++;
        if (argument[0] == '-') {
            argument++;
        }
    }
    else
        return false;

#ifdef _WIN32
    size_t optionSize = wcslen(option);
    if (wcsncmp(argument, option, optionSize) != 0) {
#else
    size_t optionSize = strlen(option);
    if (strncmp(argument, option, optionSize) != 0) {
#endif
        return false;
    }

    if (argumentOption) {
        argument += optionSize;
        if (*argument == '\0') {
            *index += 1;
            if (*index >= argc) {
                fprintf(stderr,"Error: missing required argument for option %ls\n", option);
                return false;
            }
#ifdef _WIN32
            *argumentOption = M_WCSDUP(argv[*index]);
#else
            *argumentOption = strdup(argv[*index]);
#endif
        }
        else {
#ifdef _WIN32
            *argumentOption = M_WCSDUP(argument);
#else
            * argumentOption = strdup(argument);
#endif
        }
    }
    *index += 1;
    return true;
}

char* wcharToChar(const wchar_t* w) {
    size_t len = wcslen(w);
    char* c = NULL;
    c = (char*)malloc(len + 1);
    if (c == NULL) {
        fprintf(stderr, "malloc failed/n");
        print_errno();
        exit(1);
    }
    memset(c, 0, len + 1);
    wcstombs(c, w, len);
    if (errno != 0) {
        fprintf(stderr, "wcstombs failed/n");
        print_errno();
        exit(1);
    }
    return c; 
}

#ifdef _WIN32
void FixupFileName(wchar_t* FileName) {
  size_t i;
  if (FileName == NULL) {
      return;
  }
  for (i = 0; FileName[i] != '\0'; i++)
  {
    if (FileName[i] == '/') {
        FileName[i] = '\\';
    }
    else
        continue;
  }
  return;
}
#endif

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

#define M_WINDOWS_ERROR \
   print_windows_error(); \
   M_TAREDOWN_PROG \
   return 1;

/*Cygwin and MSYS2 compilers amd linkers don't support 
the wmain -Municode entry-point*/

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
#else
int main(int argc, char* argv[]) {
#endif
    // ====================================================================================
    // Process Command Line Arguments

    wchar_t* inputFile = NULL;
#ifdef _WIN32
    wchar_t* outputFile = NULL;
#else
    char* outputFile = NULL;
#endif
#ifdef _WIN32
    wchar_t* w_temp = NULL;
#endif

    char* defineOption = NULL;
    char* entryPoint = NULL;
    char* model = NULL;
    char* variableName = NULL;

    UINT flags1 = 0;
    int verbose = 1;

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

        if (parseOpt(M_NOLOGO, argc, argv, &index, NULL)) {
            continue;
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
#ifdef _WIN32
        else if (parseOpt(M_E, argc, argv, &index, &w_temp)) {
            entryPoint = wcharToChar(w_temp);
            delete[] w_temp;
#else
        else if (parseOpt(M_E, argc, argv, &index, &entryPoint)) {
#endif
            if (verbose) {
                printf("option -E (Entry Point) with arg '%s'\n", entryPoint);
            }
            continue;
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
        else if (parseOpt(M_VI, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Vi (Output include process details) acknowledged but ignored.\n");
            }
            continue;
        }
        else if (parseOpt(M_FH, argc, argv, &index, &outputFile)) {
#ifdef _WIN32
            FixupFileName(outputFile);
#endif
            if (cmd != 0) {
                printf("You cannot specify both an object and header");
                return 1;
            }
            cmd = CMD_WRITE_HEADER;
            if (verbose) {
                printf("option -Fh (Output File) with arg %ls\n", outputFile);
            }
            continue;
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
        else if (parseOpt(M_FD, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Fd ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_ZI, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zi D3DCOMPILE_DEBUG\n");
            }
            flags1 = flags1 | D3DCOMPILE_DEBUG;
            continue;
        }
        else if (parseOpt(M_ZD, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zd D3DCOMPILE_SKIP_VALIDATION\n");
            }
            flags1 = flags1 | D3DCOMPILE_SKIP_VALIDATION;
            continue;
        }
        else if (parseOpt(M_VD, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zi D3DCOMPILE_SKIP_OPTIMIZATION\n");
            }
            flags1 = flags1 | D3DCOMPILE_SKIP_OPTIMIZATION;
            continue;
        }
        else if (parseOpt(M_ZPC, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zpc D3DCOMPILE_PACK_MATRIX_ROW_MAJOR\n");
            }
            flags1 = flags1 | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
            continue;
        }
        else if (parseOpt(M_GPP, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gpp D3DCOMPILE_PARTIAL_PRECISION\n");
            }
            flags1 = flags1 | D3DCOMPILE_PARTIAL_PRECISION;
            continue;
        }
        else if (parseOpt(M_OP, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Op D3DCOMPILE_NO_PRESHADER\n");
            }
            flags1 = flags1 | D3DCOMPILE_NO_PRESHADER;
            continue;
        }
        else if (parseOpt(M_GFA, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gfa D3DCOMPILE_AVOID_FLOW_CONTROL\n");
            }
            flags1 = flags1 | D3DCOMPILE_AVOID_FLOW_CONTROL;
            continue;
        }
        else if (parseOpt(M_WX, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -WX D3DCOMPILE_WARNINGS_ARE_ERRORS\n");
            }
            flags1 = flags1 | D3DCOMPILE_WARNINGS_ARE_ERRORS;
            continue;
        }
        else if (parseOpt(M_GES, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Ges D3DCOMPILE_ENABLE_STRICTNESS\n");
            }
            flags1 = flags1 | D3DCOMPILE_ENABLE_STRICTNESS;
            continue;
        }
        else if (parseOpt(M_GIS, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gis D3DCOMPILE_IEEE_STRICTNESS\n");
            }
            flags1 = flags1 | D3DCOMPILE_IEEE_STRICTNESS;
            continue;
        }
        else if (parseOpt(M_GEC, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Gec D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY\n");
            }
            flags1 = flags1 | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
            continue;
        }
        else if (parseOpt(M_O0, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O0 D3DCOMPILE_OPTIMIZATION_LEVEL0\n");
            }
            flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL0;
            continue;
        }
        else if (parseOpt(M_O1, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL1\n");
            }
            flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL1;
            continue;
        }
        else if (parseOpt(M_O2, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL2\n");
            }
            flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL2;
            continue;
        }
        else if (parseOpt(M_O3, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -O1 D3DCOMPILE_OPTIMIZATION_LEVEL3\n");
            }
            flags1 = flags1 | D3DCOMPILE_OPTIMIZATION_LEVEL3;
            continue;
        }
        else if (parseOpt(M_RES_MAY_ALIAS, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -res_may_alias D3DCOMPILE_RESOURCES_MAY_ALIAS\n");
            }
            flags1 = flags1 | D3DCOMPILE_RESOURCES_MAY_ALIAS;
            continue;
        }
        else if (parseOpt(M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -enable_unbounded_descriptor_tables D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES\n");
            }
            flags1 = flags1 | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
            continue;
        }
        else if (parseOpt(M_ALL_RESOURCES_BOUND, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -all_resources_bound D3DCOMPILE_ALL_RESOURCES_BOUND\n");
            }
            flags1 = flags1 | D3DCOMPILE_ALL_RESOURCES_BOUND;
            continue;
        }
        else if (parseOpt(M_ZSS, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zss D3DCOMPILE_DEBUG_NAME_FOR_SOURCE\n");
            }
            flags1 = flags1 | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
            continue;
        }
        else if (parseOpt(M_ZSB, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Zsb D3DCOMPILE_DEBUG_NAME_FOR_BINARY\n");
            }
            flags1 = flags1 | D3DCOMPILE_DEBUG_NAME_FOR_BINARY;
            continue;
        }
        else if (parseOpt(M_QUESTION_MARK, argc, argv, &index, NULL)) {
            print_usage_arg();
            M_TAREDOWN_PROG
            return 0;
        }
        else if (parseOpt(M_HELP, argc, argv, &index, NULL)) {
            print_usage_arg();
            M_TAREDOWN_PROG
            return 0;
        }
        else if (parseOpt(M_QSTRIP_REFLECT, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Qstrip_reflect ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_QSTRIP_DEBUG, argc, argv, &index, NULL)) {
            if (verbose) {
                printf("option -Qstrip_debug ignored\n");
            }
            continue;
        }
        else if (parseOpt(M_VERSION, argc, argv, &index, NULL)) {
            print_version();
            M_TAREDOWN_PROG
            return 0;
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
#else
                inputFile = new wchar_t[strlen(argv[index]) + 1];
                mbstowcs(inputFile, argv[index], strlen(argv[index]) + 1);
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
    pCompileFromFileg ptr = (pCompileFromFileg)GetProcAddress(h, "D3DCompileFromFile");
    if (ptr == NULL) {
        printf("Error: could not get the address of D3DCompileFromFile.\n");
        M_WINDOWS_ERROR
    }

    HRESULT hr;
    ID3DBlob* output = NULL;
    ID3DBlob* errors = NULL;

    if (verbose) {
        printf("Calling D3DCompileFromFile(\n");

#ifdef _WIN32
        wprintf(L"\t %ls,\n", inputFile);
#else
        printf("\t %ls,\n", inputFile);
#endif
        printf("\t");
        for (int i = 0; i < numDefines - 1; i++)
            printf(" %s=%s", defines[i].Name, defines[i].Definition);
        printf(",\n");
        printf("\t D3D_COMPILE_STANDARD_FILE_INCLUDE,\n");
        printf("\t %s,\n", entryPoint);
        printf("\t %s,\n", model);
        printf("\t 0x%016" PRIx64 ", \n",(INT64) flags1);

        printf("\t 0,\n");
        printf("\t &output,\n");
        printf("\t &errors);\n");
    }

    /*
    HRESULT WINAPI D3DCompileFromFile(
    in      LPCWSTR pFileName,
    in_opt  const D3D_SHADER_MACRO pDefines,
    in_opt  ID3DInclude pInclude,
    in      LPCSTR pEntrypoint,
    in      LPCSTR pTarget,
    in      UINT Flags1,
    in      UINT Flags2,
    out     ID3DBlob ppCode,
    out_opt ID3DBlob ppErrorMsgs
    );
    */
    hr = ptr(
        inputFile,
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        model,
        flags1,
        0,
        &output,
        &errors
    );

    // ====================================================================================
    // Output (or errors)

    if (FAILED(hr)) {
        if (errors) {
            char* error = (char*)errors->GetBufferPointer();
            fprintf(stderr, "Got an error (%li) while compiling:\n%s\n", hr, error);
            errors->Release();
            
        }
        else {
            fprintf(stderr, "Got an error (%li) while compiling, but no error message from the function.\n", hr);
            print_hresult_error(hr);
        }

        if (output)
            output->Release();
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
            print_errno();
        }
#else
        f = fopen(outputFile, "w");
        if (f = NULL) {
            print_errno();
        }
#endif

        if (cmd == CMD_WRITE_HEADER) {
            fprintf(f, "const BYTE %s[] =\n{\n", variableName);
            for (int i = 0; i < len; i++) {
                fprintf(f, "%4" PRIu8, outString[i]);
                if (i != len - 1)
                    fprintf(f, ",");
                if ((i % 6 == 5) && (i != len -1))
                    fprintf(f, "\n");
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
    M_TAREDOWN_PROG
  return 0;
}