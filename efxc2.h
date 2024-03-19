//--------------------------------------------------------------------------------------
// File: efxc2.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#pragma once
#ifdef __MINGW32__
#define __USE_MINGW_ANSI_STDIO 1
#endif

#ifdef  _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4005)
#endif /* _MSC_VER */
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifdef  _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */

#include "config.h"
#include <d3dcompiler.h>
#include <d3dcommon.h>
#ifdef _WIN32
#include <direct.h>
#endif /* _WIN32 */
#include <stdio.h>
#include <string>
#include <cassert>
#include <memory>
#include <array>
#include <wchar.h>
#include <inttypes.h>
#include <io.h>
#include <stdio.h>
#include <sal.h>

/*don't convert this to constexpr because it cause the 
compiler to complain with the printf statement.*/
#define DLL_NAME  L"d3dcompiler_47.dll"

#ifndef D3D_COMPILE_STANDARD_FILE_INCLUDE
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)
#endif

// D3DCOMPILE flags1 parameters:
#ifndef D3DCOMPILE_DEBUG
#define D3DCOMPILE_DEBUG                             (1 << 0)
#endif
#ifndef D3DCOMPILE_SKIP_VALIDATION
#define D3DCOMPILE_SKIP_VALIDATION                   (1 << 1)
#endif
#ifndef D3DCOMPILE_SKIP_OPTIMIZATION
#define D3DCOMPILE_SKIP_OPTIMIZATION                 (1 << 2)
#endif
#ifndef D3DCOMPILE_PACK_MATRIX_ROW_MAJOR
#define D3DCOMPILE_PACK_MATRIX_ROW_MAJOR             (1 << 3)
#endif
#ifndef D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR
#define D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR          (1 << 4)
#endif
#ifndef D3DCOMPILE_PARTIAL_PRECISION
#define D3DCOMPILE_PARTIAL_PRECISION                 (1 << 5)
#endif
#ifndef D3DCOMPILE_PARTIAL_PRECISION
#define D3DCOMPILE_PARTIAL_PRECISION                 (1 << 5)
#endif
#ifndef D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT
#define D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT          (1 << 6)
#endif
#ifndef D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT
#define D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT          (1 << 7)
#endif
#ifndef D3DCOMPILE_NO_PRESHADER
#define D3DCOMPILE_NO_PRESHADER                      (1 << 8)
#endif
#ifndef D3DCOMPILE_AVOID_FLOW_CONTROL
#define D3DCOMPILE_AVOID_FLOW_CONTROL                (1 << 9)
#endif
#ifndef D3DCOMPILE_WARNINGS_ARE_ERRORS
#define D3DCOMPILE_WARNINGS_ARE_ERRORS               (1 << 18)
#endif
#ifndef D3DCOMPILE_ENABLE_STRICTNESS
#define D3DCOMPILE_ENABLE_STRICTNESS                  (1 << 11)
#endif
#ifndef D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY
#define D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY     (1 << 12)
#endif
#ifndef D3DCOMPILE_IEEE_STRICTNESS
#define D3DCOMPILE_IEEE_STRICTNESS                    (1 << 13)
#endif
#ifndef D3DCOMPILE_OPTIMIZATION_LEVEL0
#define D3DCOMPILE_OPTIMIZATION_LEVEL0                (1 << 14)
#endif
#ifndef D3DCOMPILE_OPTIMIZATION_LEVEL1
#define D3DCOMPILE_OPTIMIZATION_LEVEL1                0
#endif
#ifndef D3DCOMPILE_OPTIMIZATION_LEVEL2
#define D3DCOMPILE_OPTIMIZATION_LEVEL2                ((1 << 14) | (1 << 15))
#endif
#ifndef D3DCOMPILE_OPTIMIZATION_LEVEL3
#define D3DCOMPILE_OPTIMIZATION_LEVEL3                (1 << 15)
#endif
#ifndef D3DCOMPILE_RESERVED16
#define D3DCOMPILE_RESERVED16                         (1 << 16)
#endif
#ifndef D3DCOMPILE_RESERVED17
#define D3DCOMPILE_RESERVED17                         (1 << 17)
#endif
#ifndef D3DCOMPILE_WARNINGS_ARE_ERRORS
#define D3DCOMPILE_WARNINGS_ARE_ERRORS                (1 << 18)
#endif
#ifndef D3DCOMPILE_RESOURCES_MAY_ALIAS
#define D3DCOMPILE_RESOURCES_MAY_ALIAS                (1 << 19)
#endif
#ifndef D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES
#define D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES (1 << 20)
#endif
#ifndef D3DCOMPILE_ALL_RESOURCES_BOUND
#define D3DCOMPILE_ALL_RESOURCES_BOUND                (1 << 21)
#endif
#ifndef D3DCOMPILE_DEBUG_NAME_FOR_SOURCE
#define D3DCOMPILE_DEBUG_NAME_FOR_SOURCE              (1 << 22)
#endif
#ifndef D3DCOMPILE_DEBUG_NAME_FOR_BINARY
#define D3DCOMPILE_DEBUG_NAME_FOR_BINARY              (1 << 23)
#endif

