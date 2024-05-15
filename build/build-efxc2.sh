#!/bin/bash
rm -rf src pkg
MINGW_ARCH=mingw64 makepkg-mingw -f
rm -rf *.zip
MINGW_ARCH=mingw32 makepkg-mingw -f
rm -rf *.zip
MINGW_ARCH=ucrt64 makepkg-mingw -f
rm -rf *.zip
MINGW_ARCH=clang64 makepkg-mingw -f
MINGW_ARCH=clang32 makepkg-mingw -f
cd msys
rm -rf src pkg
makepkg -f
cd ..
cd ..
rm -rf src
rm -rf pkg
