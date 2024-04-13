# efxc2

efxc2 is an enhanced version of fxc2.  The original fxc2 program 
is A wine-runnable version of Microsofts Shader Compiler fxc and 
is available at [https://github.com/mozilla/fxc2](https://github.com/mozilla/fxc2).  Unfortunately,
fxc2 does not appear to be maintained and I have encountered
quite a few issues using that program. I needed a fxc compiler that
was open-source and would adequately fulfill my needs.  So I decided
to make my own modifications to the fxc2 source-code including a pull 
request, [https://github.com/mozilla/fxc2/pull/3](https://github.com/mozilla/fxc2/pull/3) into my own enhanced
program (efxc2).

This program works by using the d3dcompiler_47.dll included in Windows 
using the API as described by:

[https://learn.microsoft.com/en-us/windows/win32/api/d3dcompiler/](https://learn.microsoft.com/en-us/windows/win32/api/d3dcompiler/)

## Usage:

This program aims to be compatible with the syntax used by Microsoft's 
*Effect-Compiler Tool* (fxc).  That program is documented at:

[https://learn.microsoft.com/en-us/windows/win32/direct3dtools/fxc](https://learn.microsoft.com/en-us/windows/win32/direct3dtools/fxc)

## Installation

### Requirements

A properly installed compiler that supports [C++20](https://isocpp.org/std/the-standard) such as:

* [Clang](http://www.clang.org)
* [GCC](https://gcc.gnu.org/)
* [Intel® oneAPI DPC++/C++ Compiler](https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.6foy5x)
* [Visual Studio](https://visualstudio.microsoft.com/)

as well as [CMake](https://cmake.org/).

### [Mingw-w64](https://www.mingw-w64.org/) under [MSYS2](https://www.msys2.org/)

MSYS2 now includes efxc2 as a mingw-64 package so you can install it with pacman with the command  `pacman -S [package name]`.  You 
can search for the appropriate "efxc2" package with the command `pacman -Ss efxc2`.

If you prefer to compile it for yourself, do the following:

1. Install [CMake](https://cmake.org/) using `pacman -S [package name]`.  
You can find what versions are available with the command `pacman -Ss cmake`.
2. [Download the PKGBUILD](https://raw.githubusercontent.com/JPeterMugaas/MINGW-ext-packages/master/mingw-w64-efxc2/PKGBUILD) file and place it in an  empty directory named `mingw-w64-efxc2`.
3. Change the current directory to where you downloaded the PKGBUILD and 
enter `makepkg-mingw` to build efxc2.
4. Install the packages you created using `pacman -U [package name]`. 

### [MSYS2](https://www.msys2.org/) Environment

1. Install [CMake](https://cmake.org/) using `pacman -S cmake`. 
2. [Download the PKGBUILD](https://raw.githubusercontent.com/JPeterMugaas/MSYS2-ext-packages/master/efxc2/PKGBUILD) file and place it in an  empty directory named `efxc2`.
2. Change the current directory to where you downloaded the PKGBUILD and 
enter `makepkg` to build efxc2.
4. Install the packages you created using `pacman -U efxc2`. 

### Other Environments

To compile this program:
1. Make a directory where you wish to build the program.  This should be 
separate from the directory where you extracted the source-code.
2. Change the current directory to this build directory.
3. Enter commands such as `cmake.exe -GNinja -DCMAKE_INSTALL_PREFIX=[installation path] [source-directory]` 
and `cmake.exe --build .`.
4. Install the program to desired path you specified in the 
CMAKE_INSTALL_PREFIX with a command such as `cmake --install`.  
Alternatively, you can simply copy the *.exe from the "Release" or "Debug" 
directory to the location you desire.*

An example I use on the *Intel oneAPI command prompt* is this:

    cmake ../ -G"Visual Studio 17 2022"
    cmake --build .

where I am building from a subdirectory under the source-code.

## License

This program is licensed under the Mozilla Public License, v. 2.0.  A copy
of that license is included in the license_MPL_2_0.txt file.