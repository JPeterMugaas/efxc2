# Enhanced version of fxc2

https://github.com/JPeterMugaas/efxc2

Release available for download on [GitHub](https://github.com/JPeterMugaas/efxc2/releases)

## Release History
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