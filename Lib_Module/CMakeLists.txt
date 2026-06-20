#-------------------------------------------------------------------------------------------------------------
project(ALib_Module VERSION 0.1)  # Project name for IDE`s
#-------------------------------------------------------------------------------------------------------------
add_library(ALib_Module STATIC)  # Create a static library
target_sources(ALib_Module
  PUBLIC
    FILE_SET CXX_MODULES FILES  # Module interface
      src/Module_Math.cppm
      src/Lib_Module.cppm
      src/Lib_Module_Concepts.cppm
      src/Lib_Module_Reflection.cppm

  PRIVATE
    src/Module_Math.cpp  # Hiddent module realization
    src/Lib_Module.cpp
    src/Lib_Module_Concepts.cpp
    src/Lib_Module_Reflection.cpp
)
#-------------------------------------------------------------------------------------------------------------
target_compile_options(ALib_Module PRIVATE -freflection)  # add #include <meta>
#-------------------------------------------------------------------------------------------------------------
