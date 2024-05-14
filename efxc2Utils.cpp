//--------------------------------------------------------------------------------------
// File: efxc2Utils.cpp
//
// Copyright (c) J. Peter Mugaas
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//--------------------------------------------------------------------------------------

#include "efxc2Utils.h"

std::string efxc2Utils::HResultName(_In_ const HRESULT hr) { 
    for (int i = 0; i < efxc2Utils::ERROR_TABLE_LENGTH; i++) {
        if (hr == efxc2Utils::g_ErrorTable[i].ErrorCode ) {
            return  efxc2Utils::g_ErrorTable[i].ErrorName; 
        }
    }
    return "Unknown Error Name";
}

void efxc2Utils::print_copyright() {
    std::cout << PROGRAM_DESCRIPTION " " PROGRAM_VERSION "\n";
    std::cout << PROGRAM_COPYRIGHT "\n";
    std::cout << "This program is licensed under the Mozilla Public License, v. 2.0.\n";
    return;
}

std::string efxc2Utils::print_usage_arg() {
    std::string ret = "";
    ret = "\n";
    ret += "More information about valid parameters is available at Microsoft's website \n";
    ret += "\n";
    ret += "https://msdn.microsoft.com/en-us/library/windows/desktop/bb509709(v=vs.85).aspx\n";
    return ret;
}

std::string efxc2Utils::print_unsupported_arg_help() {
    std::string ret = "";
    ret += "This isn't a sign of disaster, odds are it will be very easy to add support for\n";
    ret += "this argument.  Review the meaning of the argument in the real fxc program, and\n";
    ret += "then add it into efxc2.\n";
    ret += "\n";
    ret += print_usage_arg();
    return ret;
}

void efxc2Utils::print_version() { 
    std::cout << PROGRAM_DESCRIPTION " version "  PROGRAM_VERSION "\n";
    std::cout << PROGRAM_COPYRIGHT "\n";
    return;
}

void efxc2Utils::print_no_input_file() {  
    std::cerr << "You specified no input files.\n";
    std::cout << print_usage_arg();
    return;
}

void efxc2Utils::print_usage_toomany() {  
    std::cerr << "You specified multiple input files.\n";
    std::cout << "We did not expect to receive this, and aren't prepared to handle multiple input\n";
    std::cout << "files. You'll have to edit the source to behave the way you want.\n";
    std::cout << "\n";
    std::cout << print_usage_arg();
    return;
}

void efxc2Utils::print_errno_value(errno_t err) {
#ifdef _WIN32    
    std::wstring errmsg;
    errmsg.resize(ERRNO_BUFF_SIZE);
    if (_wcserror_s(errmsg.data(), ERRNO_BUFF_SIZE, err) == 0) {
        std::wcout << errmsg.c_str() << std::endl;
    }
#else
    std::string errmsg;
    errmsg.resize(ERRNO_BUFF_SIZE);
    if (strerror_s(errmsg.data(), ERRNO_BUFF_SIZE, err) = 0) {
        std::cout << errmsg.c_str() << std::endl;
    }
#endif
    return;
}

void efxc2Utils::print_hresult_error(const HRESULT hr) {  
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), std::bit_cast<char*>(&messageBuffer), 0, nullptr);
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(suppress : 6387)
#endif
    std::string message(messageBuffer, size);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    (void) LocalFree(messageBuffer);
    std::cerr << std::format("Windows Error Message: {}\n", message);
    std::cout << std::format("Windows Error Message: {}\n", message);
    std::cout << std::format("Error Code: {:#08x}\n", hr);
    std::cout << std::format("Error Name: {}\n", HResultName(hr));
    return;
}

void efxc2Utils::print_windows_error() {  
    /*from: https://gist.github.com/Aaronontheweb/7461004#file-getlasterror-cpp */
    DWORD dLastError = GetLastError();
    LPCTSTR strErrorMessage = nullptr;

    (void) FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        nullptr,
        dLastError,
        0,
        std::bit_cast<wchar_t*>(&strErrorMessage),
        0,
        nullptr);

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4477)
#endif /* _MSC_VER */
    std::wcerr << std::format(L"Windows error: {}\n", strErrorMessage);
#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */
    return;
}

void efxc2Utils::print_help_screen() { 
    print_copyright();
    std::cout << print_usage_arg();
    return;
}

 void efxc2Utils::WriteByteArrayConst(_In_ std::ofstream& f, ID3DBlob* data,
    _In_ const std::string_view variableName,
    _In_ const int outputHex) {
    auto* outString = std::bit_cast<unsigned char*>(data->GetBufferPointer());
    size_t len = data->GetBufferSize();
    f << std::format("const BYTE {}[] =\n{{\n", variableName);
    for (size_t i = 0; i < len; i++) {
        if (outputHex) {
            f << std::format(" 0x{:>02x}", outString[i]); 
        }
        else {
            f << std::format("{:>4}", outString[i]); 
        }
        if (i != len - 1) {
            f << ",";
        }
        if ((i % 6 == 5) && (i != len - 1)) {
            f << "\n";
        }
    }
    f << "\n};\n";
    return;
}

 bool efxc2Utils::readAll(_In_ std::ifstream& in, 
     _Out_ efxc2Utils::M_BUFFER& dataptr) {
     dataptr = std::make_shared<std::vector<char>>();
     auto temp = std::make_unique<std::vector<char>>();
     size_t used = 0;
     size_t n = 0;
     if (dataptr == nullptr) {
         return false; 
     }
     while (TRUE) {
         temp->resize(READALL_CHUNK);
         (void) in.read(temp->data(), READALL_CHUNK);
         n = in.gcount();
         if (n == 0) {
             break;
         }
         used += n;
         temp->resize(n);
         (void)dataptr->insert(dataptr->end(), temp->begin(), temp->end());
     }
     return true;
 }

