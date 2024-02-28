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

set(EFXC2_VERSION 0.0.1.46)
project (efxc2 VERSION ${EFXC2_VERSION}
  DESCRIPTION "Enhanced fxc2"
  HOMEPAGE_URL "https://github.com/JPeterMugaas/efxc2"
  LANGUAGES CXX)

include(GNUInstallDirs)

set(PROJECT_COMPANY "J. Peter Mugaas")
set(PROJECT_COPYRIGHT "Copyright (c) 2024 ${PROJECT_COMPANY}")

set(EXEC_SOURCES efxc2.cpp
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
	
if(WIN32)
	list(APPEND EXEC_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/efxc2.rc")
endif()
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(fxc ${EXEC_SOURCES})
target_compile_definitions(fxc PRIVATE _UNICODE UNICODE)

if(MINGW)
   target_link_options(fxc PRIVATE -municode)
endif()

install(TARGETS fxc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(MSVC)
  target_compile_options(fxc PUBLIC "/ZI")
  target_link_options(fxc PUBLIC "/INCREMENTAL")
endif()

install(FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/README.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/license_MPL_2_0.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/HISTORY.md
  DESTINATION ${CMAKE_INSTALL_DOCDIR})

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
  COMMAND fxc /nologo BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS.inc  /VnBC7Encode_TryMode456CS
)

add_test(NAME short_cmd.BC7Encode_TryMode456CS
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS.inc  /VnBC7Encode_TryMode456CS)
add_test(NAME short_cmd.BC7Encode_TryMode456CS_cs40
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests 
  COMMAND fxc BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /FhCompiled/BC7Encode_TryMode456CS_cs40.inc  /VnBC7Encode_TryMode456CS)
  
add_test(NAME long_cmd.BC7Encode_TryMode456CS
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
add_test(NAME long_cmd.BC7Encode_TryMode456CS_cs40
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/BC7Encode_TryMode456CS_cs40.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)

add_test(NAME short_cmd.BC7Encode_no_vn
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /FhCompiled/BC7Encode_no_vn.inc)
  
add_test(NAME hex_BC7Encode_no_vn
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Lx /FhCompiled/BC7Encode_hex.inc)
  
add_test(NAME error.input_file_not_found
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc NotFound.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_5_0 /ETryMode456CS /Lx /FhCompiled/BC7Encode_hex.inc)
set_tests_properties(error.input_file_not_found PROPERTIES WILL_FAIL true)

add_test(NAME error.output_file_not_found
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_SOURCE_DIR}/tests/BC7Encode.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/not_found/Compiled/BC7Encode_TryMode456CS_cs40.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
set_tests_properties(error.output_file_not_found PROPERTIES WILL_FAIL true)

add_test(NAME error.output_file_empty
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests
  COMMAND fxc ${CMAKE_CURRENT_BINARY_DIR}/tests/Empty_file.hlsl /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tcs_4_0 /ETryMode456CS /Fh${CMAKE_CURRENT_BINARY_DIR}/not_found/Compiled/BC7Encode_TryMode456CS_cs40.inc /Fd${CMAKE_CURRENT_BINARY_DIR}/Compiled/BC7Encode_TryMode456CS.pdb /VnBC7Encode_TryMode456CS)
set_tests_properties(error.output_file_empty PROPERTIES WILL_FAIL true)

add_test(NAME invalid_parameters_will_fail.DGSLEffect_fx
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
  COMMAND fxc DGSLEffect.fx /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug /Tvs_4_0_level_9_1 /Emain4BonesVc "/Fh${CMAKE_CURRENT_BINARY_DIR}/tests/Compiled/DGSLEffect_main4BonesVc.inc" "/FdCompiled/DGSLEffect_main4BonesVc.pdb" /VnDGSLEffect_main4BonesVc)