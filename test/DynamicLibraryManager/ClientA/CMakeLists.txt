# Copyright (c) 2018 Michael Heilmann
# Assert minimum CMake version.
cmake_minimum_required(VERSION 3.8)
# Define the plugin.
define_dynamic_library(Nucleus.Object Nucleus.Object.Test.DynamicLibraryManager.ClientA ${NUCLEUS_LANGUAGE_ID_C})
target_link_libraries(Nucleus.Object.Test.DynamicLibraryManager.ClientA Nucleus.Object.Library)
