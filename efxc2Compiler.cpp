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

Compiler::Compiler() {
    h = LoadLibrary(DLL_NAME);
    if (h == nullptr) {
#ifdef _WIN32
        //Find the WINDOWS dll
        wchar_t dllPath[MAX_PATH];
        size_t bytes = GetModuleFileName(nullptr, dllPath, MAX_PATH);
        if (bytes == 0) {
            fprintf(stderr, "Could not retrieve the directory of the running executable.\n");
            print_windows_error();
        }
        //Fill rest of the buffer with NULLs
        memset(dllPath + bytes, '\0', MAX_PATH - bytes);
        //Copy the dll location over top efxc2.exe
        wcscpy_s(dllPath + bytes, MAX_PATH, DLL_NAME);

        h = LoadLibrary(dllPath);
        if (h == nullptr) {
            wprintf(L"Error: could not load " DLL_NAME L" from %s\n", dllPath);
        }

#else  /* _WIN32 */
        print_windows_error();
#endif /* _WIN32 */
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
        printf("Error: could not get the address of D3DCompile2.\n");
        print_windows_error();
    }
    ptr_D3DStripShader = (pD3DStripShaderg)GetProcAddress(h, "D3DStripShader");
    if (ptr_D3DStripShader == nullptr) {
        printf("Error: could not get the address of D3DStripShader.\n");
        print_windows_error();
    }
    ptr_D3DGetBlobPart = (pD3DGetBlobPartg)GetProcAddress(h, "D3DGetBlobPart");
    if (ptr_D3DGetBlobPart == nullptr) {
        printf("Error: could not get the address of D3DGetBlobPart.\n");
        print_windows_error();
    }
    ptr_D3DSetBlobPart = (pD3DSetBlobPartg)GetProcAddress(h, "D3DSetBlobPart");
    if (ptr_D3DGetBlobPart == nullptr) {
        printf("Error: could not get the address of D3DSetBlobPart.\n");
        print_windows_error();
    }
}

void Compiler::Compile(
    _In_ const char* SourceCode,
    _In_ const size_t SourceLen,
    _In_ const size_t numDefines,
    _In_reads_opt_(_Inexpressible_(defines->Name != NULL)) CONST D3D_SHADER_MACRO* defines) {
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
    HRESULT hr = ptr_D3DCompile2(
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
    auto* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
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
        hr = ptr_D3DStripShader(compiledString, compiledLen, strip_flags, &strippedBlob);
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
    unsigned char* outputString = nullptr;
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
    char* bytes;
    unsigned char* outputString = nullptr;
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
    hr = ptr_D3DGetBlobPart(compiledString, compiledLen, D3D_BLOB_DEBUG_NAME, 0, &pPDBName);
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
    auto pDebugNameData = reinterpret_cast<const ShaderDebugName*>(pPDBName->GetBufferPointer());
    auto pName = reinterpret_cast<const char*>(pDebugNameData + 1);
    printf(".PDB Data Name: %s\n", pName);
    return nullptr;
}

void Compiler::SetPDBFileName(_In_ char* _fileName) {
    // Blobs are always a multiple of 4 bytes long. Since DxilShaderDebugName
    // is itself 4 bytes, we pad the storage of the string (not the string itself)
    // to 4 bytes also.
    size_t fileNameLen = strlen(_fileName);

    size_t lengthOfNameStorage = (fileNameLen + 0x3) & ~0x3;
    size_t nameBlobPartSize = sizeof(ShaderDebugName) + lengthOfNameStorage;
    auto pNameBlobContent = reinterpret_cast<ShaderDebugName*>(malloc(nameBlobPartSize));

    // Ensure bytes after name are indeed zeroes:
    memset(pNameBlobContent, 0, nameBlobPartSize);
    pNameBlobContent->Flags = 0;
    // declared length does not include the null terminator:
    pNameBlobContent->NameLength = fileNameLen - 1;
    // but the null terminator is expected to be present:
    memcpy(pNameBlobContent + 1, _fileName, fileNameLen);

    auto* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
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
    HRESULT hr = 0;
    hr = ptr_D3DSetBlobPart(compiledString, compiledLen, D3D_BLOB_DEBUG_NAME, 0, pNameBlobContent,
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
}

size_t Compiler::WritePDBFile(FILE* f) {
    HRESULT hr = 0;
    if (f == nullptr) {
        ID3DBlob* PDBData = nullptr;
        unsigned char* compiledString = nullptr;
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
        hr = ptr_D3DGetBlobPart(compiledString, compiledLen, D3D_BLOB_PDB, 0, &PDBData);
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
        unsigned char* outputString = (unsigned char*)PDBData->GetBufferPointer();
        size_t outputLen = PDBData->GetBufferSize();
        fwrite(outputString, outputLen, 1, f);
        return compiledLen;
    }
    else {
        return 0;
    }
}