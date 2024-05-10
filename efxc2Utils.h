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

namespace efxc2Utils {
    using M_BUFFER = std::shared_ptr<std::vector<char>>;

#ifdef _WIN32
    using M_CMD_PARAMS = std::vector<std::wstring>;
    using M_STRING = std::wstring;
    using M_STRING_VIEW = std::wstring_view;
    constexpr auto M_STRING_INIT = L"";
#else
    using M_CMD_PARAMS = std::vector<std::string>;
    using M_STRING = std::string;
    using M_STRING_VIEW = std::string_view;
    constexpr auto M_STRING_INIT = "";
#endif

    struct CompilerDefine {
        std::string Name;
        std::string Definition;
    };
    using M_COMPILER_DEFINES = std::shared_ptr<std::vector<CompilerDefine>>;

    /*from: https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer */
    /* Size of each input chunk to be
       read and allocate for. */
    const size_t  READALL_CHUNK = 262144;

    /* Helper D3D error lookup table*/
    struct ErrorTableEntry {
        HRESULT ErrorCode;
        const std::string ErrorName;
    };

    constexpr auto ERROR_TABLE_LENGTH = 11;

    const std::array <ErrorTableEntry, ERROR_TABLE_LENGTH >g_ErrorTable = { {
        { std::bit_cast<HRESULT>(0x887C0002), "D3D11_ERROR_FILE_NOT_FOUND" },
        { std::bit_cast<HRESULT>(0x887C0001), "D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS" },
        { std::bit_cast<HRESULT>(0x887C0003), "D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS" },
        { std::bit_cast<HRESULT>(0x887C0004), "D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD" },
        { std::bit_cast<HRESULT>(0x887A0001), "D3DERR_INVALIDCALL (replaced with DXGI_ERROR_INVALID_CALL)" },
        { std::bit_cast<HRESULT>(0x887A000A), "D3DERR_WASSTILLDRAWING (replaced with DXGI_ERROR_WAS_STILL_DRAWING)" },
        { std::bit_cast<HRESULT>(0x80004005), "E_FAIL" },
        { std::bit_cast<HRESULT>(0x80070057), "E_INVALIDARG" },
        { std::bit_cast<HRESULT>(0x8007000E), "E_OUTOFMEMORY" },
        { std::bit_cast<HRESULT>(0x80004001), "E_NOTIMPL" },
        { 1L, "S_FALSE ((HRESULT)" }
    } };

    struct ProfilePrefix {
        const std::string name;
        const std::string prefix;
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

    /* command codes for internal use */
    constexpr auto CMD_WRITE_HEADER = 1;
    constexpr auto CMD_WRITE_OBJECT = 2;
    constexpr auto CMD_WRITE_PDB_FILE = 4;
    constexpr auto CMD_WRITE_ASSEMBLY_CODE = 8;
    constexpr auto CMD_PREPROCESS_FILE = 16;
    constexpr auto CMD_WRITE_LIBRARY_FILE = 32;

    /* a good size for an error message */
    constexpr auto ERR_SIZE = 128;

#ifdef _WIN32
    void FixupFileName(_Inout_ std::string& FileName);
    void FixupFileName(_Inout_ std::wstring& FileName);
#endif /* _WIN32 */
    std::string HResultName(_In_ const HRESULT hr);
    bool parseOpt(_In_ const M_STRING_VIEW option, _In_ const M_CMD_PARAMS& args, _Inout_ size_t* index, _Inout_opt_ M_STRING* argumentOption);
    void print_copyright();
    [[noreturn]] void print_help_screen();
    [[noreturn]] void print_hresult_error(const HRESULT hr);
    [[noreturn]] void print_no_input_file();
    void print_unsupported_arg_help();
    void print_usage_arg();
    [[noreturn]] void print_usage_missing(const char* arg);
    [[noreturn]] void print_usage_toomany();
    [[noreturn]] void print_version();
    [[noreturn]] void print_windows_error();
    bool readAll(_In_ std::ifstream& in,
        _Out_ M_BUFFER& dataptr);
    std::string setupVariableName(_In_ const std::string_view model,
        _In_ const std::string& entryPoint);
#ifdef _WIN32
    std::wstring utf8_to_wstring(std::string const& str);
#endif
    void WriteByteArrayConst(_In_ std::ofstream& f, ID3DBlob* data,
        _In_ const std::string_view variableName,
        _In_ const int outputHex);
#ifdef _WIN32
    std::string wstring_to_utf8(std::wstring const& wstr);
#endif
}
#endif  /*EFXC2UTILS_H_INCLUDED*/