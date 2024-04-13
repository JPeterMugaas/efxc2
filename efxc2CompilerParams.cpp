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
    defines = std::make_shared<std::vector < CompilerDefine >> ();
}

void CompilerParams::add_define(const std::string_view& defineOption) {
    CompilerDefine _def;
    _def.Definition = "1";

    if (size_t delin_pos = defineOption.find('='); delin_pos == std::string::npos) {
        _def.Name = defineOption;
    }
    else {
        _def.Name = defineOption.substr(0, delin_pos);
        _def.Definition = defineOption.substr(delin_pos + 1, defineOption.length() - 1);
    }
    
    defines->insert(defines->begin(), _def);
}

void CompilerParams::LoadSourceCode(FILE* f)
{
    readall(f, SourceCode);
}
