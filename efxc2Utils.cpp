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
	std::string result = "Unknown Error Name";
	for (int i = 0; i < efxc2Utils::ERROR_TABLE_LENGTH; i++) {
		if (hr == efxc2Utils::g_ErrorTable[i].ErrorCode) {
			result = efxc2Utils::g_ErrorTable[i].ErrorName;
			break;
		}
	}
	return result;
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
	efxc2Console::Console console = efxc2Console::console; console = efxc2Console::console;
	console.std_err_pink();
	console.std_out_pink();
	std::cerr << "You specified no input files.\n";
	std::cout << print_usage_arg();
	console.std_err_reset();
	console.std_out_reset();
	return;
}

void efxc2Utils::print_usage_toomany() {
	efxc2Console::Console console = efxc2Console::console; console = efxc2Console::console;
	console.std_err_pink();
	console.std_out_pink();
	std::cerr << "You specified multiple input files.\n";
	std::cout << "We did not expect to receive this, and aren't prepared to handle multiple input\n";
	std::cout << "files. You'll have to edit the source to behave the way you want.\n";
	std::cout << "\n";
	std::cout << print_usage_arg();
	console.std_err_reset();
	console.std_out_reset();
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
	std::cout << strerror(err) << std::endl;
#endif
	return;
}

void efxc2Utils::print_hresult_error(const HRESULT hr) {
	efxc2Console::Console console = efxc2Console::console;
	console.std_err_pink();
	console.std_out_pink();
	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), M_BIT_CAST<char*>(&messageBuffer), 0, nullptr);
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(suppress : 6387)
#endif
	std::string message(messageBuffer, size);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
	(void)LocalFree(messageBuffer);
	std::cerr << M_FORMAT("Windows Error Message: {}\n", message);
	std::cout << M_FORMAT("Windows Error Message: {}\n", message);
	std::cout << M_FORMAT("Error Code: {:#08x}\n", hr);
	std::cout << M_FORMAT("Error Name: {}\n", HResultName(hr));
	console.std_err_reset();
	console.std_out_reset();
	return;
}

void efxc2Utils::print_windows_error() {
	/*from: https://gist.github.com/Aaronontheweb/7461004#file-getlasterror-cpp */
	DWORD dLastError = GetLastError();
	efxc2Console::Console console = efxc2Console::console;
	console.std_err_pink();
	console.std_out_pink();
	LPCTSTR strErrorMessage = nullptr;

	(void)FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr,
		dLastError,
		0,
		M_BIT_CAST<wchar_t*>(&strErrorMessage),
		0,
		nullptr);

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4477)
#endif /* _MSC_VER */
	std::wcerr << M_FORMAT(L"Windows error: {}\n", strErrorMessage);
#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */
	console.std_err_reset();
	console.std_out_reset();
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
	size_t len = data->GetBufferSize();
	f << M_FORMAT("const BYTE {}[] =\n{{\n", variableName);
	auto p = M_BIT_CAST<unsigned char*>(data->GetBufferPointer());
	for (size_t i = 0; i < len; i++) {
		if (outputHex) {
			f << M_FORMAT(" 0x{:02x}", *p);
		}
		else {
			f << M_FORMAT("{:>4}", *p);  
		}
		if (i != len - 1) {
			f << ",";
		}
		if ((i % 6 == 5) && (i != len - 1)) {
			f << "\n";
		}
		++p;
	}
	f << "\n};\n";
	return;
}

bool efxc2Utils::readAll(_In_ std::ifstream& in,
	_Out_ efxc2Utils::M_BUFFER& dataptr) {
	dataptr = std::make_shared<std::vector<char>>();
	auto temp = std::make_unique<std::vector<char>>();
	size_t n = 0;
	bool result = false;
	if (dataptr != nullptr) {
		while (TRUE) {
			temp->resize(READALL_CHUNK);
			(void)in.read(temp->data(), READALL_CHUNK);
			n = in.gcount();
			if (n == 0) {
				break;
			}
			temp->resize(n);
			(void)dataptr->insert(dataptr->end(), temp->begin(), temp->end());
		}
		result = true;
	}
	return result;
}

void efxc2Utils::checkForMissingArg(M_STRING_VIEW option, _In_ size_t index, const M_CMD_PARAMS& args) {
	if (index >= args.size()) {
		efxc2Console::Console console = efxc2Console::console; console = efxc2Console::console;
		console.std_err_pink();
#ifdef _WIN32
		std::wcerr << M_FORMAT(L"Error: missing required argument for option {}\n", option);
#else
		std::cerr << M_FORMAT("Error: missing required argument for option {}\n", option);
#endif
		console.std_err_reset();
		throw efxc2Exception::MissingArgument();
	}
}

