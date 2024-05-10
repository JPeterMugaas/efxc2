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

efxc2CompilerAPIContainer::CompilerAPIContainer::CompilerAPIContainer() {
    h = LoadLibraryEx(DLL_NAME,nullptr,0);
    if (h == nullptr) {
        efxc2Utils::print_windows_error();
    }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
    ptr_D3DCompile2 = std::bit_cast<pD3DCompile2g>(GetProcAddress(h, "D3DCompile2")); //-V3551 //-V2578
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    if (ptr_D3DCompile2 == nullptr) {
        std::cerr << "Error: could not get the address of D3DCompile2.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DStripShader = std::bit_cast<pD3DStripShaderg>(GetProcAddress(h, "D3DStripShader")); //-V3551 //-V2578
    if (ptr_D3DStripShader == nullptr) {
        std::cerr << "Error: could not get the address of D3DStripShader.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DGetBlobPart = std::bit_cast<pD3DGetBlobPartg>(GetProcAddress(h, "D3DGetBlobPart")); //-V3551 //-V2578
    if (ptr_D3DGetBlobPart == nullptr) {
        std::cerr << "Error: could not get the address of D3DGetBlobPart.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DSetBlobPart = std::bit_cast<pD3DSetBlobPartg>(GetProcAddress(h, "D3DSetBlobPart")); //-V3551 //-V2578
    if (ptr_D3DSetBlobPart == nullptr) {
        std::cerr << "Error: could not get the address of D3DSetBlobPart.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DDisassemble = std::bit_cast<pD3DDisassembleg>(GetProcAddress(h, "D3DDisassemble")); //-V3551 //-V2578
    if (ptr_D3DDisassemble == nullptr) {
        std::cerr << "Error: could not get the address of D3DDisassemble.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DLoadModule = std::bit_cast<gD3DLoadModulep>(GetProcAddress(h, "D3DLoadModule")); //-V3551 //-V2578
    if (ptr_D3DLoadModule == nullptr) {
        std::cerr << "Error: could not get the address of D3DLoadModule.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DCreateLinker = std::bit_cast<gD3DCreateLinkerp>(GetProcAddress(h, "D3DCreateLinker")); //-V3551 //-V2578
    if (ptr_D3DCreateLinker == nullptr) {
        std::cerr << "Error: could not get the address of D3DCreateLinker.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DCreateFunctionLinkingGraph = std::bit_cast<gD3DCreateFunctionLinkingGraphp>(GetProcAddress(h, "D3DCreateFunctionLinkingGraph")); //-V3551 //-V2578
    if (ptr_D3DCreateFunctionLinkingGraph == nullptr) {
        std::cerr << "Error: could not get the address of D3DCreateFunctionLinkingGraph.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DPreprocess = std::bit_cast<gD3DPreprocessp>(GetProcAddress(h, "D3DPreprocess")); //-V3551 //-V2578
    if (ptr_D3DPreprocess == nullptr) {
        std::cerr << "Error: could not get the address of D3DPreprocess.\n";
        efxc2Utils::print_windows_error();
    }
}
