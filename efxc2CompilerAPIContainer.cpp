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
    /* for some reason, when compiling for Clang 18.1.4, you get this warning V2005 saying you should do not
       C-type casts.  I suspect that this is how they implemented std::bit_cast.  The V2005 also needs to 
       be updated to indicate that bit_cast is also a valid option for replacing C-style typecasting.*/
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
    ptr_D3DCompile2 = std::bit_cast<pD3DCompile2g>(GetProcAddress(h, (const char*)"D3DCompile2"));  //-V2005 //-V2533
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    if (ptr_D3DCompile2 == nullptr) {
        std::cerr << "Error: could not get the address of D3DCompile2.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
    ptr_D3DStripShader = std::bit_cast<pD3DStripShaderg>(GetProcAddress(h, (const char*)"D3DStripShader")); //-V2005 //-V2533
    if (ptr_D3DStripShader == nullptr) {
        std::cerr << "Error: could not get the address of D3DStripShader.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
    ptr_D3DGetBlobPart = std::bit_cast<pD3DGetBlobPartg>(GetProcAddress(h, (const char*)"D3DGetBlobPart"));  //-V2005 //-V2533
    if (ptr_D3DGetBlobPart == nullptr) {
        std::cerr << "Error: could not get the address of D3DGetBlobPart.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }           
    ptr_D3DSetBlobPart = std::bit_cast<pD3DSetBlobPartg>(GetProcAddress(h, (const char*)"D3DSetBlobPart")); //-V2005 //-V2533
    if (ptr_D3DSetBlobPart == nullptr) {
        std::cerr << "Error: could not get the address of D3DSetBlobPart.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
    ptr_D3DDisassemble = std::bit_cast<pD3DDisassembleg>(GetProcAddress(h, (const char*)"D3DDisassemble"));  //-V2005 //-V2533
    if (ptr_D3DDisassemble == nullptr) {
        std::cerr << "Error: could not get the address of D3DDisassemble.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
    ptr_D3DLoadModule = std::bit_cast<gD3DLoadModulep>(GetProcAddress(h, (const char*)"D3DLoadModule"));  //-V2005 //-V2533
    if (ptr_D3DLoadModule == nullptr) {
        std::cerr << "Error: could not get the address of D3DLoadModule.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
    ptr_D3DCreateLinker = std::bit_cast<gD3DCreateLinkerp>(GetProcAddress(h, (const char*)"D3DCreateLinker")); //-V2005 //-V2533
    if (ptr_D3DCreateLinker == nullptr) {
        std::cerr << "Error: could not get the address of D3DCreateLinker.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
    ptr_D3DCreateFunctionLinkingGraph = std::bit_cast<gD3DCreateFunctionLinkingGraphp>(GetProcAddress(h, (const char*)"D3DCreateFunctionLinkingGraph"));  //-V2005 //-V2533
    if (ptr_D3DCreateFunctionLinkingGraph == nullptr) {
        std::cerr << "Error: could not get the address of D3DCreateFunctionLinkingGraph.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
    ptr_D3DPreprocess = std::bit_cast<gD3DPreprocessp>(GetProcAddress(h, (const char*)"D3DPreprocess")); //-V2005 //-V2533
    if (ptr_D3DPreprocess == nullptr) {
        std::cerr << "Error: could not get the address of D3DPreprocess.\n";
        efxc2Utils::print_windows_error();
        throw efxc2Exception::Win32APIFailure();
    }
}