/* flags2 parameters */
#ifndef D3DCOMPILE_EFFECT_CHILD_EFFECT
#define D3DCOMPILE_EFFECT_CHILD_EFFECT                (1 << 0)
#endif
#ifndef D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS
#define D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS              (1 << 1)
#endif
#ifndef D3DCOMPILE_FLAGS2_FORCE_ROOT_SIGNATURE_LATEST
#define D3DCOMPILE_FLAGS2_FORCE_ROOT_SIGNATURE_LATEST 0
#endif
#ifndef D3DCOMPILE_FLAGS2_FORCE_ROOT_SIGNATURE_1_0
#define D3DCOMPILE_FLAGS2_FORCE_ROOT_SIGNATURE_1_0   (1 << 4)
#endif
#ifndef D3DCOMPILE_FLAGS2_FORCE_ROOT_SIGNATURE_1_1
#define D3DCOMPILE_FLAGS2_FORCE_ROOT_SIGNATURE_1_1   (1 << 5)
#endif

/*secondary data options*/
#ifndef D3DCOMPILE_SECDATA_MERGE_UAV_SLOTS
#define D3DCOMPILE_SECDATA_MERGE_UAV_SLOTS           0x00000001
#endif
#ifndef D3DCOMPILE_SECDATA_PRESERVE_TEMPLATE_SLOTS
#define D3DCOMPILE_SECDATA_PRESERVE_TEMPLATE_SLOTS   0x00000002
#endif
#ifndef D3DCOMPILE_SECDATA_REQUIRE_TEMPLATE_MATCH
#define D3DCOMPILE_SECDATA_REQUIRE_TEMPLATE_MATCH    0x00000004
#endif

/*D3DDisassembley flags*/
#ifndef D3D_DISASM_ENABLE_COLOR_CODE
#define D3D_DISASM_ENABLE_COLOR_CODE            0x00000001
#endif
#ifndef D3D_DISASM_ENABLE_DEFAULT_VALUE_PRINTS
#define D3D_DISASM_ENABLE_DEFAULT_VALUE_PRINTS  0x00000002
#endif
#ifndef D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING
#define D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING 0x00000004
#endif
#ifndef D3D_DISASM_ENABLE_INSTRUCTION_CYCLE
#define D3D_DISASM_ENABLE_INSTRUCTION_CYCLE     0x00000008
#endif
#ifndef D3D_DISASM_DISABLE_DEBUG_INFO
#define D3D_DISASM_DISABLE_DEBUG_INFO           0x00000010
#endif
#ifndef D3D_DISASM_ENABLE_INSTRUCTION_OFFSET
#define D3D_DISASM_ENABLE_INSTRUCTION_OFFSET    0x00000020
#endif
#ifndef D3D_DISASM_INSTRUCTION_ONLY
#define D3D_DISASM_INSTRUCTION_ONLY             0x00000040
#endif
#ifndef D3D_DISASM_PRINT_HEX_LITERALS
#define D3D_DISASM_PRINT_HEX_LITERALS           0x00000080
#endif

typedef HRESULT(__stdcall* pD3DCompile2g)(
    _In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
    _In_ SIZE_T SrcDataSize,
    _In_opt_ LPCSTR pSourceName,
    _In_reads_opt_(_Inexpressible_(pDefines->Name != NULL)) CONST D3D_SHADER_MACRO* pDefines,
    _In_opt_ ID3DInclude* pInclude,
    _In_ LPCSTR pEntrypoint,
    _In_ LPCSTR pTarget,
    _In_ UINT Flags1,
    _In_ UINT Flags2,
    _In_ UINT SecondaryDataFlags,
    _In_reads_bytes_opt_(SecondaryDataSize) LPCVOID pSecondaryData,
    _In_ SIZE_T SecondaryDataSize,
    _Out_ ID3DBlob** ppCode,
    _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorMsgs);

typedef HRESULT(__stdcall* pD3DStripShaderg) (
    _In_reads_bytes_(BytecodeLength) LPCVOID pShaderBytecode,
    _In_ SIZE_T BytecodeLength,
    _In_ UINT uStripFlags,
    _Out_ ID3DBlob** ppStrippedBlob);