bool efxc2Utils::parseOpt(_In_ const M_STRING_VIEW option, _In_ const M_CMD_PARAMS& args, _Inout_ size_t* index, _Inout_opt_ M_STRING* argumentOption) { 
    if (!index || *index >= args.size()) {
        return false; 
    }

    M_STRING_VIEW argument = args[*index];
    size_t arg_idx = 0;
    if (argument[0] == '-' || argument[0] == '/') {
        arg_idx++;
        if (argument[arg_idx] == '-') {
            arg_idx++;
        }
    }
    else {
        return false; 
    }

    if (argument.compare(arg_idx, option.size(), option) != 0) {
        return false;
    }

    if (argumentOption) {
        arg_idx += option.size();
        if (arg_idx >= argument.size()) {
            *index += 1;
            if (*index >= args.size() ) {
#ifdef _WIN32
                std::wcerr << std::format(L"Error: missing required argument for option {}\n", option);
#else
                std::cerr << std::format("Error: missing required argument for option {}\n", option);
#endif
                return false;
            }
            *argumentOption = args[*index];
        }
        else {
            *argumentOption = argument.substr(arg_idx, argument.size());
        }
    }
    *index += 1;
    return true;
}

std::string efxc2Utils::setupVariableName(_In_ const std::string_view model,
    _In_ const std::string& entryPoint) {
    std::string variableName = "";
    std::string prefix = "g";
    for (int i = 0; i < PROFILE_PREFIX_TABLE_LEN; i++) {
        if (model.compare(g_profilePrefixTable[i].name) == 0) {
            prefix = g_profilePrefixTable[i].prefix;
            break;
        }
    }
    variableName = prefix + '_' + entryPoint;
    return variableName;
}

#ifdef _WIN32
void efxc2Utils::FixupFileName(_Inout_ std::wstring& FileName) { 
    if (FileName.empty()) {
        return; 
    }
    for (size_t i = 0; FileName[i] != '\0'; i++)
    {
        if (FileName[i] == '/') {
            FileName[i] = '\\';
        }
        else {
            continue;
        }
    }
    return;
}

void efxc2Utils::FixupFileName(_Inout_ std::string& FileName) { 
    if (FileName.empty()) {
        return; 
    }
    for (size_t i = 0; FileName[i] != '\0'; i++)
    {
        if (FileName[i] == '/') {
            FileName[i] = '\\';
        }
        else {
            continue;
        }
    }
    return;
}
#endif /* _WIN32 */

#ifdef _WIN32
/* In these routines,locale is freed ONCE, just before returning or exiting the program */
std::string efxc2Utils::wstring_to_utf8(std::wstring const& wstr) 
{
    if (wstr.empty()) {
        return "";
    }
    _locale_t locale = _create_locale(LC_ALL, (const char*)"en_US.UTF-8");  
    size_t nbytes = 0;
    errno_t err = 0;
    err = _wcstombs_s_l(&nbytes, nullptr, 0, wstr.c_str(), wstr.length() + 1, locale);
    if (err != 0) {
        _free_locale(locale);
        std::cerr << "_wcstombs_s_l failed.";
        print_errno_value(err);
        throw efxc2Exception::Win32APIFailure();
    }
    if (nbytes == 0) { 
        _free_locale(locale); //-V586
        return ""; 
    }

    auto str = std::make_unique<std::vector<char>>();
    str->resize(nbytes + 1);
    str->data()[nbytes] = '\0'; 
    /* The locale still is not freed.  */
    err = _wcstombs_s_l(&nbytes, str->data(), str->size(), wstr.c_str(),wstr.length() + 1, locale); //-V774
    if (err != 0) {
        std::cerr << "_wcstombs_s_l failed.";
        print_errno_value(err);
        throw efxc2Exception::Win32APIFailure();
    }
    if (nbytes == 0) {
        _free_locale(locale);  //-V586
        return "";
    }

    _free_locale(locale);   //-V586
    return str->data();
}

std::wstring efxc2Utils::utf8_to_wstring(std::string const& str) 
{
    if (str.empty()) {
        return L"";
    }
    _locale_t locale = _create_locale(LC_ALL, (const char*)"en_US.UTF-8"); 
    size_t nchars = 0;
    errno_t err = 0;
    err = _mbstowcs_s_l(&nchars, nullptr, 0, str.c_str(), str.length() + 1, locale); 
    if (err != 0) {
        _free_locale(locale);
        std::cerr << "_mbstowcs_s_l failed.";
        print_errno_value(err);
        throw efxc2Exception::Win32APIFailure();
    }
    if (nchars == 0) {
        _free_locale(locale); 
        return L""; 
    }
    auto _wstr = std::make_unique<std::vector<char>>();
    _wstr->resize((nchars + 1) * sizeof(wchar_t));
    auto* wstr = std::bit_cast<wchar_t*>(_wstr->data());
    wstr[nchars] = L'\0';

    err = _mbstowcs_s_l(&nchars, wstr, nchars, str.c_str(), str.length() + 1, locale);  //-V774
    _free_locale(locale);   //-V586
    if (err != 0) {
        std::cerr << "_mbstowcs_s_l failed.";
        print_errno_value(err);
        throw efxc2Exception::Win32APIFailure();
    }
    return wstr;
}

#endif