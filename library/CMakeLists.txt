# Copyright (c) 2017, 2018 Michael Heilmann
# Assert minimum CMake version.
cmake_minimum_required (VERSION 3.8)

# Default to "static" (as opposed to "shared") variant.
if (Nucleus.Object-Shared)
  set(Nucleus.Object.Library-Shared ${Nucleus.Object-Shared})
else()
  set(Nucleus.Object.Library-Shared OFF)
endif()

# Define the library.
if (Nucleus.Object.Library-Shared)
  # Define the shared library.
  define_shared_library(Nucleus.Object Nucleus.Object.Library ${NUCLEUS_LANGUAGE_ID_C})
else()
  # Define the static library.
  define_static_library(Nucleus.Object Nucleus.Object.Library ${NUCLEUS_LANGUAGE_ID_C})
endif()

target_link_libraries(Nucleus.Object.Library PUBLIC Nucleus.Library)
# GCC or Clang: Link library with the platform's dynamically loadable library libraries.
if (${Nucleus.Object.Library_C_COMPILER_ID} EQUAL ${NUCLEUS_C_COMPILER_ID_GCC} OR ${Nucleus.Object.Library_C_COMPILER_ID} EQUAL ${NUCLEUS_C_COMPILER_ID_CLANG})
  target_link_libraries(Nucleus.Object.Library PUBLIC dl)
endif()

include(GenerateExportHeader)
generate_export_header(
    Nucleus.Object.Library
    EXPORT_MACRO_NAME Nucleus_Object_Library_Export
    NO_EXPORT_MACRO_NAME Nucleus_Object_Library_NoExport
    DEPRECATED_MACRO_NAME Nucleus_Object_Library_Deprecated  
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/src/Nucleus/Object/Exports.h)
target_include_directories(Nucleus.Object.Library PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/src)
if (NOT Nucleus.Object.Library-Shared)
  set_target_properties(Nucleus.Object.Library PROPERTIES
                        COMPILE_FLAGS -DNUCLEUS_OBJECT_LIBRARY_STATIC_DEFINE)
endif()
