#-------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 4.3.0)
#-------------------------------------------------------------------------------------------------------------
project(ALib_Clicker VERSION 0.1)  # Project name for IDE`s
#-------------------------------------------------------------------------------------------------------------
add_library(ALib_Clicker STATIC)  # Create a static library
target_sources(ALib_Clicker
    PUBLIC
      FILE_SET CXX_MODULES FILES
        src/Lib_Clicker.cppm
        src/Clicker.cppm

    PRIVATE
        src/Lib_Clicker.cpp
        src/Clicker.cpp
)
#-------------------------------------------------------------------------------------------------------------
set_target_properties(ALib_Clicker PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
    CXX_SCAN_FOR_MODULES ON
    CXX_MODULE_STD ON
)
#-------------------------------------------------------------------------------------------------------------
# target_compile_options(ALib_Clicker PRIVATE -freflection)  # #include <meta>
#-------------------------------------------------------------------------------------------------------------
