efxc2 is an enhanced version of fxc2.  THe original fxc2 program 
is A wine-runnable version of Microsofts Shader Compiler fxc and 
is available at https://github.com/mozilla/fxc2 .  Unfortunately,
fxc2 does not appear to be maintianed and I have encountered
quite a few issues using that program. I needed a fxc compiler that
was open-source and would adiquately fullfill my needs.  So I decided
to make my own modifications to the fxc2 source-code into my own enhnaced
program (efxc2).

This program works by using the d3dcompiler_47.dll available included
in Windows using the API as desribed by:

https://learn.microsoft.com/en-us/windows/win32/api/d3dcompiler/ .

Usage:

This program aims to emulate the syntax used by Microsoft's Shader Compiler (fxc). 
That program is documented at:

https://learn.microsoft.com/en-us/windows/win32/direct3dtools/fxc

License

This program is licensed under the Mozilla Public License, v. 2.0.  A copy
of that license is included in the license_MPL_2_0.txt file.