bool efxc2Utils::parseOpt(_In_ const M_STRING_VIEW option, _In_ const M_CMD_PARAMS& args, _Inout_ size_t* index, _Inout_opt_ M_STRING* argumentOption) {
	bool terminate_routine = false;
	bool result = false;
	if (index == nullptr || *index >= args.size()) {
		terminate_routine = true;
	}
	M_STRING_VIEW argument;
	size_t arg_idx = 0;
	if (terminate_routine == false) {
		argument = args[*index]; //-V1004 // This is a false-positive since we check for a nullptr above.
		arg_idx = 0;
		if (argument[0] == '-' || argument[0] == '/') {
			arg_idx++;
			if (argument[arg_idx] == '-') {
				arg_idx++;
			}
		}
		else {
			terminate_routine = true;
		}
	}

	if (terminate_routine == false) {
		if (argument.compare(arg_idx, option.size(), option) != 0) {
			terminate_routine = true;
		}
		else {
			result = true;
		}
	}

	if (terminate_routine == false) {
		if (argumentOption) {
			arg_idx += option.size();
			if (arg_idx >= argument.size()) {
				*index += 1;
				checkForMissingArg(option, *index, args);
				*argumentOption = args[*index];
			}
			else {
				*argumentOption = argument.substr(arg_idx, argument.size());
			}
		}
		*index += 1;
		result = true;
	}
	return result;
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
	if (FileName.empty() == false) {
		for (size_t i = 0; FileName[i] != '\0'; i++)
		{
			if (FileName[i] == '/') {
				FileName[i] = '\\';
			}
			else {
				continue;
			}
		}
	}
	return;
}

void efxc2Utils::FixupFileName(_Inout_ std::string& FileName) {
	if (FileName.empty() == false) {
		for (size_t i = 0; FileName[i] != '\0'; i++)
		{
			if (FileName[i] == '/') {
				FileName[i] = '\\';
			}
			else {
				continue;
			}
		}
	}
	return;
}
#endif /* _WIN32 */

#ifdef _WIN32
/* In these routines,locale is freed ONCE, just before returning or exiting the program */
std::string efxc2Utils::wstring_to_utf8(std::wstring const& wstr)
{
	bool terminate_function = false;
	_locale_t locale = _create_locale(LC_ALL, utf8_locale.c_str());
	size_t nbytes = 0;
	errno_t err = 0;
	if (wstr.empty()) {
		terminate_function = true;
	}
	if (terminate_function == false) {
		err = _wcstombs_s_l(&nbytes, nullptr, 0, wstr.c_str(), wstr.length() + 1, locale); 
		if (err != 0) {
			_free_locale(locale);
			efxc2Console::Console console = efxc2Console::console; console = efxc2Console::console;
			console.std_err_pink();
			console.std_out_pink();
			std::cerr << "_wcstombs_s_l failed.";
			print_errno_value(err);
			console.std_err_reset();
			console.std_out_reset();
			throw efxc2Exception::Win32APIFailure();
		}
		if (nbytes == 0) {
			terminate_function = true;
		}
	}
	auto str = std::make_unique<std::vector<char>>();
	if (terminate_function == false) {
		str->resize(nbytes + 1);
		str->data()[nbytes] = '\0'; //-V2563 //-V3539
		/* The locale still is not freed.  */
		err = _wcstombs_s_l(&nbytes, str->data(), str->size(), wstr.c_str(), wstr.length() + 1, locale); 
		if (err != 0) {
			efxc2Console::Console console = efxc2Console::console; console = efxc2Console::console;
			console.std_err_pink();
			console.std_out_pink();
			std::cerr << "_wcstombs_s_l failed.";
			print_errno_value(err);
			console.std_err_reset();
			console.std_out_reset();
			throw efxc2Exception::Win32APIFailure();
		}
		if (nbytes == 0) {
			terminate_function = true;
		}
	}
	_free_locale(locale);  
	std::string result = "";
	if (terminate_function == false) {
		result = str->data();
	}

	return result;
}

std::wstring efxc2Utils::utf8_to_wstring(std::string const& str)
{
	bool terminate_function = false;
	_locale_t locale = _create_locale(LC_ALL, utf8_locale.c_str());
	size_t nchars = 0;
	errno_t err = 0;
	if (str.empty()) {
		terminate_function = true;
	}
	if (terminate_function == false) {
		err = _mbstowcs_s_l(&nchars, nullptr, 0, str.c_str(), str.length() + 1, locale); 
		if (err != 0) {
			_free_locale(locale);
			std::cerr << "_mbstowcs_s_l failed.";
			print_errno_value(err);
			throw efxc2Exception::Win32APIFailure();
		}
		if (nchars == 0) {
			terminate_function = true;
		}
	}
	auto _wstr = std::make_unique<std::vector<char>>();
	_wstr->resize((nchars + 1) * sizeof(wchar_t));
	auto* wstr = M_BIT_CAST<wchar_t*>(_wstr->data());
	wstr[nchars] = L'\0';   //-V3539 //-V2563
	if (terminate_function == false) {
		err = _mbstowcs_s_l(&nchars, wstr, nchars, str.c_str(), str.length() + 1, locale); 
		if (err != 0) {
			std::cerr << "_mbstowcs_s_l failed.";
			print_errno_value(err);
			_free_locale(locale);   
			throw efxc2Exception::Win32APIFailure();
		}
	}
	_free_locale(locale);  
	std::wstring result = L"";
	if (terminate_function == false) {
		result = wstr;
	}
	return result;
}

#endif