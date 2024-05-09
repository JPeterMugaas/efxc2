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

static void FindNOLOGO(const efxc2Utils::M_CMD_PARAMS& args, efxc2CompilerParams::CompilerParams& params) {
    /*first scan specifically for the nologo argument so no output
    is given regardless of parameter order*/
    size_t index = 0;
    while (index < args.size()) {
        /* Detect the end of the options. */
        if (efxc2Utils::parseOpt(M_NOLOGO, args, &index, nullptr)) {
            params.set_verbose(false);
            break;
        }
        else {
            index++;
        }
    }
    return;
}

static void FindDebug(const efxc2Utils::M_CMD_PARAMS& args, efxc2CompilerParams::CompilerParams& params) {
    /* scan for debug parameter*/
    size_t index = 0;
    while (index < args.size()) {
        /* Detect the end of the options. */
        if (efxc2Utils::parseOpt(M_DEBUG, args, &index, nullptr)) {
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

    efxc2Utils::M_CMD_PARAMS args(argv + 1, argv + argc);  //-V104
    efxc2Utils::M_STRING temp = efxc2Utils::M_STRING_INIT;
    efxc2CompilerAPIContainer::CompilerAPIContainer api;
    efxc2CompilerParams::CompilerParams params;
    efxc2Files::Files files;

    FindNOLOGO(args, params);
    FindDebug(args, params);

    size_t index = 0;
    /*now scan for all arguments and input file name*/
    index = 0;
    while (TRUE) {
        /* Detect the end of the options. */
        if (index >= args.size()) {
            break;
        }
        else if (efxc2Utils::parseOpt(M_QUESTION_MARK, args, &index, nullptr)) {
            efxc2Utils::print_help_screen();
        }
        else if (efxc2Cmds::parseCompilerOnlyCall(args, &index, params)) {
            continue;
        }
        else if (efxc2Cmds::parseIgnoredOptions(args, &index, params)) {
            continue;
        }
        else if (efxc2Cmds::parseNotSupportedOptions(args, &index)) {
            exit(1);
        }
        else if (efxc2Utils::parseOpt(M_D, args, &index, &temp)) {
            efxc2Cmds::cmd_D(params, temp);
            continue;
        }
        else if (efxc2Utils::parseOpt(M_E_, args, &index, &temp)) {
            efxc2Cmds::cmd_E(params, temp);
            continue;
        }
        else if (efxc2Cmds::parseCompilerFileCall(args, &index, params, files)) {
            continue;
        }
        else if (efxc2Utils::parseOpt(M_HELP, args, &index, nullptr)) {
            efxc2Utils::print_help_screen();
        }
        else if (efxc2Utils::parseOpt(M_I, args, &index, &temp)) {
            efxc2Cmds::cmd_I(params, temp);
            continue;
        }
        else if (efxc2Utils::parseOpt(M_NOLOGO, args, &index, nullptr)) {
            continue;
        }
        else if (efxc2Utils::parseOpt(M_DEBUG, args, &index, nullptr)) {
            continue;
        }
        else if (efxc2Utils::parseOpt(M_T, args, &index, &temp)) {
            efxc2Cmds::cmd_T(params, temp);
            continue;
        }
        else if (efxc2Utils::parseOpt(M_VERSION, args, &index, nullptr)) {
            efxc2Utils::print_version();
        }
        else if (efxc2Utils::parseOpt(M_VN, args, &index, &temp)) {
            efxc2Cmds::cmd_Vn(params, temp);
            continue;
        }
        else {
            efxc2Cmds::parseInputFile(args[index], params, files);
            index += 1;
        }
    }
    efxc2Compiler::Compiler compiler(api, params);
    efxc2CompilerTasks::CompilerTasks(compiler, files, params);
    return 0;
}