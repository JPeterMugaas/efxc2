//--------------------------------------------------------------------------------------
// File: efxc2CompilerAPIContainer.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2CompilerAPIContainer.h"
#include "efxc2Utils.h"
#include "efxc2Console.h"

efxc2CompilerAPIContainer::CompilerAPIContainer::CompilerAPIContainer() {
	efxc2Console::Console console = efxc2Console::console;
	h = LoadLibraryEx(DLL_NAME, nullptr, 0);
	if (h == nullptr) {
		console.PinkOutput();
		std::wcerr << M_FORMAT(L"{} failed to load ", DLL_NAME);
		std::cerr << std::endl;
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
	ptr_D3DCompile2 = std::bit_cast<pD3DCompile2g>(GetProcAddress(h, FCN_D3DCompile2));
#ifdef _MSC_VER
#pragma warning( pop )
#endif
	if (ptr_D3DCompile2 == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DCompile2);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DStripShader = std::bit_cast<pD3DStripShaderg>(GetProcAddress(h, FCN_D3DStripShader)); 
	if (ptr_D3DStripShader == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DStripShader);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DGetBlobPart = std::bit_cast<pD3DGetBlobPartg>(GetProcAddress(h, FCN_D3DGetBlobPart));
	if (ptr_D3DGetBlobPart == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DGetBlobPart);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DSetBlobPart = std::bit_cast<pD3DSetBlobPartg>(GetProcAddress(h, FCN_D3DSetBlobPart));
	if (ptr_D3DSetBlobPart == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DSetBlobPart);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DDisassemble = std::bit_cast<pD3DDisassembleg>(GetProcAddress(h, FCN_D3DDisassemble)); 
	if (ptr_D3DDisassemble == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DDisassemble);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DLoadModule = std::bit_cast<gD3DLoadModulep>(GetProcAddress(h, FCN_D3DLoadModule)); 
	if (ptr_D3DLoadModule == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DLoadModule);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DCreateLinker = std::bit_cast<gD3DCreateLinkerp>(GetProcAddress(h, FCN_D3DCreateLinker));
	if (ptr_D3DCreateLinker == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DCreateLinker);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DCreateFunctionLinkingGraph = std::bit_cast<gD3DCreateFunctionLinkingGraphp>(GetProcAddress(h, FCN_D3DCreateFunctionLinkingGraph));
	if (ptr_D3DCreateFunctionLinkingGraph == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DCreateFunctionLinkingGraph);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
	ptr_D3DPreprocess = std::bit_cast<gD3DPreprocessp>(GetProcAddress(h, FCN_D3DPreprocess));
	if (ptr_D3DPreprocess == nullptr) {
		console.PinkOutput();
		std::cerr << M_FORMAT(ERR_LOAD_FCN, FCN_D3DPreprocess);
		console.ResetOutput();
		efxc2Utils::print_windows_error();
		throw efxc2Exception::Win32APIFailure();
	}
}