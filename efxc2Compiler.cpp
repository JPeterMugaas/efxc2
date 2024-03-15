//--------------------------------------------------------------------------------------
// File: efxc2Compiler.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------
#include "efxc2Utils.h"
#include "efxc2Compiler.h"

void Compiler::initializeDefines() {
    numDefines = 1;
    defines = new D3D_SHADER_MACRO[numDefines];
    defines[numDefines - 1].Name = nullptr;
    defines[numDefines - 1].Definition = nullptr;
}

void Compiler::Compile() {
    //Default output variable name
    if (variableName == nullptr) {
        variableName = setupVariableName(model, entryPoint);
    }
    ID3DBlob* errors = nullptr;
    if (verbose) {
        printf("Calling D3DCompile2(\n");

        printf("\t SourceCode,\n");
        printf("\t %zu,\n", SourceLen);
        printf("\t %s, \n", inputFile);
        /* print defines */
        printf("\t");
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#endif
        for (size_t i = 0; i < numDefines - 1; i++)
            printf(" %s=%s", defines[i].Name, defines[i].Definition);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
        printf(",\n");
        /* done printing defines */
        printf("\t D3D_COMPILE_STANDARD_FILE_INCLUDE,\n");
        printf("\t %s,\n", entryPoint);
        printf("\t %s,\n", model);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)sflags);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)eflags);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)secondary_flags);
        printf("\t nullptr,\n");
        printf("\t 0,\n");
        printf("\t &CompilerOutput,\n");
        printf("\t &errors);\n");
    }

    /*
    HRESULT D3DCompile2(
       [in]            LPCVOID                pSrcData,
       [in]            SIZE_T                 SrcDataSize,
       [in, optional]  LPCSTR                 pSourceName,
       [in, optional]  const D3D_SHADER_MACRO *pDefines,
       [in, optional]  ID3DInclude            *pInclude,
       [in]            LPCSTR                 pEntrypoint,
       [in]            LPCSTR                 pTarget,
       [in]            UINT                   Flags1,
       [in]            UINT                   Flags2,
       [in]            UINT                   SecondaryDataFlags,
       [in, optional]  LPCVOID                pSecondaryData,
       [in]            SIZE_T                 SecondaryDataSize,
       [out]           ID3DBlob               **ppCode,
       [out, optional] ID3DBlob               **ppErrorMsgs
     );
    */
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
    auto ptr = api.get_ptr_D3DCompile2();
    HRESULT hr = ptr(
        SourceCode,
        SourceLen,
        inputFile,
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        model,
        sflags,
        eflags,
        secondary_flags,
        nullptr,
        0,
        &compilerOutput,
        &errors
    );
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    if (FAILED(hr)) {
        if (errors) {
            auto* error = (char*)errors->GetBufferPointer();
            fprintf(stderr, "Got an error while compiling:\n%s\n", error);
            errors->Release();
            printf("Error Code: 0x%lx", hr);
        }
        else {
            fprintf(stderr, "Got an error (%lx) while compiling, but no error message from the function.\n", hr);
            print_hresult_error(hr);
        }
        exit(1);
    }
}

void Compiler::StripShader() {
    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();
    HRESULT hr = 0;
    strippedBlob = nullptr;
    if (strip_flags != 0) {
        if (verbose) {
            printf("Calling D3DStripShader(\n");
            printf("\t compiledString,\n");
            printf("\t %zu,\n", compiledLen);
            printf("\t 0x%016" PRIx64 ", \n", (INT64)strip_flags);
            printf("\t &strippedBlob);\n");
        }
        /*
        HRESULT D3DStripShader(
          [in]  LPCVOID  pShaderBytecode,
          [in]  SIZE_T   BytecodeLength,
          [in]  UINT     uStripFlags,
          [out] ID3DBlob **ppStrippedBlob
        );
        */
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
        auto ptr = api.get_ptr_D3DStripShader();
        hr = ptr(compiledString, compiledLen, strip_flags, &strippedBlob);
#ifdef _MSC_VER
#pragma warning( pop )
#endif
        if (FAILED(hr)) {
            print_hresult_error(hr);
        }
    }
    else {
        strippedBlob = nullptr;
    }
}

size_t Compiler::WriteIncludeFile(FILE* f) {
    unsigned char const* outputString = nullptr;
    size_t outputLen = 0;
    if (strippedBlob == nullptr) {
        outputString = (unsigned char*)compilerOutput->GetBufferPointer();
        outputLen = compilerOutput->GetBufferSize();
    }
    else {
        outputString = (unsigned char*)strippedBlob->GetBufferPointer();
        outputLen = strippedBlob->GetBufferSize();
    }
    WriteByteArrayConst(f, outputString, outputLen, variableName, outputHex);
    return outputLen;
}

size_t Compiler::WriteObjectFile(FILE* f) {
    unsigned char const* outputString;
    size_t outputLen = 0;
    if (strippedBlob == nullptr) {
        outputString = (unsigned char*)compilerOutput->GetBufferPointer();
        outputLen = compilerOutput->GetBufferSize();
    }
    else {
        outputString = (unsigned char*)strippedBlob->GetBufferPointer();
        outputLen = strippedBlob->GetBufferSize();
    }
    fwrite(outputString, outputLen, 1, f);
    return outputLen;
}

