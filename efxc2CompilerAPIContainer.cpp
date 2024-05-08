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
    h = LoadLibrary(DLL_NAME);
    if (h == nullptr) {
        efxc2Utils::print_windows_error();
    }
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
    ptr_D3DCompile2 = (pD3DCompile2g)GetProcAddress(h, "D3DCompile2");
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    if (ptr_D3DCompile2 == nullptr) {
        std::cerr << "Error: could not get the address of D3DCompile2.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DStripShader = (pD3DStripShaderg)GetProcAddress(h, "D3DStripShader");
    if (ptr_D3DStripShader == nullptr) {
        std::cerr << "Error: could not get the address of D3DStripShader.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DGetBlobPart = (pD3DGetBlobPartg)GetProcAddress(h, "D3DGetBlobPart");
    if (ptr_D3DGetBlobPart == nullptr) {
        std::cerr << "Error: could not get the address of D3DGetBlobPart.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DSetBlobPart = (pD3DSetBlobPartg)GetProcAddress(h, "D3DSetBlobPart");
    if (ptr_D3DSetBlobPart == nullptr) {
        std::cerr << "Error: could not get the address of D3DSetBlobPart.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DDisassemble = (pD3DDisassembleg)GetProcAddress(h, "D3DDisassemble");
    if (ptr_D3DDisassemble == nullptr) {
        std::cerr << "Error: could not get the address of D3DDisassemble.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DLoadModule = (gD3DLoadModulep)GetProcAddress(h, "D3DLoadModule");
    if (ptr_D3DLoadModule == nullptr) {
        std::cerr << "Error: could not get the address of D3DLoadModule.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DCreateLinker = (gD3DCreateLinkerp)GetProcAddress(h, "D3DCreateLinker");
    if (ptr_D3DCreateLinker == nullptr) {
        std::cerr << "Error: could not get the address of D3DCreateLinker.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DCreateFunctionLinkingGraph = (gD3DCreateFunctionLinkingGraphp)GetProcAddress(h, "D3DCreateFunctionLinkingGraph");
    if (ptr_D3DCreateFunctionLinkingGraph == nullptr) {
        std::cerr << "Error: could not get the address of D3DCreateFunctionLinkingGraph.\n";
        efxc2Utils::print_windows_error();
    }
    ptr_D3DPreprocess = (gD3DPreprocessp)GetProcAddress(h, "D3DPreprocess");
    if (ptr_D3DPreprocess == nullptr) {
        std::cerr << "Error: could not get the address of D3DPreprocess.\n";
        efxc2Utils::print_windows_error();
    }
}
