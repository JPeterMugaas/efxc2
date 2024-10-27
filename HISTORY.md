# Enhanced version of fxc2

https://github.com/JPeterMugaas/efxc2

Release available for download on [GitHub](https://github.com/JPeterMugaas/efxc2/releases)

## Release History
### Oct 27, 2024
* Merge Pull Request #1 - cmake: add option to use vendored fmt on command
### July 12, 2024
* Remove duplicate code that sets up an array for the compiler includes.
* Make 4 consecutive inc statements into a for loop.
* Remove some more duplicate code. Thanks to PVS-Studio for pointing out the new procedure needs to be in a namespace.
* Fix typo in program and don't assume file-extension when linking.
* New variable - EFXC2_FMT_PATH - specify path for FMT. This path works like this EFXC2_FMT_PAT/lib, EFXC2_FMT_PATH/lib/cmake, and EFXC2_FMT_PATH/include.
* Make this program more configurable for compilers with incomplete Standard C++20 library support.
* Simplify .rc building.
* Re-factored compiling/preprocessing error handling to remove some duplicate code.
* added void efxc2Compiler::print_compiler_params_begin and void efxc2Compiler::print_compiler_params_end to refactor some debug output.
* Fix PVS Studio warning about an unused return value.
* Add note about the potential conflict that I avoid by naming a macro M_E_ instead of M_E.
* Make main and wmain use argv as a const parameter.
* Remove unnecessary console assignments.
* Only use bit header if we have BIT_CAST_SUPPORTED is enabled.
* Remove unused method from CompilerAPIContainer.
* Only use ranges header if RANGS_SUPPORTED is enabled.

### Jun 26, 2024
* Make efxc2 depend upon the [\{fmt\} library](https://fmt.dev/) if [std::format](https://en.cppreference.com/w/cpp/utility/format/format) 
is not supported.  This should allow some slightly older compilers to build efxc2.  They just have to support other stuff in the C++ Standard.
* Make associated adjustments in the source-code and CMakeLists.txt file.

### Jun 12, 2024
* Specifically check for C\+\+20 formatting library. Some compilers do not include that in their C++20 support.
* Change back to typedef's from using because the "using" syntax is not portable.
* efxc2Exception.cpp and efxc2Console.cpp - guard in case "#pragma once" does not work.
* efxc2Utils.cpp - remove an unused variable in "readall"
* efxc2Console - make Shutdown a const function.
* Clarify what we require with C++20 support in the README.md file.
* Fix README.md grammatical error.
* Remove an empty IFDEF.
* Make API function names char const and pass them to GetProcAddress as well as the fail code.
* Fix a few things that PVS-Studio found.  Feilds can be reordered to save memory.  Having an equals sign mid expression can cause confusion.
* convert some size_t conversions to static_cast Thanks to PVS-Studio for pointing out the issue.
* Fix hexidecimal 0-padding. 
* Fixed PVS-Studio warnings V3539 and 2563 concerning arithmetic in pointer operations.
* Simplify Compiler::GetPDBFileName. Thanks to PVS-Studio for pointing out the issue with it.

### May 21, 2024
* Add instructions for installing efxc2 in Cygwin.
* EFXC2_BUILD_STATIC now works for MSYS2 and Cygwin.
* Clarify program build requirements.
* added version requirement for Intel C++
* PKGBUILD used for Internal testing with mingw64 and clang64, run PVS-Studio to report on the code, run the cmake tests.
and Make a precompiled binary build for distribution.
* added version requirement for Intel C++
* EFXC2_BUILD_STATIC now works for MSYS2 and Cygwin.
* Add instructions for installing efxc2 in Cygwin.
* efxc2Files.cpp - typecast the result of open to void. Thanks to PVS-Studio.
* efxc2Utils.cpp disable warnings generated when using PVS-Studio with a 32 bit compiler.
* Fix issue with unused variable and with a generic exception being raised. Thanks to SonarCloud.
* Fix various warnings from PVS-Studio including missing returns.
* Start on making functions that have only one "return" at the end. Add PVS-Studio warning suppression for a constant defined without an integer suffix in the Win32 API.
* Fix various warnings from PVS-Studio including missing returns.
* got rid of all "exit" calls and replaced those with exception handling.  Thanks to PVS-Studio for pointing out the issue with "exit".
* The UTF8 conversion routines will now print what errno reports.
* add CMakeSettings.json.
* ID3D11Linker is NOW already defined in MSYS2.
* add CMakeSettings.json Visual Studio file to repository.
* Add more settings to CMakeSettings.json.
* Use "main" as default entry point.
* Put console in ENABLE_VIRTUAL_TERMINAL_INPUT mode. 
* Remove a duplicate code pointed out by SonarLint.
* print most error info in a pink color.
* Convert API definitions to "using" from "typedef".
* Suppress PVS-Studio warning "V802" since we have no intention of serializing the structure to disk.
* Move installation instructions to their own file to shorten the READM.md.
* Loading .DLL failure will now raise an exception.
* Add automation to the build process.

### May 10, 2024
* Add debug option for very verbose output and make sure that it is honored throughout the source-code.  New default:  Without debug being specified, only minimal output is given.  This is in contrast to /nologo where no output is given.
* Fix for handling -debug parameter. Include tests now output an object (-Fo).
* Fix clang warning "codecvt_utf8<wchar_t>' is deprecated" by replacing it with the old code that uses "MultiByteToWideChar" and "WideCharToMultiByte". See: https://en.cppreference.com/w/cpp/header/codecvt .
* Add a test for /P parameter used with /Fo. This is not allowed and the test is expected to fail.
* Full /P parameter support for preprocessing .HLSL files.
* Replace some for loops with std::ranges::find.
* Refactor nologo and debug checks out of wmain.
* Some sample shaders that I found at [Windows-universal-samples](https://github.com/microsoft/Windows-universal-samples/tree/main/Samples/D2DAdvancedColorImages/cpp) and a hlsli from the Windows SDK.
* Fixed issue with "unsupported" parameters. They weren't handled properly.
* Add test specifically for unsupported option, terminate program.
* add precomppiled shaders and the batch file I used to create them using the original fxc program.
* Convert DisplayDefines into lambdas.
* Convert a couple of for loops into for_each with lambdas and remove an unused variable.
* Changed open and close overrides to match what is in the d3dcommon.h header.
* Fix for various issues found by [PVS-Studio](https://pvs-studio.com/).
* Fixed Access Violation if no input file was specified.
* efxc2CompilerIncludes.h - make verbose and debug bools
* CMakeLists.txt - add test for preprocess requests with no input filename since that's separate code.
* Remove an old macro that referenced wcsdup function. We do not use that directly any more.
* add a comment about where some source-code is based on for later reference.
* /setprivate now works.
* eliminated pShaderWithNewName variable.  Results now go into CompilerOutput variable so that the write procedures should work with it.
* Input streams need to be read with the std::ios::binary flag. Otherwise, an implementation might change the EOL and throw things off.
* Add option to check the code with [PVS-Studio](https://pvs-studio.com/). This is set to OFF since PVS Studio is not required to build the software. It's just something to help me code.
* Suppress some PVS warnings about an issue with "[noreturn]" functions.  I double-checked the code and the issue does not apply.
* Issue detected by [PVS-Studio](https://pvs-studio.com/). The global namespace should only contain 'main', namespace declarations and 'extern "C"' declarations.
* Fix a few errors related to 64-bit types thanks to [PVS-Studio](https://pvs-studio.com/) and suppress some warnings for things I do not think are an issue.
* [PVS-Studio](https://pvs-studio.com/) warned me about where an int was used for potentially 64-bit indexes (an int is 32-bits). Unfortunately, the WideCharToMultiByte and MultiByteToWideChar Win32 API functions take a char count that is only an int. I was using them to convert things from Unicode to UTF8 and back. The fix is to switch to the _wcstombs_s_l and _mbstowcs_s_l specifying a UTF8 locale.
* The readall call can fail. 
* Sometimes, we don't use results of a function so those should be typecast to void.
* Use Win32 API constants in the g_ErrorTable table instead of hard-coding the values.

### April 24, 2024
* Convert some strings and wstrings to std::filesystem::path.
* Get rid of GetFileName and c_pdbFile.
* Fix for .pdb use default filename file path syntax.
* Added comment about SonarQube and fixed where file was being written instead of read.
* Write compiler error message to cout instead of cerr so that I can see it better in log files.
* Add 2 tests and stuff for test cases.
* Include Dir (/I) now supported.
* Refactored for loop for ease of maintenance.
* First, try to load includes from the same dir as the input file name.
* Convert dirs into a std::vector<std::filesystem::path>.
* fixed SonarQUbe warning about mixing private and public class members.
* Make a for loop into a range for loop.  Handle error if include is not opened.  It can happen  because File Size can succeeded if the directory entry is a subdir.
* Convert command-line comparison loops into range loops.

### April 18, 2024
* Fix misspellings in source-code.
* Convert program to C++20 standard.
* Flatten the API for command handlers.
* Command line parameters are now processed completely using std:string or std:wstring.
* char*'s to std::string.
* Noted requirement for a compiler capable of C++20 in README.md.
* Changed some std::string's to a std::string_view's.  std::string_view requires little overhead.
* Print source-code sample when compiling. This is 40 characters in length.
* Remove printf calls.
* Fix formatting to work with the new C++ Standard Library format function instead of printf.
* Convert print_windows_error to use wcerr.
* Converted file write routines to use "ofstream".
* Report file name if we can't open the file for writing.
* Made Files::LoadInputFile a constant procedure.
* Make WriteByteArrayConst take a ID3DBlob parameter instead of a pointer and size.
* CompilerTasks now take params as a const since we don't change that there.
* reduced the number of ifdef's by aliasing std::string and wstring.
* Stop using the low level FILE* API calls in favor of ifstream when reading files.
* rename "readall" to "readAll". 
* Remove some C Standard units that were not being used any more.

### April 10, 2024
* Remove SourceLen from CompilerParams and make print_errno use a dynamic buffer instead of a static one (also use standard types).
* publish version info in MSYS/CYGWIN. Fixed resource-file so that it complies with standards for .EXE's, not .DLL's.
* Move M_COMPILER_DEFINES to efxc2Utils. Eliminate numdefines since it's no longer used.
* Defines functionality now uses smart pointers.
* You can now pass a value to a define like this: `[define name]=[define value]`.  This is described at [Microsoft's website](https://learn.microsoft.com/en-us/windows/win32/direct3dtools/dx-graphics-tools-fxc-syntax).  If 
no value for a define is specified, a default of "1" is used.  The previous behavior was that all defines had a value of "1".
* print_errno use a dynamic buffer instead of a static one and also uses standard type.

### April 8, 2024
* Refactor Source-code to move stuff from Compiler to CompilerParams.
* Made most internals use std::string or std::wstring.
* Remove code that loads the .DLL only from the same directory as the program.
* remove malloc/free from SetPDBFileName and use std::vector for our buffer.
* SourceCode now a shared pointer to a vector of char.
* Rewrote the readall procedure to use this shared pointer to pass the source-code to the CompilerParams object.
* DLL_NAME is now a constexpr
* Removed some overloads from utf8_decode and utf8_encode to simplify the API.
* utf8_decode and utf8_encode use smart pointers instead of malloc/free.
* Set the default c++std flag to Visual Studio 2022's default.  We can change this to a later C++ standard version if needed.
* MSYS, had to switch to strerror from strerror_r. strerror_r was not declared for some reason.

### March 27, 2024
* Convert some arrays to std::array and convert parameter macros to constexpr.
* convert DLL_NAME to constexpr const.
* Replace wcharToChar with utf8_encode
* Add /Fc support.
* Refactored commands that specify files. Make .PDB file generation conditional.
* Add some support for /Cc parameter.
* Support for /Ni and /No parameters.
* Remove `using namespace std;`
* Expose `D3DLoadModule` and `D3DCreateLinker` functions.

### March 17, 2024
* add utf8_decode and utf8_encode functions, SetPDBFileName, and convert pdbFile to a utf8 string.
* .PDB support based on article at: [https://devblogs.microsoft.com/pix/using-automatic-shader-pdb-resolution-in-pix/](https://devblogs.microsoft.com/pix/using-automatic-shader-pdb-resolution-in-pix/)
* Fix minor warnings from [SonarQube](https://www.sonarsource.com/).
* Major refactoring so that the compiler is isolated from the parameters and IO as well as UNICODE. This is done through the compiler object.
* Refactor compiler calls into their own unit and procedure.
* Created "Files" object, refactored CompilerTasks, and Reworked some command handlers to use the Files object.
* Fix "all_resources_bound" flag
* simplify wmain by making calls using a table.
* Refactored Ignored and unsupported option handling to simply wmain/main.
* Remove uncalled returns and fix cmd_D in MSYS2.
* Refactored compilerAPI from the Compiler class. The API is for Windows procedure pointers.

### March 10, 2024
* Moved some routines to efxc2Utils.cpp.
* Refactoring command line handling procedures by moving them into their own unit.
* Fix reference error in Fo parameter.
* refactored code into a Compiler object.
* Made it possible to have both a /Fh and /Fo parameter.
* Work on .PDB support preperation.
* changed NULL to nullptr
* added noreturn attribute to a few functions that do not return.
* Fix local variable shadowing.
* Simplified printing a size_t with zu format arg. 
* Fixed mixed wchar_t and char literals.

### March 4, 2024
* Added support for `Qstrip_reflect`, `Qstrip_debug`, `Qstrip_priv`, and `Qstrip_rootsignature` through `D3DStripShader` API call.
* Convert from `wcstombs` API call to `wcstombs_s`.
* Annotate source-code with sal.h annotations.
* Overload `print_errno` so that we can pass a value to it.
* Fix compiler warnings from Intel compiler.

### February 21, 2024
* Original release