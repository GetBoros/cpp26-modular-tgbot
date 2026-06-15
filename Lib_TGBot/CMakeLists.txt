#-------------------------------------------------------------------------------------------------------------
project(ALib_TGBot VERSION 0.1)  # Project name for IDE`s
#-------------------------------------------------------------------------------------------------------------
include(FetchContent)

set(CPR_CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)  # Disable don`t need for now 

set(PARENT_CXX_STANDARD ${CMAKE_CXX_STANDARD})  # Save global standart cpr need std 17 or errors
set(PARENT_CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD_REQUIRED})

set(CMAKE_CXX_STANDARD 17)  # Reset global std to 17 CPR and JSON use 11 or 17 std
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.14.0  # Use verified version
)

FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3  # Use verified version
)
FetchContent_MakeAvailable(json)

set(CMAKE_CXX_STANDARD ${PARENT_CXX_STANDARD})  # Restore global std in main CMake
set(CMAKE_CXX_STANDARD_REQUIRED ${PARENT_CXX_STANDARD_REQUIRED})
#-------------------------------------------------------------------------------------------------------------
add_library(ALib_TGBot STATIC)  # Create a static library

target_sources(ALib_TGBot
  PUBLIC
    FILE_SET CXX_MODULES FILES  # Module interface
      src/Lib_TGBot.cppm
      src/TGB_Parser.cppm
      src/TGB_Network.cppm
      src/TGB_Manager.cppm

  PRIVATE  # Hiddent module realization
    src/Lib_TGBot.cpp
    src/TGB_Parser.cpp
    src/TGB_Network.cpp
    src/TGB_Manager.cpp
)
#-------------------------------------------------------------------------------------------------------------
target_compile_options(ALib_TGBot
    PRIVATE
        -freflection  # add #include <meta>
        -Wno-deprecated-declarations  # static_assert(std::is_trivial<CharType>::value, "CharType must be trivial")
    )
#-------------------------------------------------------------------------------------------------------------
target_link_libraries(ALib_TGBot  # Link downloaded lib to our module
    PRIVATE 
        cpr::cpr 
        nlohmann_json::nlohmann_json
)
#-------------------------------------------------------------------------------------------------------------
