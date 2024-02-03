cmake_minimum_required (VERSION 3.20)

set(EFXC2_VERSION 0.0.0.2)
project (efxc2 VERSION ${EFXC2_VERSION}
  DESCRIPTION "Enhanced version of fxc2"
  HOMEPAGE_URL "https://github.com/mozilla/fxc2"
  LANGUAGES CXX)
  
set(PROJECT_COMPANY "J. Peter Mugaas")
set(PROJECT_COPYRIGHT "Copyright (c) ${PROJECT_COMPANY}")

set(EXEC_SOURCES efxc2.cpp)

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
if(WIN32)
    list(APPEND EXEC_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/efxc2.rc")
endif()
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(fxc ${EXEC_SOURCES})
target_compile_definitions(fxc PRIVATE _UNICODE UNICODE)
target_link_libraries(fxc PRIVATE ole32.lib version.lib)
install(TARGETS fxc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})