char* Compiler::GetPDBFileName() {
    auto* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();
    HRESULT hr = 0;
    /*Get filename*/
    if (verbose) {
        printf("Calling D3DGetBlobPart(\n");
        printf("\t compiledString,\n");
        printf("\t %zu,\n", compiledLen);
        printf("\t D3D_BLOB_DEBUG_NAME,\n");
        printf("\t 0x%016" PRIx64 ", \n", (INT64)0);
        printf("\t &pPDBName);\n");
    }
    auto ptr = api.get_ptr_D3DGetBlobPart();
    hr = ptr(compiledString, compiledLen, D3D_BLOB_DEBUG_NAME, 0, &pPDBName);
    /*
    HRESULT D3DGetBlobPart(
    [in]  LPCVOID       pSrcData,
    [in]  SIZE_T        SrcDataSize,
    [in]  D3D_BLOB_PART Part,
    [in]  UINT          Flags,
    [out] ID3DBlob      **ppPart
    );
    */
    if (FAILED(hr)) {
        print_hresult_error(hr);
    }
    auto pDebugNameData = (ShaderDebugName*)(pPDBName->GetBufferPointer());
    auto pName = (char*)(pDebugNameData + 1);
    printf(".PDB Data Name: %s\n", pName);
    return (char*)pName;
}

void Compiler::SetPDBFileName(_In_ char* _fileName) {
    // Blobs are always a multiple of 4 bytes long. Since DxilShaderDebugName
    // is itself 4 bytes, we pad the storage of the string (not the string itself)
    // to 4 bytes also.
    size_t fileNameLen = strlen(_fileName);

    size_t lengthOfNameStorage = (fileNameLen + 0x3) & ~0x3;
    size_t nameBlobPartSize = sizeof(ShaderDebugName) + lengthOfNameStorage;
    auto pNameBlobContent = (ShaderDebugName*)(malloc(nameBlobPartSize));

    if (pNameBlobContent == nullptr) {
        fprintf(stderr, "malloc failed/n");
        print_errno();
    }
    // Ensure bytes after name are indeed zeroes:
    memset(pNameBlobContent, 0, nameBlobPartSize);
    pNameBlobContent->Flags = 0;
    // declared length does not include the null terminator:
    pNameBlobContent->NameLength = (uint16_t)fileNameLen - 1;
    // but the null terminator is expected to be present:
    memcpy(pNameBlobContent + 1, _fileName, fileNameLen);

    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();

    if (verbose) {
        printf("Calling D3DSetBlobPart(\n");
        printf("\t compiledString,\n");
        printf("\t %zu,\n", compiledLen);
        printf("\t D3D_BLOB_DEBUG_NAME,\n");
        printf("\t 0x%016" PRIx64 ", \n", (INT64)0);
        printf("\t pNameBlobContent,\n");
        printf("\t %zu,\n", nameBlobPartSize);
        printf("\t &pShaderWithNewName);\n");
    }
    HRESULT hr;
    auto ptr = api.get_ptr_D3DSetBlobPart();
    hr = ptr(compiledString, compiledLen, D3D_BLOB_DEBUG_NAME, 0, pNameBlobContent,
        nameBlobPartSize, &pShaderWithNewName);
    /*
    HRESULT D3DSetBlobPart(
    [in]  LPCVOID       pSrcData,
    [in]  SIZE_T        SrcDataSize,
    [in]  D3D_BLOB_PART Part,
    [in]  UINT          Flags,
    [in]  LPCVOID       pPart,
    [in]  SIZE_T        PartSize,
    [out] ID3DBlob      **ppNewShader);
    */
    if (FAILED(hr)) {
        print_hresult_error(hr);
    }
}

size_t Compiler::WritePDBFile(FILE* f) {
    HRESULT hr = 0;
    ID3DBlob* PDBData = nullptr;
    unsigned char const* compiledString = nullptr;
    size_t compiledLen = 0;
    if (pShaderWithNewName == nullptr) {
        compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
        compiledLen = compilerOutput->GetBufferSize();
    }
    else {
        compiledString = (unsigned char*)pShaderWithNewName->GetBufferPointer();
        compiledLen = pShaderWithNewName->GetBufferSize();
    }
    if (verbose) {
        printf("Calling D3DGetBlobPart(\n");
        printf("\t compiledString,\n");
        printf("\t %zu,\n", compiledLen);
        printf("\t D3D_BLOB_PDB,\n");
        printf("\t 0x%016" PRIx64 ", \n", (INT64)0);
        printf("\t &PDBData);\n");
    }
    auto ptr = api.get_ptr_D3DGetBlobPart();
    hr = ptr(compiledString, compiledLen, D3D_BLOB_PDB, 0, &PDBData);
    /*
    HRESULT D3DGetBlobPart(
    [in]  LPCVOID       pSrcData,
    [in]  SIZE_T        SrcDataSize,
    [in]  D3D_BLOB_PART Part,
    [in]  UINT          Flags,
    [out] ID3DBlob      **ppPart);
    */
    if (FAILED(hr)) {
        print_hresult_error(hr);
    }
    auto outputString = (unsigned char*)PDBData->GetBufferPointer();
    size_t outputLen = PDBData->GetBufferSize();
    fwrite(outputString, outputLen, 1, f);
    return compiledLen;
}

void Compiler::LoadSourceCode(FILE* f) {
    readall(f, &SourceCode, &SourceLen);
}

void Compiler::add_define(char* defineOption) {
    assert(defineOption == nullptr);
    numDefines++;
    //Copy the old array into the new array, but put the new definition at the beginning
    auto newDefines = new D3D_SHADER_MACRO[numDefines];
    for (size_t i = 1; i < numDefines; i++) {
        newDefines[i] = defines[i - 1];
    }
    delete[] defines;
    defines = newDefines;
    defines[0].Name = defineOption;
    defines[0].Definition = "1";
}
