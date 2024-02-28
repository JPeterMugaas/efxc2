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
void FixupFileName(_Inout_ wchar_t* FileName);
bool parseOpt(_In_ const wchar_t* option, _In_ int argc, _In_ wchar_t* argv[1], _Inout_ int* index, _Inout_opt_ wchar_t** argumentOption);
#else  /* _WIN32 */
bool parseOpt(_In_ const char* option, _In_ int argc, _In_ char* argv[1], _Inout_ int* index, _Inout_opt_ char** argumentOption);
#endif /* _WIN32 */
void print_errno(errno_t _errno);
void print_errno(void);
int readall(_In_ FILE* in, _Out_writes_bytes_(*sizeptr) char** dataptr, _Out_opt_ size_t* sizeptr);
#ifdef _WIN32
char* wcharToChar(_In_ LPCWSTR w);
#endif