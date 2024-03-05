# Enhanced version of fxc2

https://github.com/JPeterMugaas/efxc2

Release available for download on [GitHub](https://github.com/JPeterMugaas/efxc2/releases)

## Release History

### March 4, 2024
* Added support for `Qstrip_reflect`, `Qstrip_debug`, `Qstrip_priv`, and `Qstrip_rootsignature` through `D3DStripShader` API call.
* Convert from `wcstombs` API call to `wcstombs_s`.
* Annotate source-code with sal.h annotations.
* Overload `print_errno` so that we can pass a value to it.
* Fix compiler warnings from Intel compiler.

### February 21, 2024
* Original release