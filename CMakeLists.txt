#--------------------------------------------------------------------------------------
# File: CMakeLists.txt
#
# Copyright (c) J. Peter Mugaas
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#--------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.21)

set(EFXC2_VERSION 0.0.11.234)
project (efxc2 VERSION ${EFXC2_VERSION}
  DESCRIPTION "Enhanced fxc2"
  HOMEPAGE_URL "https://github.com/JPeterMugaas/efxc2"
  LANGUAGES CXX)

include(GNUInstallDirs)

set(PROJECT_COMPANY "J. Peter Mugaas")
set(PROJECT_COPYRIGHT "Copyright (c) 2024 ${PROJECT_COMPANY}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "Check for C++20 formatting library")
file(WRITE
  ${CMAKE_BINARY_DIR}/CMakeTmp/testCCompiler.cpp
  "#include <iostream>\r\n"
  "#include <format>\r\n"
  "\r\n"
  "int main()\r\n"
  "{\r\n"
  "    std::cout << std::format(\"Hello {}\", \"world\");\r\n"
  "}")
try_compile(FORMAT_SUPPORTED
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/CMakeTmp/testCCompiler.cpp
  OUTPUT_VARIABLE OUTPUT)
message(STATUS "Check for C++20 formatting library  - done")
if (NOT FORMAT_SUPPORTED)
    message(FATAL_ERROR "C++20 formatting not available.")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(EFXC2_BUILD_STATIC "Build with static libraries." ON)
option(EFXC2_USE_PVS_STUDIO "Enable analysis with PVS-Studio" OFF)
option(EFXC2_USE_ALL_RULES "Use all rules with PVS-Editor" OFF)

set(EXEC_SOURCES efxc2.cpp
                 efxc2Cmds.h
                 efxc2Cmds.cpp
                 efxc2Compiler.h
                 efxc2Compiler.cpp
                 efxc2CompilerAPIContainer.h
                 efxc2CompilerAPIContainer.cpp
                 efxc2CompilerIncludes.h
                 efxc2CompilerIncludes.cpp
                 efxc2Console.h
                 efxc2CompilerParams.h
                 efxc2CompilerParams.cpp
                 efxc2CompilerTasks.h
                 efxc2CompilerTasks.cpp
                 efxc2Exception.h
                 efxc2Files.h
                 efxc2Files.cpp
                 efxc2Utils.h
                 efxc2Utils.cpp
                 efxc2.h)

#----resources for .exe

string(REPLACE "." "," _RC_VER ${PROJECT_VERSION})

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/efxc2.ico" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/settings.manifest" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/build/efxc2.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/efxc2.rc" @ONLY)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/build/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
list(APPEND EXEC_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/config.h")
    
list(APPEND EXEC_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/efxc2.rc")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(fxc ${EXEC_SOURCES})
set_target_properties(fxc PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})
target_compile_definitions(fxc PRIVATE _UNICODE UNICODE)

if(EFXC2_BUILD_STATIC)
  IF (WIN32)
    target_link_options(fxc PRIVATE $<$<CXX_COMPILER_ID:Clang,IntelLLVM,GNU>:-municode  -static>)
  else()
    target_link_options(fxc PRIVATE $<$<CXX_COMPILER_ID:Clang,IntelLLVM,GNU>:-static>)
  endif()
else()
  if (WIN32)
    target_link_options(fxc PRIVATE $<$<CXX_COMPILER_ID:Clang,IntelLLVM,GNU>:-municode>)
  endif()
endif()

install(TARGETS fxc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(_MSC_VER)
  target_compile_options(fxc PUBLIC "/ZI")
  target_link_options(fxc PUBLIC "/INCREMENTAL")
endif()

install(FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  ${CMAKE_CURRENT_SOURCE_DIR}/license_MPL_2_0.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/HISTORY.md
  ${CMAKE_CURRENT_SOURCE_DIR}/INSTALL.md
  DESTINATION ${CMAKE_INSTALL_DOCDIR})

if(EFXC2_USE_PVS_STUDIO)
  include(${CMAKE_SOURCE_DIR}/build/PVS-Studio.cmake) 
  if (EFXC2_USE_ALL_RULES)
     pvs_studio_add_target(TARGET fxc.analyze ALL
                      OUTPUT FORMAT sarif
                      ANALYZE fxc
                      MODE 64;GA:1,2;CS;OWASP;AUTOSAR;MISRA 
                      LOG pvs-studio.sarif)
  else()
    pvs_studio_add_target(TARGET fxc.analyze ALL
                      OUTPUT FORMAT sarif      
                      ANALYZE fxc
                      MODE 64;GA:1,2;CS
                      LOG pvs-studio.sarif)
  endif()
endif()

set(CPACK_GENERATOR ZIP)

include(CPack)
enable_testing()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/tests/Empty_file.hlsl "")

add_test(NAME help
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc --help)
add_test(NAME nologo 
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc /nologo BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS.inc  /VnBC7Encode_TryMode456CS)
add_test(NAME default_PDB_filename
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Zss /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS.inc /FdCompiled/ /VnBC7Encode_TryMode456CS)
add_test(NAME default_PDB_filename_dot
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Zss /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS.inc /FdCompiled/. /VnBC7Encode_TryMode456CS)
add_test(NAME short_cmd.BC7Encode_TryMode456CS
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS.inc  /VnBC7Encode_TryMode456CS)
add_test(NAME short_cmd.BC7Encode_TryMode456CS_cs40
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests 
  COMMAND fxc BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS_cs40.inc  /VnBC7Encode_TryMode456CS) 
add_test(NAME long_cmd.BC7Encode_TryMode456CS
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME long_cmd.BC7Encode_TryMode456CS_cs40
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS_cs40.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME short_cmd.BC7Encode_no_vn
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_no_vn.inc)
add_test(NAME hex_BC7Encode_no_vn
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Lx /FhCompiled/BC7Encode_hex.inc)
add_test(NAME error.input_file_not_found
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc NotFound.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Lx /FhCompiled/BC7Encode_hex.inc)
set_tests_properties(error.input_file_not_found PROPERTIES WILL_FAIL true)
add_test(NAME error.output_file_not_found
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/not_found/Compiled/BC7Encode_TryMode456CS_cs40.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
set_tests_properties(error.output_file_not_found PROPERTIES WILL_FAIL true)
add_test(NAME error.output_file_empty
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_BINARY_DIR}/tests/Empty_file.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/not_found/Compiled/BC7Encode_TryMode456CS_cs40.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
set_tests_properties(error.output_file_empty PROPERTIES WILL_FAIL true)
add_test(NAME error.preprocess_with_fo
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason/test_2
  COMMAND fxc BloomCombine.hlsl /debug /I include /P ${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_2..hlsl /E main /T ps_4_0_level_9_1 /Zi /Fo${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_2..cso)
set_tests_properties(error.preprocess_with_fo PROPERTIES WILL_FAIL true)
add_test(NAME error.not_supported_dumpbin
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc /dumpbin DGSLEffect.fx  /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Lx /FhCompiled/BC7Encode_hex.inc)
set_tests_properties(error.not_supported_dumpbin PROPERTIES WILL_FAIL true)

add_test(NAME error.compile_no_input_file_specified
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc  /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Lx /FhCompiled/BC7Encode_hex.inc)
set_tests_properties(error.compile_no_input_file_specified PROPERTIES WILL_FAIL true)

add_test(NAME error.preprocess_no_input_specified
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc /P ${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_2..hlsl /debug)
set_tests_properties(error.preprocess_no_input_specified PROPERTIES WILL_FAIL true)

add_test(NAME invalid_parameters_will_fail.DGSLEffect_fx
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc DGSLEffect.fx /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /debug /Qstrip_debug /Tvs_4_0_level_9_1 /Emain4BonesVc "/Fh${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/DGSLEffect_main4BonesVc.inc" "/Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/DGSLEffect_main4BonesVc.pdb" /VnDGSLEffect_main4BonesVc)
add_test(NAME long_cmd.BC7Encode_TryMode456CS_asm
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /debug /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Fc${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.asm /Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME defines.1_default_define
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /debug /DTEST_DEFINE /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Fc${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.asm /Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME defines.1_define_2
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /debug /DTEST_DEFINE=2 /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Fc${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.asm /Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME defines.2_default_define
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /debug /DTEST_DEFINE /D TEST_DEFINE2 /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Fc${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.asm /Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME defines.2_define_2
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /debug /DTEST_DEFINE=2 /D TEST_DEFINE2=2 /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Fc${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.asm /Fd${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME includes.include_test_1
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason/test_1
  COMMAND fxc BloomCombine.hlsl /debug /E main /T ps_4_0_level_9_1 /Zi /Fo${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_1..cso)
add_test(NAME includes.include_test_1_reldir
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason
  COMMAND fxc test_1/BloomCombine.hlsl /debug /E main /T ps_4_0_level_9_1 /Zi /Fo${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_1..cso)
add_test(NAME includes.include_test_2
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason/test_2
  COMMAND fxc BloomCombine.hlsl /debug /I include /E main /T ps_4_0_level_9_1 /Zi /Fo${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_2..cso)
add_test(NAME includes.include_test_2_FQN
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason/test_2
  COMMAND fxc BloomCombine.hlsl /debug /I ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason/test_2/include /E main /T ps_4_0_level_9_1 /Zi /Fo${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_2..cso)
add_test(NAME includes.preprocess_test_2_FQN
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason/test_2
  COMMAND fxc BloomCombine.hlsl /debug /I ${CMAKE_CURRENT_SOURCE_DIR}/tests//leason/test_2/include /P${CMAKE_CURRENT_BINARY_DIR}/tests/BloomCombin_ps_2..hlsl)

add_test(NAME setprivate.compile.ReinhardEffect
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/D2DAdvancedColorImages/D2DAdvancedColorImages
  COMMAND fxc /T ps_4_0 ReinhardEffect.hlsl /D D2D_FULL_SHADER /D D2D_ENTRY=main /E main /setprivate "compiled/ReinhardEffect.fxlib" /Fo "${CMAKE_CURRENT_BINARY_DIR}/tests/ReinhardEffect.cso" /Fh "${CMAKE_CURRENT_BINARY_DIR}/tests/ReinhardEffect.h" /debug /I includes)

add_test(NAME setprivate.compile.FilmicEffect
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/D2DAdvancedColorImages/D2DAdvancedColorImages
  COMMAND fxc /T ps_4_0 FilmicEffect.hlsl /D D2D_FULL_SHADER /D D2D_ENTRY=main /E main /setprivate "compiled/FilmicEffect.fxlib" /Fo "${CMAKE_CURRENT_BINARY_DIR}/tests/FilmicEffect.cso" /Fh "${CMAKE_CURRENT_BINARY_DIR}/tests/FilmicEffectShader.h" /debug /I includes)

add_test(NAME setprivate.compile.SdrOverlayEffect
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/D2DAdvancedColorImages/D2DAdvancedColorImages
  COMMAND fxc /T ps_4_0 SdrOverlayEffect.hlsl /D D2D_FULL_SHADER /D D2D_ENTRY=main /E main /setprivate "compiled/SdrOverlayEffect.fxlib" /Fo "${CMAKE_CURRENT_BINARY_DIR}/tests/SdrOverlayEffect.cso" /Fh "${CMAKE_CURRENT_BINARY_DIR}/tests/SdrOverlayEffectShader.h" /debug /I includes)

add_test(NAME setprivate.compile.LuminanceHeatmapEffect
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/D2DAdvancedColorImages/D2DAdvancedColorImages
  COMMAND fxc /T ps_4_0 LuminanceHeatmapEffect.hlsl /D D2D_FULL_SHADER /D D2D_ENTRY=main /E main /setprivate "compiled/LuminanceHeatmapEffect.fxlib" /Fo "${CMAKE_CURRENT_BINARY_DIR}/tests/LuminanceHeatmapEffect.cso" /Fh "${CMAKE_CURRENT_BINARY_DIR}/tests/LuminanceHeatmapEffectShader.h" /debug /I includes)
