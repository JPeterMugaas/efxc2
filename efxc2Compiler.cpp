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
#include "efxc2Exception.h"
#include "efxc2Console.h"

void efxc2Compiler::print_D3D_SHADER_MACRO(D3D_SHADER_MACRO i) {
    if (i.Name != nullptr && i.Definition != nullptr) {
        std::cout << M_FORMAT(" {}={}", i.Name, i.Definition);
    }
}

void efxc2Compiler::SetupDefines(efxc2Utils::M_COMPILER_DEFINES _defines, std::vector<D3D_SHADER_MACRO>& defines) {
    D3D_SHADER_MACRO _def = { nullptr, nullptr };

    auto insert_def = [&_def, &defines](efxc2Utils::CompilerDefine const& a) {
        _def.Definition = a.Definition.c_str();
        _def.Name = a.Name.c_str();
        (void)defines.insert(defines.end(), _def); };

    (void)M_FOR_EACH(_defines->begin(), _defines->end(), insert_def);

    _def.Definition = nullptr;
    _def.Name = nullptr;
    (void)defines.insert(defines.end(), _def);
}

void efxc2Compiler::print_defines(std::vector<D3D_SHADER_MACRO> const& defines) {
    std::cout << "\t";
    (void)M_FOR_EACH(defines.begin(), defines.end(), print_D3D_SHADER_MACRO);
    std::cout << ",\n";
}

void efxc2Compiler::print_source_code_sample(efxc2Utils::M_BUFFER const& SourceCode) {
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
}

void efxc2Compiler::Compiler::Preprocess() {
    auto SourceCode = params.get_SourceCode();
    if (SourceCode == nullptr) {
        efxc2Utils::print_no_input_file();
        throw efxc2Exception::NoInputFile();
    }
    size_t SourceLen = SourceCode->size(); 
    auto  includeDirs = params.get_includeDirs();
    std::string _inputFile = params.get_inputFile();
    const char* inputFile = _inputFile.c_str();

    auto _defines = params.get_defines();
    auto defines = std::make_unique<std::vector<D3D_SHADER_MACRO>>();

    SetupDefines(_defines, *defines);

    ID3DBlob* errors = nullptr;
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DPreprocess(\n";
        print_source_code_sample(SourceCode);
        std::cout << M_FORMAT("\t {},\n", SourceLen);
        std::cout << M_FORMAT("\t {}, \n", inputFile);
        print_defines(*defines.get());
        std::cout << "\t D3D_COMPILE_STANDARD_FILE_INCLUDE,\n";
        std::cout << "\t &CompilerOutput,\n";
        std::cout << "\t &errors);\n";
    }
    /*
    HRESULT D3DPreprocess(
      [in]            LPCVOID                pSrcData,
      [in]            SIZE_T                 SrcDataSize,
      [in, optional]  LPCSTR                 pSourceName,
      [in, optional]  const D3D_SHADER_MACRO *pDefines,
      [in, optional]  ID3DInclude            *pInclude,
      [out]           ID3DBlob               **ppCodeText,
      [out, optional] ID3DBlob               **ppErrorMsgs
    );
    */
    auto ptr = api.get_ptr_D3DPreprocess();
    HRESULT hr = ptr(
        SourceCode.get()->data(),
        SourceLen,
        inputFile,
        defines->data(),
        includeDirs,
        &pPreprocessOutput,
        &errors);
    if (FAILED(hr)) {
        efxc2Console::Console console = efxc2Console::console;
        console.std_err_pink();
        std::cerr << "Preprocess error";
        console.std_out_pink();
        if (errors) {
            auto* error = M_BIT_CAST<char*>(errors->GetBufferPointer());
            std::cout << M_FORMAT("Got an error while preprocessing:\n{}\n", error);
            (void)errors->Release();
            std::cout << M_FORMAT("Error Code: {:#08x}", hr);
        }
        else {
            std::cout << M_FORMAT("Got an error {:#08x} while preprocessing, but no error message from the function.\n", hr);
            efxc2Utils::print_hresult_error(hr);
        }
        console.std_err_reset();
        console.std_out_reset();
        throw efxc2Exception::PreprocessError();
    }
}

