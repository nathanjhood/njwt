cmake_minimum_required (VERSION 3.15...3.28 FATAL_ERROR)

#[===========================================================================[
  Dependencies
#]===========================================================================]
list (APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_LIST_DIR}/share/cmake/Modules"
)

include (CMakeRC)
include (CMakeJS)
include (JoinPaths)
include (WriteVersionFile)
include (WriteVersionHeader)

set (CMAKE_CXX_STANDARD 17)

#[===========================================================================[
  Project setup
#]===========================================================================]
project (njwt VERSION 0.0.0)

# Use git to update version information
write_version_file ()
write_version_header ()
configure_file (
  "include/njwt/njwt_version.hpp" # input
  "include/njwt/njwt_version.hpp" # output
)

# Start a list of resources to be compiled
set (RESOURCES)

# Start a list of targets to be built
set (TARGETS)

#[===========================================================================[
  TARGET = njwt::cmrc
#]===========================================================================]
if (FALSE)
message (STATUS "Configuring njwt::cmrc")

list (APPEND RESOURCES
  README.md
  VERSION
  LICENSE
)

cmrc_add_resource_library (resources
  ${RESOURCES}
  ALIAS njwt::cmrc
)

set_target_properties (resources
  PROPERTIES
  LIBRARY_OUTPUT_NAME njwt_cmrc
)

list (APPEND TARGETS resources cmrc-base)
endif()

#[===========================================================================[
  TARGET = njwt::base64
#]===========================================================================]

if (FALSE)
message (STATUS "Configuring njwt::base64")

configure_file (
  "include/njwt/base64/base64.hpp"
  "include/njwt/base64/base64.hpp"
)

add_library (base64 STATIC)
add_library (njwt::base64 ALIAS base64)

# Include our configured headers
target_include_directories (base64
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/base64>
    $<INSTALL_INTERFACE:include/base64>
)

# Create a fileset of 'header' type as part of the library
target_sources (base64

  PUBLIC

    FILE_SET njwt_base64_PUBLIC_HEADERS
    TYPE HEADERS
    BASE_DIRS
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/njwt/base64>
        $<INSTALL_INTERFACE:include/njwt/base64>
    FILES
        # include/njwt/base64/base64.hpp
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/njwt/base64/base64.hpp>
        $<INSTALL_INTERFACE:include/njwt/base64/base64.hpp>
)

# Include our sources to be compiled
target_sources (base64 PRIVATE "${PROJECT_SOURCE_DIR}/src/njwt/base64/base64.cpp")

set_target_properties (base64
  PROPERTIES
  LIBRARY_OUTPUT_NAME njwt_base64
)

# Add the target to the list
list (APPEND TARGETS base64)
endif()

#[===========================================================================[
  TARGET = njwt::njwt
#]===========================================================================]

message (STATUS "Configuring njwt::njwt")

# Add our headers to a list
set (HEADERS)
list (APPEND HEADERS
  "include/njwt/njwt_base64.hpp"
  "include/njwt/njwt_cmrc.hpp"
  "include/njwt/njwt.hpp"
)

# Add out sources to a list
set (SOURCE_FILES)
list (APPEND SOURCE_FILES
  "src/njwt/njwt.cpp"
)

# If building out-of-source...
if (NOT ${PROJECT_SOURCE_DIR} EQUAL ${PROJECT_BINARY_DIR})
  # ...copy our headers to the binary directory.
  foreach (HEADER IN LISTS HEADERS)
    configure_file ("${HEADER}" "${HEADER}")
  endforeach ()
endif ()

add_library (njwt STATIC)
add_library (njwt::njwt ALIAS njwt)

