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

void Compiler::Compile() {
    auto SourceCode = params.get_SourceCode();
    size_t SourceLen = SourceCode->size();
    auto eflags = params.get_eflags();
    auto sflags = params.get_sflags();
    auto secondary_flags = params.get_secondary_flags();
    std::string _entryPoint = params.get_entryPoint();
    const char* entryPoint = _entryPoint.c_str();
    std::string _model = params.get_model();
    const char* model = _model.c_str();
    std::string _inputFile = params.get_inputFile();
    const char* inputFile = _inputFile.c_str();
    auto _defines = params.get_defines();
    auto defines = std::make_unique<std::vector<D3D_SHADER_MACRO>>();
    D3D_SHADER_MACRO _def;
    for (size_t i = 0; i <  _defines->size(); i++) {
        _def.Definition = _defines.get()->at(i).Definition.c_str();
        _def.Name = _defines.get()->at(i).Name.c_str();
        defines->insert(defines->end(), _def);
    }
    _def.Definition = nullptr;
    _def.Name = nullptr;
    defines->insert(defines->end(), _def);
    ID3DBlob* errors = nullptr;
    if (params.get_verbose()) {
        printf("Calling D3DCompile2(\n");

        printf("\t SourceCode,\n");
        printf("\t %zu,\n", SourceLen);
        printf("\t %s, \n", inputFile);
        /* print defines */
        printf("\t");
        for (size_t i = 0; i < defines->size(); i++)
            if (defines->at(i).Name != nullptr) {
                printf(" %s=%s", defines->at(i).Name, defines->at(i).Definition);
            }
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
    auto ptr = api.get_ptr_D3DCompile2();
    HRESULT hr = ptr(
        SourceCode.get()->data(),
        SourceLen,
        inputFile,
        defines->data(),
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

void Compiler::Disassemble() {
    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();
    auto disassembly_flags = params.get_disassembly_flags();
    HRESULT hr = 0;
    disassemblyCodeBlob = nullptr;
    if (params.get_verbose()) {
        printf("Calling D3DDisassemble(\n");
        printf("\t compiledString,\n");
        printf("\t %zu,\n", compiledLen);
        printf("\t 0x%016" PRIx64 ", \n", (INT64)disassembly_flags);
        printf("\t nullptr, \n");
        printf("\t &disassemlyCodeBlob);");
    }
    /*HRESULT D3DDisassemble(
    [in]           LPCVOID  pSrcData,
    [in]           SIZE_T   SrcDataSize,
    [in]           UINT     Flags,
    [in, optional] LPCSTR   szComments,
    [out]          ID3DBlob **ppDisassembly
    ); */
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 6387)
#endif
    auto ptr = api.get_ptr_D3DDisassemble();
    hr = ptr(compiledString, compiledLen, disassembly_flags, nullptr, &disassemblyCodeBlob);
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    if (FAILED(hr)) {
        print_hresult_error(hr);
    }
}

void Compiler::StripShader() {
    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();
    auto strip_flags = params.get_strip_flags();
    HRESULT hr = 0;
    strippedBlob = nullptr;
    if (strip_flags != 0) {
        if (params.get_verbose()) {
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
}

size_t Compiler::WriteAssemblyCode(FILE* f) {
    unsigned char const* outputString = nullptr;
    size_t outputLen = 0;

    if (disassemblyCodeBlob != nullptr) {
        outputString = (unsigned char*)disassemblyCodeBlob->GetBufferPointer();
        outputLen = disassemblyCodeBlob->GetBufferSize();
        fwrite(outputString, outputLen, 1, f);
    }
    return outputLen;
}

size_t Compiler::WriteIncludeFile(FILE* f) {
    auto variableName = params.get_variableName();
    
    //Default output variable name
    if (variableName == "") {
        std::string model = params.get_model();
        std::string entryPoint = params.get_entryPoint();
        variableName = setupVariableName(model.c_str(), entryPoint.c_str());
    } 
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
    WriteByteArrayConst(f, outputString, outputLen, variableName.c_str(), params.get_outputHex());
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

std::string Compiler::GetPDBFileName() {
    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();
    HRESULT hr = 0;
    /*Get filename*/
    if (params.get_verbose()) {
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
    return pName;
}

void Compiler::SetPDBFileName(_In_ const std::string& _fileName) {
    // Blobs are always a multiple of 4 bytes long. Since DxilShaderDebugName
    // is itself 4 bytes, we pad the storage of the string (not the string itself)
    // to 4 bytes also.
    size_t fileNameLen = _fileName.length();

    size_t lengthOfNameStorage = (fileNameLen + 0x3) & ~0x3;
    size_t nameBlobPartSize = sizeof(ShaderDebugName) + lengthOfNameStorage;

    std::vector<char> pNameBlobContent;
    pNameBlobContent.resize(nameBlobPartSize + sizeof(ShaderDebugName));
    // Ensure bytes after name are indeed zeroes:
    std::fill(pNameBlobContent.begin(), pNameBlobContent.end(), 0);
    auto* header = reinterpret_cast<ShaderDebugName*>(&pNameBlobContent[0]);

    header->Flags = 0;
    // declared length does not include the null terminator:
    header->NameLength = (uint16_t)fileNameLen - 1;
    // but the null terminator is expected to be present:
    for (int i = 0; i < _fileName.length(); i++) {
        pNameBlobContent[sizeof(ShaderDebugName) + i] = _fileName[i];
    }

    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();

    if (params.get_verbose()) {
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
    hr = ptr(compiledString, compiledLen, D3D_BLOB_DEBUG_NAME, 0, pNameBlobContent.data(),
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
    if (params.get_verbose()) {
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
