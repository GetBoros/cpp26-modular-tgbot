#-------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 4.3.0)
#-------------------------------------------------------------------------------------------------------------
project(ALib_CBWatcher VERSION 0.1)  # Project name for IDE`s
#-------------------------------------------------------------------------------------------------------------
add_library(ALib_CBWatcher STATIC)  # Create a static library
target_sources(ALib_CBWatcher
    PUBLIC
      FILE_SET CXX_MODULES FILES
        src/CB_Fetcher.cppm
        src/CB_Room_Listener.cppm
        src/Lib_CBWatcher.cppm

    PRIVATE
        src/CB_Fetcher.cpp
        src/CB_Room_Listener.cpp
        src/Lib_CBWatcher.cpp
)
#-------------------------------------------------------------------------------------------------------------
set_target_properties(ALib_CBWatcher PROPERTIES
    CXX_STANDARD 26
    CXX_STANDARD_REQUIRED ON
    CXX_SCAN_FOR_MODULES ON
    CXX_MODULE_STD ON
)
#-------------------------------------------------------------------------------------------------------------
# target_compile_options(ALib_CBWatcher PRIVATE -freflection)  # #include <meta>
#-------------------------------------------------------------------------------------------------------------
target_link_libraries(ALib_CBWatcher PRIVATE
    ixwebsocket
    cpr::cpr
    simdjson
    ssl  # <-- OpenSSL
    crypto  # <-- OpenSSL
    z  # <-- ZLib compression
    pthread  # <-- threads Linux
    ) # Link the library to the executable
#-------------------------------------------------------------------------------------------------------------
target_include_directories(ALib_CBWatcher PUBLIC
    /usr/local/include  # simdjson or ixwebsocket includes here
)
#-------------------------------------------------------------------------------------------------------------