typedef HRESULT(__stdcall* pD3DGetBlobPartg) (
    _In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
    _In_ SIZE_T SrcDataSize,
    _In_ D3D_BLOB_PART Part,
    _In_ UINT Flags,
    _Out_ ID3DBlob** ppPart);

typedef HRESULT(__stdcall* pD3DSetBlobPartg) (
    _In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
    _In_ SIZE_T SrcDataSize,
    _In_ D3D_BLOB_PART Part,
    _In_ UINT Flags,
    _In_reads_bytes_(PartSize) LPCVOID pPart,
    _In_ SIZE_T PartSize,
    _Out_ ID3DBlob** ppNewShader);

typedef HRESULT(__stdcall* pD3DDisassembleg) (
    _In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
    _In_ SIZE_T SrcDataSize,
    _In_ UINT Flags,
    _In_opt_ LPCSTR szComments,
    _Out_ ID3DBlob** ppDisassembly);

// This struct represents the first four bytes of the name blob:
struct ShaderDebugName
{
    uint16_t Flags;       // Reserved, must be set to zero.
    uint16_t NameLength;  // Length of the debug name, without null terminator.
    // Followed by NameLength bytes of the UTF-8-encoded name.
    // Followed by a null terminator.
    // Followed by [0-3] zero bytes to align to a 4-byte boundary.
};

/* helper macros for functions */
#ifdef _WIN32
#define M_WCSDUP _wcsdup
#else  /* _WIN32 */
#define M_WCSDUP wcsdup
#endif /* _WIN32 */

#ifndef _countof
#define _countof(a) (sizeof(a)/sizeof(*(a)))
#endif

/* command codes for internal use */
constexpr auto CMD_WRITE_HEADER = 1;
constexpr auto CMD_WRITE_OBJECT = 2;
constexpr auto CMD_WRITE_ASSEMBLY_CODE = 4;

/* a good size for an error message */
constexpr auto ERR_SIZE = 128;

/* Parameter constants since we need both
   Unicode and ASCII versions depending
   upon platform.*/

