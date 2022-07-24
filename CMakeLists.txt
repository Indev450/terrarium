cmake_minimum_required(VERSION 3.10)

project(terrarium VERSION 0.1)

find_package(Lua 5.4 REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

file(GLOB ENTITY_SOURCES "src/entity/*.cpp")
file(GLOB GRAPHICS_SOURCES "src/graphics/*.cpp")
file(GLOB PLAYER_SOURCES "src/player/*.cpp")
file(GLOB ITEM_SOURCES "src/item/*.cpp")
file(GLOB UI_SOURCES "src/ui/*.cpp")
file(GLOB MAPGEN_SOURCES "src/mapgen/*.cpp")
file(GLOB LUA_API_SOURCES "src/modding/lua/*.cpp")
file(GLOB LIB_PERLIN_SOURCES "src/lib/perlin/*.cpp")

add_executable(terrarium
    src/main.cpp
    ${ENTITY_SOURCES}
    ${GRAPHICS_SOURCES}
    ${PLAYER_SOURCES}
    ${ITEM_SOURCES}
    ${UI_SOURCES}
    ${MAPGEN_SOURCES}
    ${LUA_API_SOURCES}
    ${LIB_PERLIN_SOURCES}
)

set_property(TARGET terrarium PROPERTY CXX_STANDART_REQUIRED ON)
set_property(TARGET terrarium PROPERTY CXX_STANDART 17)
target_compile_options(terrarium PRIVATE -Wall -g)
target_include_directories(terrarium PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(terrarium sfml-system sfml-window sfml-graphics sfml-audio ${LUA_LIBRARY})
