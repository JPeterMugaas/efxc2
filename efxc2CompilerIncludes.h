//--------------------------------------------------------------------------------------
// File: efxc2CompilerIncludes.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#pragma once

#ifndef EFXC2COMPILERINCLUDES_H_INCLUDED
#define EFXC2COMPILERINCLUDES_H_INCLUDED
#include "efxc2.h"
#include "efxc2Utils.h"

class CompilerIncludes : public ID3DInclude
{
    /* do not change this signature, it's part of an "inheritance" API. */
    HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
    /* do not change this signature, it's part of an "inheritance" API. */
    HRESULT Close(LPCVOID pData) override;
public:
    void AddIncludeDir(const M_STRING_VIEW& _dir);
    void set_verbose(int _verbose) { verbose = _verbose;  }
    int get_verbose() const { return verbose; }
    std::filesystem::path get_input_parent_path() { return input_parent_path;  }
    void set_input_parent_path(const std::filesystem::path& _input_parent_pat) { input_parent_path = _input_parent_pat; }
private:
    int verbose = 1;
    std::vector<std::filesystem::path> dirs;
public:
    std::filesystem::path input_parent_path;
    CompilerIncludes() = default;
    virtual ~CompilerIncludes() = default;
};

#endif
