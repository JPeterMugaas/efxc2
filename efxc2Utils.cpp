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

static std::string HResultName(_In_ const HRESULT hr) {
    for (int i = 0; i < ERROR_TABLE_LENGTH; i++) {
        if (hr == g_ErrorTable[i].ErrorCode ) {
            return  g_ErrorTable[i].ErrorName;
        }
    }
    return "Unknown Error Name";
}

void print_copyright() {
    std::cout << PROGRAM_DESCRIPTION " " PROGRAM_VERSION "\n";
    std::cout << PROGRAM_COPYRIGHT "\n";
    std::cout << "This program is licensed under the Mozilla Public License, v. 2.0.\n";
    return;
}

void print_usage_arg() {
    std::cout << "\n";
    std::cout << "More information about valid parameters is available at Microsoft's website \n";
    std::cout << "\n";
    std::cout << "https://msdn.microsoft.com/en-us/library/windows/desktop/bb509709(v=vs.85).aspx\n";
    return;
}

void print_unsupported_arg_help() {
    std::cout << "This isn't a sign of disaster, odds are it will be very easy to add support for\n";
    std::cout << "this argument.  Review the meaning of the argument in the real fxc program, and\n";
    std::cout << "then add it into efxc2.\n";
    std::cout << "\n";
    print_usage_arg();
    return;
}

[[noreturn]] void print_version() {
    std::cout << PROGRAM_DESCRIPTION " version "  PROGRAM_VERSION "\n";
    std::cout << PROGRAM_COPYRIGHT "\n";
    exit(0);
}

[[noreturn]] void print_usage_missing(const char* arg) {
    std::cerr << std::format("efxc2 is missing the {} argument.\n", arg);
    std::cout << "We expected to receive this, and it's likely things will not work correctly\n";
    std::cout << "without it.  Review efxc2 and make sure things will work.\n";
    std::cout << "\n";
    print_usage_arg();
    exit(1);
}

[[noreturn]] void print_usage_toomany() {
    std::cerr << "You specified multiple input files.\n";
    std::cout << "We did not expect to receive this, and aren't prepared to handle multiple input\n";
    std::cout << "files. You'll have to edit the source to behave the way you want.\n";
    std::cout << "\n";
    print_usage_arg();
    exit(1);
}

[[noreturn]] void print_hresult_error(const HRESULT hr) {
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);
    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    std::cerr << std::format("Windows Error Message: {}\n", messageBuffer);
    std::cout << std::format("Windows Error Message : {}\n", messageBuffer);
    std::cout << std::format("Error Code: {:#08x}\n", hr);
    std::cout << std::format("Error Name: {}\n", HResultName(hr));
    exit(1);
}

[[noreturn]] void print_windows_error() {
    /*from: https://gist.github.com/Aaronontheweb/7461004#file-getlasterror-cpp */
    DWORD dLastError = GetLastError();
    LPCTSTR strErrorMessage = nullptr;

    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        nullptr,
        dLastError,
        0,
        (LPWSTR)&strErrorMessage,
        0,
        nullptr);

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4477)
#endif /* _MSC_VER */
#ifdef _WIN32
    fwprintf(stderr, L"Windows error: %ls\n", strErrorMessage);
#else  /* _WIN32 */
    fprintf(stderr, "Windows error: %ls\n", strErrorMessage);
#endif /* _WIN32 */
#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */
    exit(1);
}

[[noreturn]] void print_help_screen() {
    print_copyright();
    print_usage_arg();
    exit(0);
}

[[noreturn]] void print_errno(errno_t _errno) {
#ifdef _WIN32
    auto errmsg = std::make_unique<std::vector<char>>(ERR_SIZE);
    strerror_s(errmsg->data(), ERR_SIZE, _errno);
    fprintf(stderr, "%s\n", errmsg->data());
#else  /* _WIN32 */
    char* errmsg = nullptr;
    errmsg = strerror(_errno);
    std::cerr << std::format("{}\n", errmsg);
#endif /* _WIN32 */
    exit(1);
}

[[noreturn]] void print_errno(void) {
    print_errno(errno);
}

