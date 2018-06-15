# Copyright (c) 2017 - 2018 Michael Heilmann
# Assert minimum CMake version.
cmake_minimum_required (VERSION 3.8)
# Define the tool.
set(libraries Nucleus.Library)
define_executable(Nucleus.Media Nucleus.Media.Tools.Generator ${libraries} ${NUCLEUS_LANGUAGE_ID_C})
