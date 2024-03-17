# Enhanced version of fxc2

https://github.com/JPeterMugaas/efxc2

Release available for download on [GitHub](https://github.com/JPeterMugaas/efxc2/releases)

## Release History
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