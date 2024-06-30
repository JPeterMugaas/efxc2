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
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <errno.h>
#include <exception>
#ifdef USE_FMT_LIBRARY
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/xchar.h>
#else
#include <format>
#endif
#include <fstream>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <string_view>
#include <vector>
#include <sal.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#ifdef _WIN32
#include <wchar.h>
#endif

#ifdef USE_FMT_LIBRARY
#if ( _MSC_VER )
#pragma comment(lib, "fmt")
#endif
#endif

extern "C" {
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

/* Both MSYS2 and WIN32 define this structure. */
#if defined(__CYGWIN) && !defined(__MSYS__)
/* for some reason, this is not declared in MSYS's Win32 headers*/

// {59A6CD0E-E10D-4C1F-88C0-63ABA1DAF30E}
	interface DECLSPEC_UUID("59A6CD0E-E10D-4C1F-88C0-63ABA1DAF30E") ID3D11Linker;
	DEFINE_GUID(IID_ID3D11Linker,
		0x59a6cd0e, 0xe10d, 0x4c1f, 0x88, 0xc0, 0x63, 0xab, 0xa1, 0xda, 0xf3, 0xe);

#define INTERFACE ID3D11Linker

	DECLARE_INTERFACE_(ID3D11Linker, IUnknown)
	{
		STDMETHOD(QueryInterface)(THIS_ _In_ REFIID iid, _Out_ LPVOID * ppv) PURE;
		STDMETHOD_(ULONG, AddRef)(THIS) PURE;
		STDMETHOD_(ULONG, Release)(THIS) PURE;

		// Link the shader and produce a shader blob suitable to D3D runtime.
		STDMETHOD(Link)(THIS_ _In_ interface ID3D11ModuleInstance* pEntry,
			_In_ LPCSTR pEntryName,
			_In_ LPCSTR pTargetName,
			_In_ UINT uFlags,
			_COM_Outptr_ ID3DBlob * *ppShaderBlob,
			_Always_(_Outptr_opt_result_maybenull_) ID3DBlob * *ppErrorBuffer) PURE;

		// Add an instance of a library module to be used for linking.
		STDMETHOD(UseLibrary)(THIS_ _In_ interface ID3D11ModuleInstance* pLibraryMI) PURE;

		// Add a clip plane with the plane coefficients taken from a cbuffer entry for 10L9 shaders.
		STDMETHOD(AddClipPlaneFromCBuffer)(THIS_ _In_ UINT uCBufferSlot, _In_ UINT uCBufferEntry) PURE;
	};

#endif
	typedef HRESULT (__stdcall* pD3DCompile2g) (
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

	typedef HRESULT (__stdcall* pD3DStripShaderg) (
		_In_reads_bytes_(BytecodeLength) LPCVOID pShaderBytecode,
		_In_ SIZE_T BytecodeLength,
		_In_ UINT uStripFlags,
		_Out_ ID3DBlob** ppStrippedBlob);

	typedef HRESULT (__stdcall* pD3DGetBlobPartg) (
		_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
		_In_ SIZE_T SrcDataSize,
		_In_ D3D_BLOB_PART Part,
		_In_ UINT Flags,
		_Out_ ID3DBlob** ppPart);

	typedef HRESULT (__stdcall* pD3DSetBlobPartg) (
		_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
		_In_ SIZE_T SrcDataSize,
		_In_ D3D_BLOB_PART Part,
		_In_ UINT Flags,
		_In_reads_bytes_(PartSize) LPCVOID pPart,
		_In_ SIZE_T PartSize,
		_Out_ ID3DBlob** ppNewShader);

	typedef HRESULT (__stdcall* pD3DDisassembleg) (
		_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
		_In_ SIZE_T SrcDataSize,
		_In_ UINT Flags,
		_In_opt_ LPCSTR szComments,
		_Out_ ID3DBlob** ppDisassembly);

	typedef HRESULT (__stdcall* gD3DLoadModulep) (
		_In_ LPCVOID pSrcData,
		_In_ SIZE_T cbSrcDataSize,
		_Out_ interface ID3D11Module** ppModule
		);

#ifdef _MSC_VER
	typedef HRESULT(__stdcall* gD3DCreateLinkerp) (
		__out interface ID3D11Linker** ppLinker
		);
	typedef HRESULT (__stdcall* gD3DCreateFunctionLinkingGraphp) (
		_In_ UINT uFlags,
		_Out_ interface ID3D11FunctionLinkingGraph** ppFunctionLinkingGraph
		);
#else
	typedef HRESULT (__stdcall* gD3DCreateLinkerp) (
		_Out_ interface ID3D11Linker** ppLinker
		);
	typedef HRESULT (__stdcall* gD3DCreateFunctionLinkingGraphp) (
		_In_ UINT uFlags,
		_Out_ interface ID3D11FunctionLinkingGraph** ppFunctionLinkingGraph
		);
#endif
	typedef HRESULT (__stdcall* gD3DPreprocessp) (
		_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
		_In_ SIZE_T SrcDataSize,
		_In_opt_ LPCSTR pSourceName,
		_In_opt_ CONST D3D_SHADER_MACRO* pDefines,
		_In_opt_ ID3DInclude* pInclude,
		_Out_ ID3DBlob** ppCodeText,
		_Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorMsgs);
}
