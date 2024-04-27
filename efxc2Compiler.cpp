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

static void DisplayDefine(D3D_SHADER_MACRO i) {
    if (i.Name != nullptr && i.Definition != nullptr) {
        std::cout << std::format("{}={}", i.Name, i.Definition);
    }
}

void Compiler::Compile() {
    auto SourceCode = params.get_SourceCode();
    size_t SourceLen = SourceCode->size();
    auto eflags = params.get_eflags();
    auto sflags = params.get_sflags();
    auto secondary_flags = params.get_secondary_flags();
    auto  includeDirs = params.get_includeDirs();
    std::string _entryPoint = params.get_entryPoint();
    const char* entryPoint = _entryPoint.c_str();
    std::string _model = params.get_model();
    const char* model = _model.c_str();
    std::string _inputFile = params.get_inputFile();
    const char* inputFile = _inputFile.c_str();
    auto _defines = params.get_defines();
    auto defines = std::make_unique<std::vector<D3D_SHADER_MACRO>>();
    D3D_SHADER_MACRO _def = { nullptr, nullptr };
    for (size_t i = 0; i <  _defines->size(); i++) {
        _def.Definition = _defines.get()->at(i).Definition.c_str();
        _def.Name = _defines.get()->at(i).Name.c_str();
        defines->insert(defines->end(), _def);
    }
    _def.Definition = nullptr;
    _def.Name = nullptr;
    defines->insert(defines->end(), _def);
    ID3DBlob* errors = nullptr;
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DCompile2(\n";
        /* Source Code sample*/
        std::cout << "\t \"";
        for (size_t i = 0; i < SourceCode->size(); ++i) {
            if (i < 41) {
                std::cout << SourceCode->at(i);
            }
            else {
                std::cout << "...";
                break;
            }
        }
        std::cout << "\",\n";
        /**/
        std::cout << std::format("\t {},\n", SourceLen);
        std::cout << std::format("\t {}, \n", inputFile);
        /* print defines */
        std::cout << "\t";
        std::ranges::for_each(defines->begin(), defines->end(), DisplayDefine);
        std::cout << ",\n";
        /* done printing defines */
        std::cout << "\t D3D_COMPILE_STANDARD_FILE_INCLUDE,\n";
        std::cout << std::format("\t {},\n", entryPoint);
        std::cout << std::format("\t {},\n", model);
        std::cout << std::format("\t {:#08x},\n", sflags);
        std::cout << std::format("\t {:#08x},\n", eflags);
        std::cout << std::format("\t {:#08x},\n", secondary_flags);
        std::cout << "\t nullptr,\n";
        std::cout << "\t 0,\n";
        std::cout << "\t &CompilerOutput,\n";
        std::cout << "\t &errors);\n";
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
        includeDirs,
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
        std::cerr << "Compile error";
        if (errors) {
            auto* error = (char*)errors->GetBufferPointer();
            std::cout << std::format("Got an error while compiling:\n{}\n", error);
            errors->Release();
            std::cout << std::format( "Error Code: {:#08x}", hr);
        }
        else {
            std::cout << std::format( "Got an error {:#08x} while compiling, but no error message from the function.\n", hr);
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
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DDisassemble(\n";
        std::cout << "\t compiledString,\n";
        std::cout << std::format("\t {},\n", compiledLen);
        std::cout << std::format("\t {:#08x},\n", disassembly_flags);
        std::cout << "\t nullptr, \n";
        std::cout << "\t nullptr, \n";
        std::cout << "\t &disassemlyCodeBlob);\n";
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
        if (params.get_verbose() && params.get_debug()) {
            std::cout << "Calling D3DStripShader(\n";
            std::cout << "\t compiledString,\n";
            std::cout << std::format("\t {},\n", compiledLen);
            std::cout << std::format("\t {:#08x},\n", strip_flags);
            std::cout << "\t &strippedBlob);\n";
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

size_t Compiler::WriteAssemblyCode(std::ofstream& f) {
    char const* outputString = nullptr;
    size_t outputLen = 0;

    if (disassemblyCodeBlob != nullptr) {
        outputString = (char*)disassemblyCodeBlob->GetBufferPointer();
        outputLen = disassemblyCodeBlob->GetBufferSize();
        f.write(outputString, outputLen);
    }
    return outputLen;
}

size_t Compiler::WriteIncludeFile(std::ofstream& f)
{
    auto variableName = params.get_variableName();
       
    //Default output variable name
    if (variableName == "") {
        std::string model = params.get_model();
        std::string entryPoint = params.get_entryPoint();
        variableName = setupVariableName(model.c_str(), entryPoint.c_str());
    }
    ID3DBlob* data;
    if (strippedBlob == nullptr) {
        data = compilerOutput;
    }
    else {
        data = strippedBlob;
    }
    WriteByteArrayConst(f, data, variableName, params.get_outputHex());
    return data->GetBufferSize();
}

size_t Compiler::WriteObjectFile(std::ofstream& f) {
    char const* outputString;
    size_t outputLen = 0;
    if (strippedBlob == nullptr) {
        outputString = (char*)compilerOutput->GetBufferPointer();
        outputLen = compilerOutput->GetBufferSize();
    }
    else {
        outputString = (char*)strippedBlob->GetBufferPointer();
        outputLen = strippedBlob->GetBufferSize();
    }
    f.write(outputString, outputLen);
    return outputLen;
}

std::string Compiler::GetPDBFileName() {
    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();
    HRESULT hr = 0;
    /*Get filename*/
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DGetBlobPart(\n";
        std::cout << "\t compiledString,\n";
        std::cout << std::format("\t {},\n", compiledLen);
        std::cout << "\t D3D_BLOB_DEBUG_NAME,\n";
        std::cout << std::format("\t {:#08x},\n", 9);
        std::cout << "\t & pPDBName); \n";
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
    if (params.get_verbose()) {
        std::cout << std::format(".PDB Data Name: {}\n", pName);
    }
    return pName;
}

void Compiler::SetPDBFileName(_In_ const std::string_view& _fileName) {
    // Blobs are always a multiple of 4 bytes long. Since DxilShaderDebugName
    // is itself 4 bytes, we pad the storage of the string (not the string itself)
    // to 4 bytes also.
    size_t fileNameLen = _fileName.length();

    size_t lengthOfNameStorage = (fileNameLen + 0x3) & ~0x3;
    size_t nameBlobPartSize = sizeof(ShaderDebugName) + lengthOfNameStorage;

    std::vector<char> pNameBlobContent;
    pNameBlobContent.resize(nameBlobPartSize + sizeof(ShaderDebugName));
    // Ensure bytes after name are indeed zeroes:
    std::ranges::fill(pNameBlobContent.begin(), pNameBlobContent.end(), 0);
    auto* header = std::bit_cast<ShaderDebugName*>(&pNameBlobContent[0]);

    header->Flags = 0;
    // declared length does not include the null terminator:
    header->NameLength = (uint16_t)fileNameLen - 1;
    // but the null terminator is expected to be present:
    for (int i = 0; i < _fileName.length(); i++) {
        pNameBlobContent[sizeof(ShaderDebugName) + i] = _fileName[i];
    }

    auto const* compiledString = (unsigned char*)compilerOutput->GetBufferPointer();
    size_t compiledLen = compilerOutput->GetBufferSize();

    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DSetBlobPart(\n";
        std::cout << "\t compiledString,\n";
        std::cout << std::format("\t {},\n", compiledLen);
        std::cout << "\t D3D_BLOB_DEBUG_NAME,\n";
        std::cout << std::format("\t {:#08x},\n", 0);
        std::cout << "\t pNameBlobContent,\n";
        std::cout <<  std::format("\t {},\n", nameBlobPartSize);
        std::cout << "\t &pShaderWithNewName);\n";
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

size_t Compiler::WritePDBFile(std::ofstream& f) {
    HRESULT hr = 0;
    ID3DBlob* PDBData = nullptr;
    char const* compiledString = nullptr;
    size_t compiledLen = 0;
    if (pShaderWithNewName == nullptr) {
        compiledString = (char*)compilerOutput->GetBufferPointer();
        compiledLen = compilerOutput->GetBufferSize();
    }
    else {
        compiledString = (char*)pShaderWithNewName->GetBufferPointer();
        compiledLen = pShaderWithNewName->GetBufferSize();
    }
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DGetBlobPart(\n";
        std::cout << "\t compiledString,\n";
        std::cout << std::format("\t {},\n", compiledLen);
        std::cout << "\t D3D_BLOB_PDB,\n";
        std::cout << std::format("\t {:#08x},\n", (INT64)0);
        std::cout << "\t &PDBData);\n";
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
    auto outputString = (char*)PDBData->GetBufferPointer();
    size_t outputLen = PDBData->GetBufferSize();
    f.write(outputString, outputLen);
    return compiledLen;
}
