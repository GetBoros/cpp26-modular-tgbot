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
target_compile_options(AApplication PRIVATE -Wno-template-body -Wno-unused-result)
#-------------------------------------------------------------------------------------------------------------
target_link_libraries(AApplication PRIVATE
    ALib_Clicker
    ALib_Server
    ALib_Module
    ALib_TGBot
    ixwebsocket      # <-- Добавили
    simdjson
    ssl              # <-- OpenSSL
    crypto           # <-- OpenSSL
    z                # <-- ZLib компрессия
    pthread          # <-- Потоки Linux
    ) # Link the library to the executable
#-------------------------------------------------------------------------------------------------------------
target_include_directories(AApplication PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/local/include   # <-- Добавили путь к заголовочным файлам
)
#-------------------------------------------------------------------------------------------------------------
