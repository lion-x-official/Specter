# Minimum required version of CMake
cmake_minimum_required(VERSION 3.21)

# Project definition: name, version, and language
project(Specter VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++23, require it, and disable compiler-specific extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Automatically find all .cpp and .hpp files in the src and include directories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    include/*.hpp
)

# Create the main executable from the collected source files
add_executable(Specter ${SOURCES})

# Add the include directory for header file lookup
# This allows #include "..." to find headers in the include directory
# PRIVATE means only this target sees the include path
# PUBLIC would propagate to targets that link against this one
# INTERFACE would only propagate to dependents
# Here, PRIVATE is appropriate for an executable
# CMAKE_SOURCE_DIR is the root of the project
# This is useful for IDE integration and for modular codebases
# You can add more include directories as needed
# Example: target_include_directories(Specter PRIVATE ${CMAKE_SOURCE_DIR}/third_party/include)
target_include_directories(Specter PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Compiler flags for MSVC (Microsoft Visual C++)
# These flags control warning levels, optimization, debug info, and parallel build
# $<$<CONFIG:Debug>:...> applies flags only in Debug configuration
# $<$<CONFIG:Release>:...> applies flags only in Release configuration
# $<$<CONFIG:RelWithDebInfo>:...> applies flags only in RelWithDebInfo configuration
if (MSVC)
    target_compile_options(Specter PRIVATE
        $<$<CONFIG:Debug>:/W3 /Zi>
        $<$<CONFIG:Release>:/O2 /Ob2 /DNDEBUG /MP>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Ob1 /Zi /DNDEBUG /MP>
    )
endif()

# Linker options for MSVC
# /INCREMENTAL:NO disables incremental linking for a more optimized final binary
# /LTCG enables Link Time Code Generation (LTO) for better optimization
# You can add more linker options as needed
# Example: target_link_options(Specter PRIVATE /DEBUG)
target_link_options(Specter PRIVATE /INCREMENTAL:NO /LTCG)

# Preprocessor definitions
# WIN32_LEAN_AND_MEAN excludes rarely-used Windows headers for faster build and smaller binary
# You can add more definitions as needed
# Example: target_compile_definitions(Specter PRIVATE MY_DEFINE=1)
target_compile_definitions(Specter PRIVATE WIN32_LEAN_AND_MEAN)

# Link against Windows system libraries
# kernel32.lib and user32.lib are required for basic Windows API functionality
# You can add more libraries as needed
# Example: target_link_libraries(Specter PRIVATE gdi32.lib)
target_link_libraries(Specter PRIVATE kernel32.lib user32.lib)
