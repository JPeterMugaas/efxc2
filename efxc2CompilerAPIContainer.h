//--------------------------------------------------------------------------------------
// File: efxc2CompilerAPIContainer.h
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef EFXC2COMPILERAPICONTAINER_H_INCLUDED
#define EFXC2COMPILERAPICONTAINER_H_INCLUDED
#include "efxc2.h"

class CompilerAPIContainer {
public:
	explicit CompilerAPIContainer();
	pD3DCompile2g get_ptr_D3DCompile2() const { return  ptr_D3DCompile2; }
	pD3DStripShaderg get_ptr_D3DStripShader() const { return ptr_D3DStripShader; }
	pD3DGetBlobPartg get_ptr_D3DGetBlobPart() const { return ptr_D3DGetBlobPart; }
	pD3DSetBlobPartg get_ptr_D3DSetBlobPart() const { return ptr_D3DSetBlobPart; }
private:
	HMODULE get_h() const { return h; }
	HMODULE          h;
	pD3DCompile2g    ptr_D3DCompile2;
	pD3DStripShaderg ptr_D3DStripShader;
	pD3DGetBlobPartg ptr_D3DGetBlobPart;
	pD3DSetBlobPartg ptr_D3DSetBlobPart;
};

#endif /*EFXC2COMPILERAPICONTAINER_H_INCLUDED*/