void efxc2Compiler::Compiler::Compile() {
    auto SourceCode = params.get_SourceCode();
    if (SourceCode == nullptr) {
        efxc2Utils::print_no_input_file();
        throw efxc2Exception::NoInputFile();
    }
    size_t SourceLen = SourceCode->size(); 
    auto eflags = params.get_eflags();
    auto sflags = params.get_sflags();
    auto secondary_flags = params.get_secondary_flags();
    auto  includeDirs = params.get_includeDirs();
    std::string _entryPoint = params.get_entryPoint();
    const char* entryPoint = nullptr;
    if (!_entryPoint.empty()) {
        entryPoint = _entryPoint.c_str();
    }
    std::string _model = params.get_model();
    const char* model = _model.c_str();
    std::string _inputFile = params.get_inputFile();
    const char* inputFile = _inputFile.c_str();
    auto _defines = params.get_defines();
    auto defines = std::make_unique<std::vector<D3D_SHADER_MACRO>>();

    SetupDefines(_defines, *defines);

    ID3DBlob* errors = nullptr;
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DCompile2(\n";
        print_source_code_sample(SourceCode);
        std::cout << M_FORMAT("\t {},\n", SourceLen);
        std::cout << M_FORMAT("\t {}, \n", inputFile);
        print_defines( *defines.get() );
        std::cout << "\t D3D_COMPILE_STANDARD_FILE_INCLUDE,\n";
        if (entryPoint == nullptr) {
            std::cout << M_FORMAT("\t nullptr,\n");
        }
        else {
            std::cout << M_FORMAT("\t {},\n", entryPoint);
        }
        std::cout << M_FORMAT("\t {},\n", model);
        std::cout << M_FORMAT("\t {:#08x},\n", sflags);
        std::cout << M_FORMAT("\t {:#08x},\n", eflags);
        std::cout << M_FORMAT("\t {:#08x},\n", secondary_flags);
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
    auto ptr_D3DCompile2 = api.get_ptr_D3DCompile2();
    HRESULT hr = ptr_D3DCompile2(
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
        efxc2Console::Console console = efxc2Console::console;
        console.std_err_pink();
        console.std_out_pink();
        std::cerr << "Compile error";
        if (errors) {
            auto* error = M_BIT_CAST<char*>(errors->GetBufferPointer());
            std::cout << M_FORMAT("Got an error while compiling:\n{}\n", error);
            (void)errors->Release();
            std::cout << M_FORMAT("Error Code: {:#08x}", hr);
        }
        else {
            std::cout << M_FORMAT("Got an error {:#08x} while compiling, but no error message from the function.\n", hr);
            efxc2Utils::print_hresult_error(hr);
        }
        console.std_err_reset();
        console.std_out_reset();
        throw efxc2Exception::CompileError();
    }
    return;
}

void efxc2Compiler::Compiler::Disassemble() {
    auto const* compiledString = M_BIT_CAST<unsigned char*>(compilerOutput->GetBufferPointer());
    size_t compiledLen = compilerOutput->GetBufferSize();
    auto disassembly_flags = params.get_disassembly_flags();
    HRESULT hr = 0;
    disassemblyCodeBlob = nullptr;
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DDisassemble(\n";
        std::cout << "\t compiledString,\n";
        std::cout << M_FORMAT("\t {},\n", compiledLen);
        std::cout << M_FORMAT("\t {:#08x},\n", disassembly_flags);
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
        efxc2Utils::print_hresult_error(hr);
        throw efxc2Exception::Win32HLSLFailure();
    }
    return;
}

void efxc2Compiler::Compiler::StripShader() {
    auto strip_flags = params.get_strip_flags();
    strippedBlob = nullptr;

    if (strip_flags != 0) {
        if (params.get_verbose() && params.get_debug()) {
            std::cout << "Calling D3DStripShader(\n";
            std::cout << "\t compiledString,\n";
            std::cout << M_FORMAT("\t {},\n", compilerOutput->GetBufferSize());
            std::cout << M_FORMAT("\t {:#08x},\n", strip_flags);
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
        auto const* compiledString = M_BIT_CAST<char*>(compilerOutput->GetBufferPointer());
        size_t compiledLen = compilerOutput->GetBufferSize();
        HRESULT hr = 0;
        auto ptr = api.get_ptr_D3DStripShader();
        hr = ptr(compiledString, compiledLen, strip_flags, &strippedBlob);
#ifdef _MSC_VER
#pragma warning( pop )
#endif
        if (FAILED(hr)) {
            efxc2Utils::print_hresult_error(hr);
            throw efxc2Exception::Win32HLSLFailure();
        }
    }
    return;
}

size_t efxc2Compiler::Compiler::WriteAssemblyCode(std::ofstream& f) {
    char const* outputString = nullptr;
    size_t outputLen = 0;

    if (disassemblyCodeBlob != nullptr) {
        outputString = M_BIT_CAST<char*>(disassemblyCodeBlob->GetBufferPointer());
        outputLen = disassemblyCodeBlob->GetBufferSize();
        (void)f.write(outputString, outputLen);
    }
    return outputLen;
}

size_t efxc2Compiler::Compiler::WriteIncludeFile(std::ofstream& f)
{
    auto variableName = params.get_variableName();

    //Default output variable name
    if (variableName.empty()) {
        std::string model = params.get_model();
        std::string entryPoint = params.get_entryPoint();
        variableName = efxc2Utils::setupVariableName(model, entryPoint);
    }
    ID3DBlob* data;
    if (strippedBlob == nullptr) {
        data = compilerOutput;
    }
    else {
        data = strippedBlob;
    }
    efxc2Utils::WriteByteArrayConst(f, data, variableName, params.get_outputHex());
    return data->GetBufferSize();
}

size_t efxc2Compiler::Compiler::WriteObjectFile(std::ofstream& f) {
    char const* outputString;
    size_t outputLen = 0;
    if (strippedBlob == nullptr) {
        outputString = M_BIT_CAST<char*>(compilerOutput->GetBufferPointer());
        outputLen = compilerOutput->GetBufferSize();
    }
    else {
        outputString = M_BIT_CAST<char*>(strippedBlob->GetBufferPointer());
        outputLen = strippedBlob->GetBufferSize();
    }
    (void)f.write(outputString, outputLen);
    return outputLen;
}

size_t efxc2Compiler::Compiler::WritePreprocessFile(std::ofstream& f) {
    const char* outputString = M_BIT_CAST<char*>(pPreprocessOutput->GetBufferPointer());
    size_t outputLen = pPreprocessOutput->GetBufferSize();
    (void)f.write(outputString, outputLen);
    return outputLen;
}

std::string efxc2Compiler::Compiler::GetPDBFileName() {
    /*
    This is based on code I found at:

    https://devblogs.microsoft.com/pix/using-automatic-shader-pdb-resolution-in-pix/

    */

    auto const* compiledString = M_BIT_CAST<unsigned char*>(compilerOutput->GetBufferPointer());
    size_t compiledLen = compilerOutput->GetBufferSize();
    HRESULT hr = 0;
    /*Get filename*/
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DGetBlobPart(\n";
        std::cout << "\t compiledString,\n";
        std::cout << M_FORMAT("\t {},\n", compiledLen);
        std::cout << "\t D3D_BLOB_DEBUG_NAME,\n";
        std::cout << M_FORMAT("\t {:#08x},\n", 9);
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
        efxc2Utils::print_hresult_error(hr);
        throw efxc2Exception::Win32HLSLFailure();
    }
    char* pName = M_BIT_CAST<char*>(pPDBName->GetBufferPointer());
    /* Skip over reserved feild - uint_16 and length field uint_16*/
    for (int i = 0; i <= 3; i++) {
      ++pName;
    }
    if (params.get_verbose()) {
        std::cout << M_FORMAT(".PDB Data Name: {}\n", pName);
    }
    return pName;
}

void efxc2Compiler::Compiler::EmbedPrivateData() {
    auto const* compiledString = M_BIT_CAST<unsigned char*>(compilerOutput->GetBufferPointer());
    size_t compiledLen = compilerOutput->GetBufferSize();
    auto private_data = params.get_PrivateData();
    size_t private_data_size = private_data->size();  
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DSetBlobPart(\n";
        std::cout << "\t compiledString,\n";
        std::cout << M_FORMAT("\t {},\n", compiledLen);
        std::cout << "\t D3D_BLOB_PRIVATE_DATA,\n";
        std::cout << M_FORMAT("\t {:#08x},\n", 0);
        std::cout << "\t pNameBlobContent,\n";
        std::cout << M_FORMAT("\t {},\n", private_data_size);
        std::cout << "\t &compilerOutput);\n";
    }
    HRESULT hr;
    auto ptr = api.get_ptr_D3DSetBlobPart();
    hr = ptr(compiledString, compiledLen, D3D_BLOB_PRIVATE_DATA, 0, private_data->data(),
        private_data_size, &compilerOutput);
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
        efxc2Utils::print_hresult_error(hr);
        throw efxc2Exception::Win32HLSLFailure();
    }
}

void efxc2Compiler::Compiler::SetPDBFileName(_In_ const std::string_view _fileName) {
    /*
    This is based on code I found at:

    https://devblogs.microsoft.com/pix/using-automatic-shader-pdb-resolution-in-pix/

    */
    // Blobs are always a multiple of 4 bytes long. Since DxilShaderDebugName
    // is itself 4 bytes, we pad the storage of the string (not the string itself)
    // to 4 bytes also.
    size_t fileNameLen = std::size(_fileName);

    size_t lengthOfNameStorage = (fileNameLen + 0x3) & ~0x3;
    size_t nameBlobPartSize = sizeof(ShaderDebugName) + lengthOfNameStorage;

    std::vector<char> pNameBlobContent;
    pNameBlobContent.resize(nameBlobPartSize + sizeof(ShaderDebugName));
    // Ensure bytes after name are indeed zeroes:
    (void)std::ranges::fill(pNameBlobContent, 0);
    auto* header = M_BIT_CAST<ShaderDebugName*>(&pNameBlobContent[0]);

    header->Flags = 0;
    // declared length does not include the null terminator:
    // The typcast to uint16_t is deliberate because that struct stores a 16 bit value,
    header->NameLength = static_cast<uint16_t>(fileNameLen - 1);
    // but the null terminator is expected to be present:
    for (size_t i = 0; i < fileNameLen; i++) {
        pNameBlobContent[sizeof(ShaderDebugName) + i] = _fileName[i];
    }

    auto const* compiledString = M_BIT_CAST<unsigned char*>(compilerOutput->GetBufferPointer());
    size_t compiledLen = compilerOutput->GetBufferSize();

    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DSetBlobPart(\n";
        std::cout << "\t compiledString,\n";
        std::cout << M_FORMAT("\t {},\n", compiledLen);
        std::cout << "\t D3D_BLOB_DEBUG_NAME,\n";
        std::cout << M_FORMAT("\t {:#08x},\n", 0);
        std::cout << "\t pNameBlobContent,\n";
        std::cout << M_FORMAT("\t {},\n", nameBlobPartSize);
        std::cout << "\t &pShaderWithNewName);\n";
    }
    HRESULT hr;
    auto ptr = api.get_ptr_D3DSetBlobPart();
    hr = ptr(compiledString, compiledLen, D3D_BLOB_DEBUG_NAME, 0, pNameBlobContent.data(),
        nameBlobPartSize, &compilerOutput);
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
        efxc2Utils::print_hresult_error(hr);
        throw efxc2Exception::Win32HLSLFailure();
    }
}

size_t efxc2Compiler::Compiler::WritePDBFile(std::ofstream& f) {
    HRESULT hr = 0;
    ID3DBlob* PDBData = nullptr;
    char const* compiledString = nullptr;
    size_t compiledLen = 0;
    compiledString = M_BIT_CAST<char*>(compilerOutput->GetBufferPointer());
    compiledLen = compilerOutput->GetBufferSize();
    if (params.get_verbose() && params.get_debug()) {
        std::cout << "Calling D3DGetBlobPart(\n";
        std::cout << "\t compiledString,\n";
        std::cout << M_FORMAT("\t {},\n", compiledLen);
        std::cout << "\t D3D_BLOB_PDB,\n";
        std::cout << M_FORMAT("\t {:#08x},\n", 0);
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
        efxc2Utils::print_hresult_error(hr);
        throw efxc2Exception::Win32HLSLFailure();
    }
    auto outputString = M_BIT_CAST<char*>(PDBData->GetBufferPointer());
    size_t outputLen = PDBData->GetBufferSize();
    (void)f.write(outputString, outputLen);
    return compiledLen;
}