//--------------------------------------------------------------------------------------
// File: efxc2Utils.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#pragma once
#include "efxc2.h"

#ifdef _WIN32
void FixupFileName(wchar_t* FileName);
bool parseOpt(const wchar_t* option, int argc, wchar_t* argv[1], int* index, wchar_t** argumentOption);
#else  /* _WIN32 */
bool parseOpt(const char* option, int argc, char* argv[1], int* index, char** argumentOption);
#endif /* _WIN32 */
void print_errno();
int readall(FILE* in, char** dataptr, size_t* sizeptr);
char* wcharToChar(const wchar_t* w);