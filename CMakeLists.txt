cmake_minimum_required (VERSION 2.8)
project (Wyrmwick)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

file (GLOB GAME_INCLUDES src/*.h)
file (GLOB GAME_SOURCES src/*.cpp)

set (WYRMWICK_INCLUDES ${GAME_INCLUDES})
set (WYRMWICK_SOURCES ${GAME_SOURCES})

find_package (SDL2 REQUIRED)
include_directories (${SDL2_INCLUDE_DIR})
link_directories (${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})

find_package (Lua 5.2 REQUIRED)
include_directories (${LUA_INCLUDE_DIR})
link_directories (${LUA_LIBRARY})

add_executable (Wyrmwick ${WYRMWICK_SOURCES} ${WYRMWICK_INCLUDES})
target_link_libraries (Wyrmwick ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY} ${LUA_LIBRARY})