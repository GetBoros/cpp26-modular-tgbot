#-------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 4.3.0)
#-------------------------------------------------------------------------------------------------------------
project(ALib_Server VERSION 0.1)  # Project name for IDE`s
#-------------------------------------------------------------------------------------------------------------
add_library(ALib_Server STATIC)  # Create a static library
target_sources(ALib_Server
    PUBLIC
      FILE_SET CXX_MODULES FILES
        src/Server.cppm

    PRIVATE
        src/Server.cpp
)
#-------------------------------------------------------------------------------------------------------------
set_target_properties(ALib_Server PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
    CXX_SCAN_FOR_MODULES ON
    CXX_MODULE_STD ON
)
#-------------------------------------------------------------------------------------------------------------
# target_compile_options(ALib_Server PRIVATE -freflection)  # #include <meta>
#-------------------------------------------------------------------------------------------------------------
