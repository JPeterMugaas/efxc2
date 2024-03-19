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
#ifndef EFXC2UTILS_H_INCLUDED
#define EFXC2UTILS_H_INCLUDED
#include "efxc2.h"

/*from: https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer */
/* Size of each input chunk to be
   read and allocate for. */
const size_t  READALL_CHUNK = 262144;

const int  READALL_OK = 0;  /* Success */
const int  READALL_INVALID = -1;  /* Invalid parameters */
const int  READALL_ERROR = -2;  /* Stream error */
const int  READALL_TOOMUCH = -3;  /* Too much input */
const int  READALL_NOMEM = -4;  /* Out of memory */

/* Helper D3D error lookup table*/
struct ErrorTableEntry {
    HRESULT ErrorCode;
    const char* ErrorName;
};

constexpr auto ERROR_TABLE_LENGTH = 11;

const std::array <ErrorTableEntry, ERROR_TABLE_LENGTH >g_ErrorTable = { {
    { (HRESULT) 0x887C0002, "D3D11_ERROR_FILE_NOT_FOUND" },
    { (HRESULT) 0x887C0001, "D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS" },
    { (HRESULT) 0x887C0003, "D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS" },
    { (HRESULT) 0x887C0004, "D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD" },
    { (HRESULT) 0x887A0001, "D3DERR_INVALIDCALL (replaced with DXGI_ERROR_INVALID_CALL)" },
    { (HRESULT) 0x887A000A, "D3DERR_WASSTILLDRAWING (replaced with DXGI_ERROR_WAS_STILL_DRAWING)" },
    { (HRESULT) 0x80004005, "E_FAIL" },
    { (HRESULT) 0x80070057, "E_INVALIDARG" },
    { (HRESULT) 0x8007000E, "E_OUTOFMEMORY" },
    { (HRESULT) 0x80004001, "E_NOTIMPL" },
    { 1L, "S_FALSE ((HRESULT)" }
} };

struct ProfilePrefix {
    const char* name;
    const char* prefix;
};

constexpr auto PROFILE_PREFIX_TABLE_LEN = 12;

const std::array <ProfilePrefix, PROFILE_PREFIX_TABLE_LEN>g_profilePrefixTable = { {
  { "ps_2_0", "g_ps20"},
  { "ps_2_a", "g_ps21"},
  { "ps_2_b", "g_ps21"},
  { "ps_2_sw", "g_ps2ff"},
  { "ps_3_0", "g_ps30"},
  { "ps_3_sw", "g_ps3ff"},

  { "vs_1_1", "g_vs11"},
  { "vs_2_0", "g_vs20"},
  { "vs_2_a", "g_vs21"},
  { "vs_2_sw", "g_vs2ff"},
  { "vs_3_0", "g_vs30"},
  { "vs_3_sw", "g_vs3ff"},
} };

char* concat(const char* s1, const char* s2);
#ifdef _WIN32
wchar_t* concat(const wchar_t* s1, const wchar_t* s2);
#endif
char* GetFileName(_In_ char* path, _Out_ int* IsSpecialFolder);
#ifdef _WIN32
wchar_t* GetFileName(_In_ wchar_t* path, _Out_ int* IsSpecialFolder);
void FixupFileName(_Inout_ wchar_t* FileName);
bool parseOpt(_In_ const wchar_t* option, 
    _In_ int argc, 
    _In_ wchar_t* argv[1], 
    _Inout_ int* index, 
    _Inout_opt_ wchar_t** argumentOption);
#else  /* _WIN32 */
char* LoadSource(_In_ const char* filename, _Out_ size_t* len);
bool parseOpt(_In_ const char* option, 
    _In_ int argc, 
    _In_ char* argv[1], 
    _Inout_	int* index, 
    _Inout_opt_ char** argumentOption);
#endif /* _WIN32 */
void print_copyright();
[[noreturn]] void print_errno(errno_t _errno);
[[noreturn]] void print_errno(void);
[[noreturn]] void print_help_screen();
[[noreturn]] void print_hresult_error(const HRESULT hr);
void print_unsupported_arg_help();
[[noreturn]] void print_usage_missing(const char* arg);
[[noreturn]] void print_usage_toomany();
[[noreturn]] void print_version();
[[noreturn]] void print_windows_error();
int readall(_In_ FILE* in, 
    _Out_writes_bytes_(*sizeptr) char** dataptr, 
    _Out_opt_ size_t* sizeptr);
char* setupVariableName(_In_ const char* model,
    const _In_ char* entryPoint);
#ifdef _WIN32
wchar_t* utf8_decode(const char* str, size_t nbytes);
char* utf8_encode(const wchar_t* wstr, size_t nchars);
char* utf8_encode(const wchar_t* wstr);
#endif
void WriteByteArrayConst(_In_ FILE* f, 
    _In_reads_bytes_(len) const unsigned char* outString, 
    _In_ const size_t len, 
    _In_z_ const char* variableName, 
    _In_ const int outputHex);

#endif  /*EFXC2UTILS_H_INCLUDED*/