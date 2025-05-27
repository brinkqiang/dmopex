cmake_minimum_required(VERSION 3.21)

PROJECT(dmopex)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

SET(DMOPEX_VERSION_MAJOR "1")
SET(DMOPEX_VERSION_MINOR "0")
SET(DMOPEX_VERSION_PATCH "1")
SET(DMOPEX_VERSION "${DMOPEX_VERSION_MAJOR}.${DMOPEX_VERSION_MINOR}.${DMOPEX_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${DMOPEX_VERSION}")

OPTION(USE_DMOPEX "use dmopex" OFF)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/include/dmopex_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/dmopex_config.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

ModuleImportAll("thirdparty")

LibImport("libdmopex" "src/libdmopex")

if(PROJECT_IS_TOP_LEVEL)
    ExeImport("test" "libdmopex;dmtest")
endif()

AddInstall("libdmopex" "${CMAKE_CURRENT_SOURCE_DIR}/include")