/* from: https://btechgeeks.com/how-to-get-filename-from-a-path-with-or-without-extension-in-cpp/*/
std::string GetFileName(_In_ const std::string& path, _Out_ int* IsSpecialFolder) {
    *IsSpecialFolder = false;
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    if (size_t i = path.rfind(sep, path.length()); i != std::string::npos)
    {
        std::string filename = path.substr(i + 1, path.length() - i);
        std::string rawname = filename.substr(0, path.length());
        if ((rawname.compare("..") == 0) || (rawname.compare(".") == 0)) {
            *IsSpecialFolder = true;
            rawname = "";
        }
        return rawname;
    }
    return "";
}

#ifdef _WIN32
std::wstring GetFileName(_In_ const std::wstring& path, _Out_ int* IsSpecialFolder) {
    *IsSpecialFolder = false;
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    if (size_t i = path.rfind(sep, path.length()); i != std::wstring::npos)
    {
        std::wstring filename = path.substr(i + 1, path.length() - i);
        std::wstring rawname = filename.substr(0, path.length());
        if ((rawname.compare(L"..") == 0) || (rawname.compare(L".") == 0)) {
            *IsSpecialFolder = true;
            rawname = L"";
        }
        return rawname;
    }

    return L"";
}
#endif

void WriteByteArrayConst(_In_ FILE* f, _In_reads_bytes_(len) const unsigned char* outString, 
    _In_ const size_t len, 
    _In_z_ const char* variableName, 
    _In_ const int outputHex) {
    fprintf(f, "const BYTE %s[] =\n{\n", variableName);
    for (size_t i = 0; i < len; i++) {
        if (outputHex) {
            fprintf(f, " 0x%02" PRIx8, outString[i]);
        }
        else {
            fprintf(f, "%4" PRIu8, outString[i]);
        }
        if (i != len - 1) {
            fprintf(f, ",");
        }
        if ((i % 6 == 5) && (i != len - 1)) {
            fprintf(f, "\n");
        }
    }
    fprintf(f, "\n};\n");
}

int readall(_In_ FILE * in,
    _Out_ M_BUFFER& dataptr) {
    dataptr = std::make_shared<std::vector<char>>();
    auto temp = std::make_unique<std::vector<char>>();
    size_t used = 0;
    size_t n;
    if (in == nullptr || dataptr == nullptr) {
        return READALL_INVALID;
    }
    while (TRUE) {
        temp->resize(READALL_CHUNK);
        n = fread(temp->data(), 1, READALL_CHUNK, in);
        if (n == 0) {
            break;
        }
        used += n;
        temp->resize(n);
        dataptr->insert(dataptr->end(), temp->begin(), temp->end());
    }
    if (ferror(in)) {
        return READALL_ERROR;
    }
    return READALL_OK;
}

bool parseOpt(_In_ const M_STRING& option, _In_ const M_CMD_PARAMS& args, _Inout_ size_t* index, _Inout_opt_ M_STRING* argumentOption) {
    if (!index || *index >= args.size()) {
        return false;
    }

    M_STRING argument = args[*index];
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

std::string setupVariableName(_In_ const std::string_view& model,
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
void FixupFileName(_Inout_ std::wstring& FileName) {
    if (FileName.empty()) {
        return;
    }
    for (int i = 0; FileName[i] != '\0'; i++)
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

void FixupFileName(_Inout_ std::string& FileName) {
    if (FileName.empty()) {
        return;
    }
    for (int i = 0; FileName[i] != '\0'; i++)
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
/*These functions are from:
  https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte
*/

std::wstring utf8_decode(const std::string& str) {
    int nchars = 0;
    if ((nchars = MultiByteToWideChar(CP_UTF8,
        MB_ERR_INVALID_CHARS, str.c_str(), (int)str.length(), nullptr, 0)) == 0) {
        return L"";
    }

    auto _wstr = std::make_unique<std::vector<char>>();
    _wstr->resize(((size_t)nchars + 1) * sizeof(wchar_t));
    auto* wstr = std::bit_cast<wchar_t*>(_wstr->data());
    wstr[nchars] = L'\0';
    
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
        str.c_str(), (int)str.length(), wstr, nchars) == 0) {
        return L"";
    }
    return wstr;
}

std::string utf8_encode(const std::wstring& wstr) {
    int nbytes = 0;
    if ((nbytes = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
        wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr)) == 0) {
        return "";
    }

    auto str = std::make_unique<std::vector<char>>();
    str->resize(nbytes + 1);
    str->data()[nbytes] = '\0';

    if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
        wstr.c_str(), (int)wstr.length(), str->data(), nbytes, nullptr, nullptr) == 0) {
        return "";
    }
    return str->data();
}

#endif