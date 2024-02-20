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
#include <wchar.h>
#include <inttypes.h>
#include <io.h>
#include <stdio.h>

#define DLL_NAME L"d3dcompiler_47.dll"

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

typedef HRESULT(__stdcall *pD3DCompile2g)(
                          const void* data,
                          SIZE_T data_size,
                          const char* filename,
                          const D3D_SHADER_MACRO* defines,
                          ID3DInclude* include, 
                          const char* entrypoint,
                          const char* target, 
                          UINT sflags, 
                          UINT eflags, 
                          UINT secondary_flags,
                          const void* secondary_data, 
                          SIZE_T secondary_data_size, 
                          ID3DBlob** shader,
                          ID3DBlob** error_messages);

struct ProfilePrefix {
  const char* name;
  const char* prefix;
};

static const ProfilePrefix g_profilePrefixTable[] = {
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

  { NULL, NULL}
};

/* helper macros for functions */
#ifdef _WIN32
#define M_WCSDUP _wcsdup
#else  /* _WIN32 */
#define M_WCSDUP wcsdup
#endif /* _WIN32 */

/* command codes for internal use */
#define CMD_WRITE_HEADER 1
#define CMD_WRITE_OBJECT 2

/* a good size for an error message */
#define ERR_SIZE 128

/* Parameter constants since we need both
   Unicode and ASCII versions depending
   upon platform.*/

#ifdef _WIN32
#define M_AT_SYMBOL                          L"@"
#define M_QUESTION_MARK                      L"?"
#define M_ALL_RESOURCES_BOUND                L"all_resources_bound"
#define M_CC                                 L"Cc"
#define M_COMPRESS                           L"compress"
#define M_D                                  L"D"
#define M_DECOMPRESS                         L"decompress"
#define M_DUMPBIN                            L"dumpbin"
#define M_E_                                 L"E"
#define M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES L"enable_unbounded_descriptor_tables"
#define M_FD                                 L"Fd"
#define M_FE                                 L"Fe"
#define M_FH                                 L"Fh"
#define M_FL                                 L"Fl"
#define M_FO                                 L"Fo"
#define M_FORCE_ROOTSIG_VER                  L"force_rootsig_ver"
#define M_FX                                 L"Fx"
#define M_GCH                                L"Gch"
#define M_GDP                                L"Gdp"
#define M_GEC                                L"Gec"
#define M_GES                                L"Ges"
#define M_GETPRIVATE                         L"getprivate"
#define M_GFA                                L"Gfa"
#define M_GIS                                L"Gis"
#define M_GPP                                L"Gpp"
#define M_HELP                               L"help"
#define M_I                                  L"I"
#define M_LX                                 L"Lx"
#define M_MATCHUAVS                          L"matchUAVs"
#define M_MERGEUAVS                          L"mergeUAVs"
#define M_NI                                 L"Ni"
#define M_NO                                 L"No"
#define M_NOLOGO                             L"nologo"
#define M_O0                                 L"O0"
#define M_O1                                 L"O1"
#define M_O2                                 L"O2"
#define M_O3                                 L"O3"
#define M_OP                                 L"Op"
#define M_P                                  L"P"
#define M_QSTRIP_DEBUG                       L"Qstrip_debug"
#define M_QSTRIP_PRIV                        L"Qstrip_priv"
#define M_QSTRIP_REFLECT                     L"Qstrip_reflect"
#define M_QSTRIP_ROOTSIGNATURE               L"Qstrip_rootsignature"
#define M_RES_MAY_ALIAS                      L"res_may_alias"
#define M_SETPRIVATE                         L"setprivate"
#define M_SHTEMPLATE                         L"shtemplate"
#define M_T                                  L"T"
#define M_VD                                 L"Vd"
#define M_VERSION                            L"version"
#define M_VI                                 L"Vi"
#define M_VN                                 L"Vn"
#define M_WX                                 L"WX"
#define M_ZD                                 L"Vd"
#define M_ZI                                 L"Zi"
#define M_ZPC                                L"Zpc"
#define M_ZPR                                L"Zpr"
#define M_ZSB                                L"Zsb"
#define M_ZSS                                L"Zss"
#else  /* _WIN32 */
#define M_AT_SYMBOL                          "@"
#define M_QUESTION_MARK                      "?"
#define M_ALL_RESOURCES_BOUND                "all_resources_bound"
#define M_CC                                 "Cc"
#define M_COMPRESS                           "compress"
#define M_D                                  "D"
#define M_DECOMPRESS                         "decompress"
#define M_DUMPBIN                            "dumpbin"
#define M_E_                                 "E"
#define M_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES "enable_unbounded_descriptor_tables"
#define M_FD                                 "Fd"
#define M_FE                                 "Fe"
#define M_FH                                 "Fh"
#define M_FL                                 "Fl"
#define M_FO                                 "Fo"
#define M_FORCE_ROOTSIG_VER                  "force_rootsig_ver"
#define M_FX                                 "Fx"
#define M_GCH                                "Gch"
#define M_GDP                                "Gdp"
#define M_GEC                                "Gec"
#define M_GES                                "Ges"
#define M_GETPRIVATE                         "getprivate"
#define M_GFA                                "Gfa"
#define M_GIS                                "Gis"
#define M_GPP                                "Gpp"
#define M_HELP                               "help"
#define M_I                                  "I"
#define M_LX                                 "Lx"
#define M_MATCHUAVS                          "matchUAVs"
#define M_MERGEUAVS                          "mergeUAVs"
#define M_NI                                 "Ni"
#define M_NO                                 "No"
#define M_NOLOGO                             "nologo"
#define M_O0                                 "O0"
#define M_O1                                 "O1"
#define M_O2                                 "O2"
#define M_O3                                 "O3"
#define M_OP                                 "Op"
#define M_P                                  "P"
#define M_QSTRIP_DEBUG                       "Qstrip_debug"
#define M_QSTRIP_PRIV                        "Qstrip_priv"
#define M_QSTRIP_REFLECT                     "Qstrip_reflect"
#define M_QSTRIP_ROOTSIGNATURE               "Qstrip_rootsignature"
#define M_RES_MAY_ALIAS                      "res_may_alias"
#define M_SETPRIVATE                         "setprivate"
#define M_SHTEMPLATE                         "shtemplate"
#define M_T                                  "T"
#define M_VD                                 "Vd"
#define M_VERSION                            "version"
#define M_VI                                 "Vi"
#define M_VN                                 "Vn"
#define M_WX                                 "WX"
#define M_ZD                                 "Vd"
#define M_ZI                                 "Zi"
#define M_ZPC                                "Zpc"
#define M_ZPR                                "Zpr"
#define M_ZSB                                "Zsb"
#define M_ZSS                                "Zss"
#endif /* _WIN32 */