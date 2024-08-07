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
#include "efxc2Console.h"
#include "efxc2Files.h"

/*Cygwin and MSYS2 compilers amd linkers don't support
the wmain -Municode entry-point*/

#ifdef _WIN32
int wmain(int argc, const wchar_t* argv[]) {
#else  /* _WIN32 */
int main(int argc, const char* argv[]) {
#endif /* _WIN32 */
	efxc2Console::Console console;
	console.Initialize();
	int result = 0;
	try {
		// ====================================================================================
		// Process Command Line Arguments

		efxc2Utils::M_CMD_PARAMS args(&argv[1], &argv[argc]);
		efxc2Utils::M_STRING temp = efxc2Utils::M_STRING_INIT;
		efxc2CompilerAPIContainer::CompilerAPIContainer api;
		efxc2CompilerParams::CompilerParams params;
		efxc2Files::Files files;

		efxc2Cmds::FindNOLOGO(args, params);
		efxc2Cmds::FindDebug(args, params);
		int immediate_exit = 0;

		size_t index = 0;
		/*now scan for all arguments and input file name*/
		index = 0;
		while (TRUE) {
			/* Detect the end of the options. */
			if ((index >= args.size()) || (immediate_exit > 0)) {
				break;
			}
			else if ((efxc2Utils::parseOpt(efxc2Cmds::M_QUESTION_MARK, args, &index, nullptr)) ||
				(efxc2Utils::parseOpt(efxc2Cmds::M_HELP, args, &index, nullptr))) {
				efxc2Utils::print_help_screen();
				immediate_exit = 1;
				continue;
			}
			else if (efxc2Cmds::parseCompilerOnlyCall(args, &index, params)) {
				continue;
			}
			else if (efxc2Cmds::parseIgnoredOptions(args, &index, params)) {
				continue;
			}
			else if (efxc2Cmds::parseNotSupportedOptions(args, &index)) {
				result = 1;
				immediate_exit = 1;
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_D, args, &index, &temp)) {
				efxc2Cmds::cmd_D(params, temp);
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_E_, args, &index, &temp)) {
				efxc2Cmds::cmd_E(params, temp);
				continue;
			}
			else if (efxc2Cmds::parseCompilerFileCall(args, &index, params, files)) {
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_I, args, &index, &temp)) {
				efxc2Cmds::cmd_I(params, temp);
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_NOLOGO, args, &index, nullptr)) {
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_DEBUG, args, &index, nullptr)) {
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_T, args, &index, &temp)) {
				efxc2Cmds::cmd_T(params, temp);
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_VERSION, args, &index, nullptr)) {
				efxc2Utils::print_version();
				immediate_exit = 1;
				continue;
			}
			else if (efxc2Utils::parseOpt(efxc2Cmds::M_VN, args, &index, &temp)) {
				efxc2Cmds::cmd_Vn(params, temp);
				continue;
			}
			else {
				efxc2Cmds::parseInputFile(args[index], params, files);
				index += 1;
			}
		}
		if (immediate_exit == 0) {
			efxc2Compiler::Compiler compiler(api, params);
			efxc2CompilerTasks::CompilerTasks(compiler, files, params);
		}
	}
	catch (...) {
		/*We already reported the error to the user. */
		result = 1;
	}
	console.Shutdown();
	return result;
}