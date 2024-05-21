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
        
#ifdef _WIN32
        void Initialize() {
            std_output = GetStdHandle(STD_OUTPUT_HANDLE);
            std_error = GetStdHandle(STD_ERROR_HANDLE);
            (void)GetConsoleMode(std_output, &stdout_orig_console_mode);
            (void)GetConsoleMode(std_error, &stderr_orig_console_mode);
            (void)SetConsoleMode(std_output, stdout_orig_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            (void)SetConsoleMode(std_error, stderr_orig_console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#else
        void Initialize() const {
#endif
        }
        
#ifdef _WIN32
        void Shutdown() {
            (void)SetConsoleMode(std_output, stdout_orig_console_mode);
            (void)SetConsoleMode(std_error, stderr_orig_console_mode);
#else
        void Shutdown() const {
#endif
        }

        void std_out_pink() const {
            if (IsStdOutputAConsole()) {
                std::cout << term_printPink;
            }
        };
        void std_out_reset() const {
            if (IsStdOutputAConsole()) {
                std::cout << term_printLtReset;
            }
        };
        void std_err_pink() const {
            if (IsStdErrAConsole()) {
                std::cerr << term_printPink;
            }
        };
        void std_err_reset() const {
            if (IsStdErrAConsole()) {
                std::cerr << term_printLtReset;
            }
        };
        void PinkOutput() const {
            std_out_pink();
            std_err_pink();
        }
        void ResetOutput() const {
            std_out_reset();
            std_err_reset();
        }
    private:
#ifdef _WIN32
        HANDLE std_error = nullptr; //-V122
        DWORD stderr_orig_console_mode = 0;
        HANDLE std_output = nullptr; //-V122
        DWORD stdout_orig_console_mode = 0;
#endif
        bool IsStdErrAConsole() const {
#ifdef _WIN32
            return (GetFileType(std_output) == FILE_TYPE_CHAR);
#else
            return (isatty(STDERR_FILENO));
#endif
        }
        bool IsStdOutputAConsole() const {
#ifdef _WIN32
            return (GetFileType(std_error) == FILE_TYPE_CHAR);
#else
            return (isatty(STDOUT_FILENO));
#endif
        }
    };
     const Console console;
}