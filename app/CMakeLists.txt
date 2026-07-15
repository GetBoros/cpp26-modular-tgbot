#-------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 4.3.0)
#-------------------------------------------------------------------------------------------------------------
project(ReflectionTest VERSION 0.1) # Project name for IDE`s
#-------------------------------------------------------------------------------------------------------------
add_executable(AApplication src/main.cpp)
target_sources(AApplication
    PRIVATE
        src/main.cpp
    PUBLIC
        FILE_SET CXX_MODULES FILES
            src/Test.cppm
)
#-------------------------------------------------------------------------------------------------------------
set_target_properties(AApplication PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
    CXX_SCAN_FOR_MODULES ON
    CXX_MODULE_STD ON
)
#-------------------------------------------------------------------------------------------------------------
target_link_libraries(AApplication PRIVATE
    ALib_Clicker
    ALib_Module
    ALib_TGBot
    ) # Link the library to the executable
#-------------------------------------------------------------------------------------------------------------
target_include_directories(AApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)  # include directory library
#-------------------------------------------------------------------------------------------------------------
