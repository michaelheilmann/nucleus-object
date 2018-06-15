# Copyright (c) 2017, 2018 Michael Heilmann
# Assert minimum CMake version.
cmake_minimum_required(VERSION 3.8)

# Include the "Nucleus" build system.
if (NOT NUCLEUS_PATH)
  set(NUCLEUS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/nucleus)
endif()
if (NUCLEUS_PATH)
  include(${NUCLEUS_PATH}/buildsystem/buildsystem.cmake)
endif()

# Define module.
define_module(Nucleus.Object)
detect_operating_system(Nucleus.Object)

# If "shared" is not defined, default to "ON".
if (NOT DEFINED Nucleus.Object-Shared)
  set(Nucleus.Object-Shared ON)
endif()

# Include "Nucleus" unless it is already included.
if (NOT NUCLEUS_INCLUDED)
  set(Nucleus-Shared {Nucleus.Object-Shared})
  set(Nucleus-With-Optimizations ${Nucleus.Object-With-Optimizations})
  set(Nucleus-With-Debug-Information ${Nucleus.Object-With-Debug-Information})
  add_subdirectory(${NUCLEUS_PATH})
  set(NUCLEUS_INCLUDED YES)
endif()

# Add module "library".
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/library")

# Add module "test".
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
