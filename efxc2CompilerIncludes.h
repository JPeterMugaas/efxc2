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
private:
    int verbose = 1;
    std::vector<M_STRING> dirs;
public:

    CompilerIncludes() = default;
    virtual ~CompilerIncludes() = default;
};

#endif
