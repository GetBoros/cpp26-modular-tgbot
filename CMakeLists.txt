#-----------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 4.3.0)
#-----------------------------------------------------------------------------------------------------------
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
set(CMAKE_CXX_MODULE_STD ON)  # 2. Turn on module std generation
#-----------------------------------------------------------------------------------------------------------
project(ReflectionTest CXX)
#-----------------------------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)  # Debu or Release
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin)  # store execution app->build/Debug/bin
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib)  # store lib->build/Debug/lib

#-----------------------------------------------------------------------------------------------------------
add_subdirectory(Lib_TGBot)  # static library
add_subdirectory(Lib_Module)  # static library
add_subdirectory(Lib_Clicker)  # static library
add_subdirectory(app)
#-----------------------------------------------------------------------------------------------------------
if(UNIX AND NOT APPLE) # For linux we want to put the executable and .so files in build/bin
    set_target_properties(AApplication PROPERTIES INSTALL_RPATH "$ORIGIN")  # $ORIGIN now points to build/bin
endif()
#-----------------------------------------------------------------------------------------------------------