# Include our configured headers
target_include_directories (njwt
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Create a fileset of 'header' type as part of the library
target_sources (njwt

  PUBLIC

    FILE_SET njwt_PUBLIC_HEADERS
    TYPE HEADERS
    BASE_DIRS
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    FILES
        # include/njwt/njwt.hpp
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/njwt/njwt.hpp>
        $<INSTALL_INTERFACE:include/njwt/njwt.h>

        # include/njwt/njwt_version.h
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/njwt/njwt_version.hpp>
        $<INSTALL_INTERFACE:include/njwt/njwt_version.hpp>
)

# Include our sources to be compiled
target_sources (njwt PRIVATE ${SOURCE_FILES})

set_target_properties(njwt
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
  PDB_OUTPUT_DIRECTORY     "${PROJECT_BINARY_DIR}/bin"
)

if (FALSE)
  target_link_libraries (njwt njwt::cmrc)
endif ()

if (FALSE)
  target_link_libraries (njwt njwt::base64)
endif ()

# Add the target to the list
list (APPEND TARGETS njwt)

#[===========================================================================[
  TARGET = njwt::addon-base
#]===========================================================================]

# Alias the addon dependencies
add_library (addon-base INTERFACE)
add_library (njwt::addon-base ALIAS addon-base)
target_link_libraries (addon-base INTERFACE cmake-js::cmake-js)
list (APPEND TARGETS addon-base)

#[===========================================================================[
  TARGET = njwt::addon
#]===========================================================================]

cmakejs_create_napi_addon (
  # NAME
  addon
  # SOURCES
  "src/njwt/njwt_addon.cpp"
  ALIAS njwt
  NAMESPACE njwt::addon
)

set_target_properties(addon
  PROPERTIES
  LIBRARY_OUTPUT_NAME njwt
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
  PDB_OUTPUT_DIRECTORY     "${PROJECT_BINARY_DIR}/bin"
)

target_link_libraries (addon
  PRIVATE
  njwt::addon-base
  njwt::njwt
)

list (APPEND TARGETS addon)

#[===========================================================================[
  Export this configuration to share it with other CMake projects
]===========================================================================]#

# copy the types
file(COPY
  "${PROJECT_SOURCE_DIR}/lib/njwt.node.js"
  "${PROJECT_SOURCE_DIR}/lib/njwt.node.ts"
  "${PROJECT_SOURCE_DIR}/lib/njwt.node.d.ts"
  DESTINATION
  "${PROJECT_BINARY_DIR}/lib"
)

# Collect and export targets
set (njwt_TARGETS "${TARGETS}" CACHE STRING "Targets to be built." FORCE)

export (
  TARGETS ${njwt_TARGETS}
  FILE ${PROJECT_BINARY_DIR}/share/cmake/njwt_targets.cmake
  NAMESPACE njwt::
)

# get access to helper functions for creating config files
include (CMakePackageConfigHelpers)

include (JoinPaths)
join_paths (libdir_for_pc_file     "\${exec_prefix}" "${CMAKE_INSTALL_LIBDIR}")
join_paths (includedir_for_pc_file "\${prefix}"      "${CMAKE_INSTALL_INCLUDEDIR}")

# Create package config file
configure_file (
  ${PROJECT_SOURCE_DIR}/share/pkgconfig/njwt.pc.in
  ${PROJECT_BINARY_DIR}/share/pkgconfig/njwt.pc
  @ONLY
)

# create cmake config file
configure_package_config_file (
    ${PROJECT_SOURCE_DIR}/share/cmake/njwt_config.cmake.in
    ${PROJECT_BINARY_DIR}/share/cmake/njwt_config.cmake
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/njwt
)

# generate the version file for the cmake config file
write_basic_package_version_file (
	${PROJECT_BINARY_DIR}/share/cmake/njwt_config_version.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

#[===========================================================================[
  CTest
#]===========================================================================]

enable_testing()
include ("${PROJECT_SOURCE_DIR}/test/njwt_tests.cmake")

#[===========================================================================[
  CPack
#]===========================================================================]
set(CPACK_PACKAGE_CHECKSUM "${PROJECT_VERSION_TWEAK}")
set(CPACK_PACKAGE_VENDOR "nathanjhood")
set(CPACK_PACKAGE_NAME              "${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_FILE_NAME         "${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}") # Compiled binary distribution
set(CPACK_SOURCE_PACKAGE_FILE_NAME  "${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-Source") # No system spec as this is un-compiled source file distribution
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PROJECT_VERSION_VERSION_TWEAK})
set(CPACK_RESOURCE_FILE_VERSION ${PROJECT_SOURCE_DIR}/VERSION)
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README  ${PROJECT_SOURCE_DIR}/README.md)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    _CPack_Packages
    /*.zip
    /*.tar
    /*.tar.*
    /.env*
    /.git/*
    /.cmake
    /.github
    /.vs
    /.vscode
    /.cache
    /.config
    /.local
    /doc
    /docs
    /bin
    /lib
    /usr
    /out
    /build
    /Release
    /Debug
    /MinSizeRel
    /RelWithDebInfo
    /downloads
    /installed
    /node_modules
    /vcpkg
    /.*build.*
    /package-lock.json
    /yarn.lock
    /\\\\.DS_Store
)
include(CPack)

#[===========================================================================[
  Exit politely.
#]===========================================================================]
unset (HEADERS)
unset (SOURCE_FILES)
unset (TARGETS)
unset (RESOURCES)
