# Enhanced version of fxc2

https://github.com/JPeterMugaas/efxc2

Release available for download on [GitHub](https://github.com/JPeterMugaas/efxc2/releases)

## Release History
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