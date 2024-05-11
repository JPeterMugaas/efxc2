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

as well as [CMake](https://cmake.org/).  You will often need a build system for CMake such as:

* [Ninja](https://ninja-build.org/)
* [GNU Make](https://www.gnu.org/software/make/)

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

### [Cygwin](https://www.cygwin.com/)

The Cygwin version of this program requires GNU C++ 13.1 or greater.  Thus, this program requires that you install a test version of GNU GCC from the Cygwin setup utility.

1. To compile this program for Cygwin, make a directory for building the files then enter:
```
cmake -GNinja [source directory]
cmake --build .
```
2. To run tests on this build, enter the following:
```
cmake --build . --target test
```
3. To install this program, enter the following:
```
DESTDIR="[installation prefix of your choice]" cmake --install .
```
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

## Tools I Use to Develop this Program

These tools are **NOT** required to build this program or submit request.  I list them because they are helpful to me in developing this program.

[Microsoft Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/) - 

[PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.

[SonarCloud](https://www.sonarsource.com/products/sonarcloud/) - a cloud-based code review tool that integrates with DevOps platforms and CI/CD workflows.

[SonarLint](https://www.sonarsource.com/products/sonarlint/) - an advanced linter in your IDE for Clean Code