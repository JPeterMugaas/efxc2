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
#include "efxc2CompilerTasks.h"
#include "efxc2Files.h"

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
    FILE* f;
    CompilerAPIContainer api;
    Compiler compiler(api);
    Files files;

    /*first scan specifically for the nologo argument so no output
    is given regardless of parameter order*/
    int index = 1;
    while (index < argc) {
        /* Detect the end of the options. */
        if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
            compiler.set_verbose(false);
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
        else if (parseCompilerOnlyCall(argc, argv, &index, compiler)) {
            continue;
        }
        else if (parseIgnoredOpts(argc, argv, &index, compiler)) {
            continue;
        }
        else if (parseNotSupportedOpts(argc, argv, &index)) {
            ;
        }
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif /* _MSC_VER*/
        else if (parseOpt(M_D, argc, argv, &index, &temp)) {
            cmd_D(compiler, temp);
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
            continue;
        }
        else if (parseOpt(M_E_, argc, argv, &index, &temp)) {
            cmd_E(compiler, temp);
#ifdef _WIN32
            delete[] temp;
#endif
            continue;
        }
        else if (parseOpt(M_FD, argc, argv, &index, &temp)) {
            cmd_Fd(compiler, files, temp);
            continue;
        }
        else if (parseOpt(M_FH, argc, argv, &index, &temp)) {
            cmd_Fh(compiler, files, temp);
            continue;
        }
        else if (parseOpt(M_FO, argc, argv, &index, &temp)) {
            cmd_Fo(compiler, files, temp);
            continue;
        }
        else if (parseOpt(M_HELP, argc, argv, &index, nullptr)) {
            print_help_screen();
        }
        else if (parseOpt(M_NOLOGO, argc, argv, &index, nullptr)) {
            continue;
        }
        else if (parseOpt(M_T, argc, argv, &index, &temp)) {
            cmd_T(compiler, temp);
#ifdef _WIN32
            delete[] temp;
#endif
            continue;
        }
        else if (parseOpt(M_VERSION, argc, argv, &index, nullptr)) {
            print_version();
        }
        else if (parseOpt(M_VN, argc, argv, &index, &temp)) {
            cmd_Vn(compiler, temp);
#ifdef _WIN32
            delete[] temp;
#endif /* _WIN32 */
            continue;
        }
        else {
#ifdef _WIN32
            parseInputFile(argv[index], compiler, files);
#else
            parseInputFile(argv[index], compiler, files);
#endif
            index += 1;
        }
    }

#ifdef _WIN32
    CompilerTasks(compiler, files);
#else
    CompilerTasks(compiler, files);
#endif
    return 0;
}