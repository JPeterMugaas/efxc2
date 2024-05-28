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

## Building and Installing This Program

See: [INSTALL.md](INSTALL.md)

## License

This program is licensed under the Mozilla Public License, v. 2.0.  A copy
of that license is included in the license_MPL_2_0.txt file.

## Tools I Use to Develop this Program

These tools are **NOT** required to build this program or submit pull requests.  I list them because they are helpful to me in developing this program.

[Microsoft Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/)

[PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.

[SonarCloud](https://www.sonarsource.com/products/sonarcloud/) - a cloud-based code review tool that integrates with DevOps platforms and CI/CD workflows.

[SonarLint](https://www.sonarsource.com/products/sonarlint/) - an advanced linter in your IDE for Clean Code
