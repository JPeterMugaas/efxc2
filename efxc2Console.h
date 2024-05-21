//--------------------------------------------------------------------------------------
// File: efxc2Console.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#pragma once
#include "efxc2.h"

namespace efxc2Console {
    /*from: https://prirai.github.io/blogs/ansi-esc/#colors-graphics-mode */

    const std::string term_printPink = "\x1b"
        "[38;5;210m";
    const std::string term_printLtReset = "\x1b"
        "[1;00";
    class Console {
    public:
        Console() {
#ifdef _WIN32
            std_output = GetStdHandle(STD_OUTPUT_HANDLE);
            std_error = GetStdHandle(STD_ERROR_HANDLE);
            (void)GetConsoleMode(std_output, &stdout_orig_console_mode);
            (void)GetConsoleMode(std_error, &stderr_orig_console_mode);
#endif
#ifdef _WIN32
            (void)SetConsoleMode(std_output, stdout_orig_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            (void)SetConsoleMode(std_error, stderr_orig_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        };
        virtual ~Console() {
#ifdef _WIN32
            (void)SetConsoleMode(std_output, stdout_orig_console_mode);
            (void)SetConsoleMode(std_error, stderr_orig_console_mode);
#endif
        };
        void std_out_pink() {
            if (IsStdOutputAConsole()) {
                std::cout << term_printPink;
            }
        };
        void std_out_reset()  {
            if (IsStdOutputAConsole()) {
                std::cout << term_printLtReset;
            }
        };
        void std_err_pink() {
            if (IsStdErrAConsole()) {
                std::cerr << term_printPink;
            }
        };
        void std_err_reset() {
            if (IsStdErrAConsole()) {
                std::cerr << term_printLtReset;
            }
        };
    private:
#ifdef _WIN32
        HANDLE std_error = nullptr; //-V122
        DWORD stderr_orig_console_mode = 0;
        HANDLE std_output = nullptr; //-V122
        DWORD stdout_orig_console_mode = 0;
#endif
        bool IsStdErrAConsole() {
#ifdef _WIN32
            return (GetFileType(std_output) == FILE_TYPE_CHAR);
#else
            return (isatty(STDERR_FILENO));
#endif
        }
        bool IsStdOutputAConsole() {
#ifdef _WIN32
            return (GetFileType(std_error) == FILE_TYPE_CHAR);
#else
            return (isatty(STDOUT_FILENO));
#endif
        }
    };
     const Console console;
}