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

void efxc2CompilerParams::CompilerParams::initializeDefines() {
    defines = std::make_shared<std::vector < efxc2Utils::CompilerDefine >>();
}

void efxc2CompilerParams::CompilerParams::add_define(const std::string_view defineOption) {
    efxc2Utils::CompilerDefine _def;
    _def.Definition = "1";

    if (size_t Dellin_pos = defineOption.find('='); Dellin_pos == std::string::npos) {
        _def.Name = defineOption;
    }
    else {
        _def.Name = defineOption.substr(0, Dellin_pos);
        _def.Definition = defineOption.substr(Dellin_pos + 1, defineOption.length() - 1);
    }

    (void)defines->insert(defines->begin(), _def);
}

void efxc2CompilerParams::CompilerParams::LoadSourceCode(std::ifstream& f) {
    if (efxc2Utils::readAll(f, SourceCode) == false) {
        std::cerr << "Error reading file." << std::endl;
        exit(1); //-V2014
    }

}

void efxc2CompilerParams::CompilerParams::LoadPrivateData(std::ifstream& f) {
    if (efxc2Utils::readAll(f, PrivateData) == false) {
        std::cerr << "Error reading file." << std::endl;
        exit(1);//-V2014
    }
}