#ifdef _WIN32
constexpr const wchar_t* M_AT_SYMBOL = L"@";
constexpr const wchar_t* M_QUESTION_MARK = L"?";
constexpr const wchar_t* M_ALL_RESOURCES_BOUND = L"all_resources_bound";
constexpr const wchar_t* M_CC = L"Cc";
constexpr const wchar_t* M_COMPRESS = L"compress";
constexpr const wchar_t* M_D = L"D";
constexpr const wchar_t* M_DECOMPRESS = L"decompress";
constexpr const wchar_t* M_DUMPBIN = L"dumpbin";
constexpr const wchar_t* M_E_ =  L"E";
constexpr const wchar_t* M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES = L"enable_unbounded_descriptor_tables";
constexpr const wchar_t* M_FC = L"Fc";
constexpr const wchar_t* M_FD =  L"Fd";
constexpr const wchar_t* M_FE =  L"Fe";
constexpr const wchar_t* M_FH = L"Fh";
constexpr const wchar_t* M_FL = L"Fl";
constexpr const wchar_t* M_FO = L"Fo";
constexpr const wchar_t* M_FORCE_ROOTSIG_VER = L"force_rootsig_ver";
constexpr const wchar_t* M_FX = L"Fx";
constexpr const wchar_t* M_GCH = L"Gch";
constexpr const wchar_t* M_GDP = L"Gdp";
constexpr const wchar_t* M_GEC = L"Gec";
constexpr const wchar_t* M_GES = L"Ges";
constexpr const wchar_t* M_GETPRIVATE = L"getprivate";
constexpr const wchar_t* M_GFA = L"Gfa";
constexpr const wchar_t* M_GIS = L"Gis";
constexpr const wchar_t* M_GPP = L"Gpp";
constexpr const wchar_t* M_HELP = L"help";
constexpr const wchar_t* M_I = L"I";
constexpr const wchar_t* M_LX = L"Lx";
constexpr const wchar_t* M_MATCHUAVS = L"matchUAVs";
constexpr const wchar_t* M_MERGEUAVS = L"mergeUAVs";
constexpr const wchar_t* M_NI = L"Ni";
constexpr const wchar_t* M_NO = L"No";
constexpr const wchar_t* M_NOLOGO = L"nologo";
constexpr const wchar_t* M_O0 = L"O0";
constexpr const wchar_t* M_O1 = L"O1";
constexpr const wchar_t* M_O2 = L"O2";
constexpr const wchar_t* M_O3 = L"O3";
constexpr const wchar_t* M_OD = L"Od";
constexpr const wchar_t* M_OP = L"Op";
constexpr const wchar_t* M_P = L"P";
constexpr const wchar_t* M_QSTRIP_DEBUG = L"Qstrip_debug";
constexpr const wchar_t* M_QSTRIP_PRIV = L"Qstrip_priv";
constexpr const wchar_t* M_QSTRIP_REFLECT = L"Qstrip_reflect";
constexpr const wchar_t* M_QSTRIP_ROOTSIGNATURE = L"Qstrip_rootsignature";
constexpr const wchar_t* M_RES_MAY_ALIAS = L"res_may_alias";
constexpr const wchar_t* M_SETPRIVATE = L"setprivate";
constexpr const wchar_t* M_SHTEMPLATE = L"shtemplate";
constexpr const wchar_t* M_T = L"T";
constexpr const wchar_t* M_VD = L"Vd";
constexpr const wchar_t* M_VERSION = L"version";
constexpr const wchar_t* M_VI = L"Vi";
constexpr const wchar_t* M_VN = L"Vn";
constexpr const wchar_t* M_WX = L"WX";
constexpr const wchar_t* M_ZI = L"Zi";
constexpr const wchar_t* M_ZPC = L"Zpc";
constexpr const wchar_t* M_ZPR = L"Zpr";
constexpr const wchar_t* M_ZSB = L"Zsb";
constexpr const wchar_t* M_ZSS = L"Zss";
#else  /* _WIN32 */
constexpr const char* M_AT_SYMBOL = "@";
constexpr const char* M_QUESTION_MARK = "?";
constexpr const char* M_ALL_RESOURCES_BOUND = "all_resources_bound";
constexpr const char* M_CC = "Cc";
constexpr const char* M_COMPRESS = "compress";
constexpr const char* M_D = "D";
constexpr const char* M_DECOMPRESS = "decompress";
constexpr const char* M_DUMPBIN = "dumpbin";
constexpr const char* M_E_ = "E";
constexpr const char* M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES = "enable_unbounded_descriptor_tables";
constexpr const char* M_FC = "Fc";
constexpr const char* M_FD = "Fd";
constexpr const char* M_FE = "Fe";
constexpr const char* M_FH = "Fh";
constexpr const char* M_FL = "Fl";
constexpr const char* M_FO = "Fo";
constexpr const char* M_FORCE_ROOTSIG_VER = "force_rootsig_ver";
constexpr const char* M_FX = "Fx";
constexpr const char* M_GCH = "Gch";
constexpr const char* M_GDP = "Gdp";
constexpr const char* M_GEC = "Gec";
constexpr const char* M_GES = "Ges";
constexpr const char* M_GETPRIVATE = "getprivate";
constexpr const char* M_GFA = "Gfa";
constexpr const char* M_GIS = "Gis";
constexpr const char* M_GPP = "Gpp";
constexpr const char* M_HELP = "help";
constexpr const char* M_I = "I";
constexpr const char* M_LX = "Lx";
constexpr const char* M_MATCHUAVS = "matchUAVs";
constexpr const char* M_MERGEUAVS = "mergeUAVs";
constexpr const char* M_NI = "Ni";
constexpr const char* M_NO = "No";
constexpr const char* M_NOLOGO = "nologo";
constexpr const char* M_O0 = "O0";
constexpr const char* M_O1 = "O1";
constexpr const char* M_O2 = "O2";
constexpr const char* M_O3 = "O3";
constexpr const char* M_OD = "Od";
constexpr const char* M_OP = "Op";
constexpr const char* M_P = "P";
constexpr const char* M_QSTRIP_DEBUG = "Qstrip_debug";
constexpr const char* M_QSTRIP_PRIV = "Qstrip_priv";
constexpr const char* M_QSTRIP_REFLECT = "Qstrip_reflect";
constexpr const char* M_QSTRIP_ROOTSIGNATURE = "Qstrip_rootsignature";
constexpr const char* M_RES_MAY_ALIAS = "res_may_alias";
constexpr const char* M_SETPRIVATE = "setprivate";
constexpr const char* M_SHTEMPLATE = "shtemplate";
constexpr const char* M_T = "T";
constexpr const char* M_VD = "Vd";
constexpr const char* M_VERSION = "version";
constexpr const char* M_VI = "Vi";
constexpr const char* M_VN = "Vn";
constexpr const char* M_WX = "WX";
constexpr const char* M_ZI = "Zi";
constexpr const char* M_ZPC = "Zpc";
constexpr const char* M_ZPR = "Zpr";
constexpr const char* M_ZSB = "Zsb";
constexpr const char* M_ZSS = "Zss";
#endif /* _WIN32 */