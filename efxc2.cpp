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


void FindNOLOGO(const M_CMD_PARAMS& args, CompilerParams& params) {
    /*first scan specifically for the nologo argument so no output
    is given regardless of parameter order*/
    size_t index = 0;
    while (index < args.size()) {
        /* Detect the end of the options. */
        if (parseOpt(M_NOLOGO, args, &index, nullptr)) {
            params.set_verbose(false);
            break;
        }
        else {
            index++;
        }
    }
    return;
}

void FindDebug(const M_CMD_PARAMS& args, CompilerParams& params) {
    /* scan for debug parameter*/
    size_t index = 0;
    while (index < args.size()) {
        /* Detect the end of the options. */
        if (parseOpt(M_DEBUG, args, &index, nullptr)) {
            params.set_debug(true);
            break;
        }
        else {
            index++;
        }
    }
    return;
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

    M_CMD_PARAMS args(argv + 1, argv + argc);
    M_STRING temp = M_STRING_INIT;
    CompilerAPIContainer api;
    CompilerParams params;
    Files files;

    FindNOLOGO(args, params);
    FindDebug(args, params);

    size_t index = 0;
    /*now scan for all arguments and input file name*/
    index = 0;
    while (TRUE) {
        /* Detect the end of the options. */
        if (index >= args.size())
            break;
        else if (parseOpt(M_QUESTION_MARK, args, &index, nullptr)) {
            print_help_screen();
        }
        else if (parseCompilerOnlyCall(args, &index, params)) {
            continue;
        }
        else if (parseIgnoredOptions(args, &index, params)) {
            continue;
        }
        else if (parseNotSupportedOptions(args, &index)) {
            /* If true, this will not return.  It exits the program .*/
        }
        else if (parseOpt(M_D, args, &index, &temp)) {
            cmd_D(params, temp);
            continue;
        }
        else if (parseOpt(M_E_, args, &index, &temp)) {
            cmd_E(params, temp);
            continue;
        }
        else if (parseCompilerFileCall(args, &index, params, files)) {
            continue;
        }
        else if (parseOpt(M_HELP, args, &index, nullptr)) {
            print_help_screen();
        }
        else if (parseOpt(M_I, args, &index, &temp)) {
            cmd_I(params, temp);
            continue;
        }
        else if (parseOpt(M_NOLOGO, args, &index, nullptr)) {
            continue;
        }
        else if (parseOpt(M_DEBUG, args, &index, nullptr)) {
            continue;
        }
        else if (parseOpt(M_T, args, &index, &temp)) {
            cmd_T(params, temp);
            continue;
        }
        else if (parseOpt(M_VERSION, args, &index, nullptr)) {
            print_version();
        }
        else if (parseOpt(M_VN, args, &index, &temp)) {
            cmd_Vn(params, temp);
            continue;
        }
        else {
            parseInputFile(args[index], params, files);
            index += 1;
        }
    }
    Compiler compiler(api, params);
    CompilerTasks(compiler, files, params);
    return 0;
}