# Building and Installing This Program

## Requirements

A properly installed compiler that supports [C++20](https://isocpp.org/std/the-standard) including [Text Formatting](https://en.cppreference.com/w/cpp/utility/format/format) such as:

* [Clang](http://www.clang.org) 17.0.0 or greater
* [GCC](https://gcc.gnu.org/) 13.1.0 or greater
* [Intel® oneAPI DPC++/C++ Compiler](https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.6foy5x) 2022.2.1 or greater
* [Visual Studio](https://visualstudio.microsoft.com/) 2019 version 16.11.2 or greater

as well as [CMake](https://cmake.org/).  You will often need a build system for CMake such as:

* [Ninja](https://ninja-build.org/)
* [GNU Make](https://www.gnu.org/software/make/)

## [Mingw-w64](https://www.mingw-w64.org/) under [MSYS2](https://www.msys2.org/)

MSYS2 now includes efxc2 as a mingw-64 package so you can install it with pacman with the command  `pacman -S [package name]`.  You 
can search for the appropriate "efxc2" package with the command `pacman -Ss efxc2`.

If you prefer to compile it for yourself, do the following:

1. Install [CMake](https://cmake.org/) using `pacman -S [package name]`.  
You can find what versions are available with the command `pacman -Ss cmake`.
2. [Download the PKGBUILD](https://raw.githubusercontent.com/JPeterMugaas/MINGW-ext-packages/master/mingw-w64-efxc2/PKGBUILD) file and place it in an  empty directory named `mingw-w64-efxc2`.
3. Change the current directory to where you downloaded the PKGBUILD and 
enter `makepkg-mingw` to build efxc2.
4. Install the packages you created using `pacman -U [package name]`. 

## [MSYS2](https://www.msys2.org/) Environment

1. Install [CMake](https://cmake.org/) using `pacman -S cmake`. 
2. [Download the PKGBUILD](https://raw.githubusercontent.com/JPeterMugaas/MSYS2-ext-packages/master/efxc2/PKGBUILD) file and place it in an  empty directory named `efxc2`.
2. Change the current directory to where you downloaded the PKGBUILD and 
enter `makepkg` to build efxc2.
4. Install the packages you created using `pacman -U efxc2`. 

## [Cygwin](https://www.cygwin.com/)

TUnfortunately, the Cygwin release version of GCC is 11.4.0 so you need to install a test version of GNU GCC from the Cygwin setup utility.

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
## Other Environments

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
