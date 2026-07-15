#-------------------------------------------------------------------------------------------------------------
project(ALib_Module VERSION 0.1)  # Project name for IDE`s
#-------------------------------------------------------------------------------------------------------------
add_library(ALib_Module STATIC)  # Create a static library
target_sources(ALib_Module
  PUBLIC
    FILE_SET CXX_MODULES FILES  # Module interface
      src/Module_Math.cppm
      src/Lib_Module.cppm
      src/Example_Reflection.cppm
      src/Lib_Module_Concepts.cppm
      src/Lib_Module_Reflection.cppm

  PRIVATE
    src/Module_Math.cpp  # Hiddent module realization
    src/Lib_Module.cpp
    src/Example_Reflection.cpp
    src/Lib_Module_Concepts.cpp
    src/Lib_Module_Reflection.cpp
)
#-------------------------------------------------------------------------------------------------------------
# 1.0. Explicitly enable C++26 modules and standard library import for ALib_Module
set_target_properties(ALib_Module PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
    CXX_SCAN_FOR_MODULES ON
    CXX_MODULE_STD ON
)
#-------------------------------------------------------------------------------------------------------------
target_compile_options(ALib_Module PRIVATE -freflection)  # add #include <meta>
#-------------------------------------------------------------------------------------------------------------
