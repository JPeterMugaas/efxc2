//--------------------------------------------------------------------------------------
// File: efxc2CompilerParams.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#include "efxc2CompilerParams.h"
#include "efxc2Utils.h"

void CompilerParams::initializeDefines() {
    numDefines = 1;
    defines = new D3D_SHADER_MACRO[numDefines];
    defines[numDefines - 1].Name = nullptr;
    defines[numDefines - 1].Definition = nullptr;
}

void CompilerParams::add_define(const std::string& defineOption) {
    numDefines++;
    //Copy the old array into the new array, but put the new definition at the beginning
    auto newDefines = new D3D_SHADER_MACRO[numDefines];
    for (size_t i = 1; i < numDefines; i++) {
        newDefines[i] = defines[i - 1];
    }
    delete[] defines;
    defines = newDefines;
    defines[0].Name = defineOption.c_str();
    defines[0].Definition = "1";
}

void CompilerParams::LoadSourceCode(FILE* f)
{
    readall(f, SourceCode);
    if (SourceCode != nullptr) {
        SourceLen = SourceCode->size();
    }
    else {
        SourceLen = 0;
    }
}
