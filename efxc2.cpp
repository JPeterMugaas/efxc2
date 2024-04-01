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
#include "efxc2CompilerParams.h"
#include "efxc2CompilerTasks.h"
#include "efxc2Files.h"

using namespace std;

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
    wchar_t* temp = nullptr;
#else
    char* temp = nullptr;
#endif
    CompilerAPIContainer api;
    CompilerParams params;
    Files files;

    /*first scan specifically for the nologo argument so no output
    is given regardless of parameter order*/
    int index = 1;
    while (index < argc) {
        /* Detect the end of the options. */
        if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
            params.set_verbose(false);
            break;
        }
        else {
            index++;
        }
    }

    /*now scan for all arguments and input file name*/
    index = 1;
    while (TRUE) {
        /* Detect the end of the options. */
        if (index >= argc)
            break;
        else if (parseOpt(M_QUESTION_MARK, argc, argv, &index, nullptr)) {
            print_help_screen();
        }
        else if (parseCompilerOnlyCall(argv, &index, params)) {
            continue;
        }
        else if (parseIgnoredOpts(argv, &index, params)) {
            continue;
        }
        else if (parseNotSupportedOpts(argv, &index)) {
            /* If true, this will not return.  It exits the program .*/
        }
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif /* _MSC_VER*/
        else if (parseOpt(M_D, argc, argv, &index, &temp)) {
            cmd_D(params, temp);
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
            continue;
        }
        else if (parseOpt(M_E_, argc, argv, &index, &temp)) {
            cmd_E(params, temp);
#ifdef _WIN32
            delete[] temp;
#endif
            continue;
        }
        else if (parseCompilerFileCall(argc, argv, &index, params, files)) {
            continue;
        }
        else if (parseOpt(M_HELP, argc, argv, &index, nullptr)) {
            print_help_screen();
        }
        else if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
            continue;
        }
        else if (parseOpt(M_T, argc, argv, &index, &temp)) {
            cmd_T(params, temp);
#ifdef _WIN32
            delete[] temp;
#endif
            continue;
        }
        else if (parseOpt(M_VERSION, argc, argv, &index, nullptr)) {
            print_version();
        }
        else if (parseOpt(M_VN, argc, argv, &index, &temp)) {
            cmd_Vn(params, temp);
#ifdef _WIN32
            delete[] temp;
#endif /* _WIN32 */
            continue;
        }
        else {
#ifdef _WIN32
            parseInputFile(argv[index], params, files);
#else
            parseInputFile(argv[index], params, files);
#endif
            index += 1;
        }
    }
    Compiler compiler(api, params);
#ifdef _WIN32
    CompilerTasks(compiler, files, params);
#else
    CompilerTasks(compiler, files, params);
#endif
    return 0;
}