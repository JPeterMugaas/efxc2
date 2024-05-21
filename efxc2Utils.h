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
#include "efxc2Exception.h"
#include "efxc2Console.h"

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

	/* We have to disable warning V3515 and V2523 about constants having a suffix such as U or L because the Win32 API
	  header did not define those constants with these suffixes. */
	const std::array <ErrorTableEntry, ERROR_TABLE_LENGTH >g_ErrorTable = { {
		{ D3D11_ERROR_FILE_NOT_FOUND, "D3D11_ERROR_FILE_NOT_FOUND" },  //-V3515  //-V2523
		{ D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS, "D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS" },  //-V3515 //-V2523
		{ D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS, "D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS" },  //-V3515 //-V2523
		{ D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD, "D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD" }, //-V3515 //-V2523
		{ DXGI_ERROR_INVALID_CALL, "D3DERR_INVALIDCALL (replaced with DXGI_ERROR_INVALID_CALL)" },  //-V3515 //-V2523
		{ DXGI_ERROR_WAS_STILL_DRAWING, "D3DERR_WASSTILLDRAWING (replaced with DXGI_ERROR_WAS_STILL_DRAWING)" },  //-V3515 //-V2523
		{ E_FAIL, "E_FAIL" },  //-V3515 //-V2523
		{ E_INVALIDARG, "E_INVALIDARG" }, //-V3515  //-V2523
		{ E_OUTOFMEMORY, "E_OUTOFMEMORY" },  //-V3515 //-V2523
		{ E_NOTIMPL, "E_NOTIMPL" },  //-V3515 //-V2523
		{ S_FALSE, "S_FALSE ((HRESULT)" }  //-V3515 //-V2523
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
	const std::string utf8_locale = "en_US.UTF-8";
#endif

	constexpr size_t ERRNO_BUFF_SIZE = 256;

	void  checkForMissingArg(M_STRING_VIEW option, _In_ size_t index, const M_CMD_PARAMS& args);
#ifdef _WIN32
	void FixupFileName(_Inout_ std::string& FileName);
	void FixupFileName(_Inout_ std::wstring& FileName);
#endif /* _WIN32 */
	std::string HResultName(_In_ const HRESULT hr);
	bool parseOpt(_In_ const M_STRING_VIEW option, _In_ const M_CMD_PARAMS& args, _Inout_ size_t* index, _Inout_opt_ M_STRING* argumentOption);
	void print_copyright();
	void print_errno_value(errno_t err);
	void print_help_screen();
	void print_hresult_error(const HRESULT hr);
	void print_no_input_file();
	std::string print_unsupported_arg_help();
	std::string print_usage_arg();
	void print_usage_toomany();
	void print_version();
	void print_windows_error();
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