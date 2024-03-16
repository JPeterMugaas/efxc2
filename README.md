# efxc2

efxc2 is an enhanced version of fxc2.  The original fxc2 program 
is A wine-runnable version of Microsofts Shader Compiler fxc and 
is available at https://github.com/mozilla/fxc2 .  Unfortunately,
fxc2 does not appear to be maintained and I have encountered
quite a few issues using that program. I needed a fxc compiler that
was open-source and would adequately fulfill my needs.  So I decided
to make my own modifications to the fxc2 source-code including a pull 
request ( https://github.com/mozilla/fxc2/pull/3 ) into my own enhanced
program (efxc2).

This program works by using the d3dcompiler_47.dll included in Windows 
using the API as described by:

https://learn.microsoft.com/en-us/windows/win32/api/d3dcompiler/

## Usage:

This program aims to be compatible with the syntax used by Microsoft's 
Shader Compiler (fxc).  That program is documented at:

https://learn.microsoft.com/en-us/windows/win32/direct3dtools/fxc

## Compiling this program

### Requirements

A properly installed compiler such as:

* [Clang](http://www.clang.org)
* [GCC](https://gcc.gnu.org/)
* [Intel� oneAPI DPC++/C++ Compiler](https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.6foy5x)
* [Visual Studio](https://visualstudio.microsoft.com/)

as well as [CMake](https://cmake.org/).

### [MSYS2](https://www.msys2.org/) Environment

Ddo the following:

1. install [CMake](https://cmake.org/) using `pacman -S [package name]`.  
You can find what versions are available with the command `pacman -Ss cmake`.
2. [Download the PKGBUILD](https://raw.githubusercontent.com/JPeterMugaas/MINGW-ext-packages/master/mingw-w64-efxc2/PKGBUILD) file and place it in an  empty directory named `mingw-w64-efxc2`.
3. change the current directory to where you downloaded the PKGBUILD and 
enter `makepkg-mingw` to build efxc2.
4. install the packages you created using `pacman -U [package name]`. 

### Other Environments

To compile this program:
1. make a directory where you wish to build the program.  This should be 
separate from the directory where you extracted the source-code.
2. change the current directory to this build directory.
3. enter commands such as `cmake.exe -GNinja -DCMAKE_INSTALL_PREFIX=[installation path] [source-directory]` 
and `cmake.exe --build .`.
4. install the program to desired path you specified in the 
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