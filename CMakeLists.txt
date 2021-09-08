cmake_minimum_required(VERSION 3.12)
project(Horion)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_GENERATOR_PLATFORM x64)

### Other Stuff ###
# if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#     add_compile_options(-Wno-dynamic-class-memaccess -Wno-microsoft-goto -Wno-delete-incomplete /EHsc /Ob1)
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#     # add_compile_options(/MP /Ob1)
# endif()

### Add MACROS ###
add_compile_definitions(WIN32_LEAN_AND_MEAN)
add_compile_definitions(NOMINMAX)
if(CMAKE_BUILD_TYPE MATCHES Release)
elseif(CMAKE_BUILD_TYPE MATCHES BetaConf)
    add_compile_definitions(_BETA)
endif()

file(GLOB_RECURSE HorionFiles
        "*.h"
        "*.cpp")
add_library(Horion SHARED ${HorionFiles})

### Add Library ###
# add_library(Horion SHARED "Horion/Loader.cpp")

### Include Subdirectories ###
target_include_directories(Horion PUBLIC ".")
target_include_directories(Horion PUBLIC "include")
target_include_directories(Horion PUBLIC "include/glm/")

### Link Libraries ###
target_link_libraries(Horion PUBLIC minhook)

### Add Subdirectories ###
add_subdirectory(Lib)
# add_subdirectory(Memory)
# add_subdirectory(Horion)
# add_subdirectory(SDK)