# Copyright (c) 2017, 2018 Michael Heilmann
# Assert minimum CMake version.
cmake_minimum_required(VERSION 3.8)
# Define the test.
define_test(Nucleus.Object Nucleus.Object.Test.DynamicLibraryManager.Host ${NUCLEUS_LANGUAGE_ID_C})
target_link_libraries(Nucleus.Object.Test.DynamicLibraryManager.Host PRIVATE Nucleus.Object.Library)
