cmake_minimum_required (VERSION 3.15...3.28 FATAL_ERROR)

#[===========================================================================[
  Dependencies
]===========================================================================]#
list (APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_LIST_DIR}/share/cmake/Modules"
)

include (CMakeRC)
include (CMakeJS)
include (JoinPaths)


#[===========================================================================[
  Project setup
]===========================================================================]#
project (njwt VERSION 0.0.0)

# Use git to update version information
include (WriteVersionFile)
write_version_file ()
include (WriteVersionHeader)
write_version_header ()
configure_file (
  "include/${PROJECT_NAME}/version.hpp" # input
  "include/${PROJECT_NAME}/version.hpp" # output
)

# Start a list of targets to be built
set (TARGETS)

cmakejs_create_napi_addon (
  # NAME
  addon
  # SOURCES
  "src/${PROJECT_NAME}/addon.cpp"
)
