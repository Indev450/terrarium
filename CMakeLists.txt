cmake_minimum_required(VERSION 3.10)

project(terrarium VERSION 0.1)

find_package(Lua 5.4 REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

file(GLOB ENTITY_SOURCES "src/entity/*.cpp")
file(GLOB GRAPHICS_SOURCES "src/graphics/*.cpp")
file(GLOB SOUNDS_SOURCES "src/sounds/*.cpp")
file(GLOB ACTIVITY_SOURCES "src/activity/*.cpp")
file(GLOB PLAYER_SOURCES "src/player/*.cpp")
file(GLOB ITEM_SOURCES "src/item/*.cpp")
file(GLOB CRAFT_SOURCES "src/craft/*.cpp")
file(GLOB UI_SOURCES "src/ui/*.cpp")
file(GLOB WORLD_SOURCES "src/world/*.cpp")
file(GLOB MAPGEN_SOURCES "src/mapgen/*.cpp")
file(GLOB LUA_SERVER_API_SOURCES "src/modding/lua/server/*.cpp")
file(GLOB LUA_CLIENT_API_SOURCES "src/modding/lua/client/*.cpp")
file(GLOB UTILS_SOURCES "src/utils/*.cpp")
file(GLOB LIB_PERLIN_SOURCES "src/lib/perlin/*.cpp")

add_executable(terrarium
    src/main.cpp
    src/game.cpp
    ${ENTITY_SOURCES}
    ${GRAPHICS_SOURCES}
    ${SOUNDS_SOURCES}
    ${ACTIVITY_SOURCES}
    ${PLAYER_SOURCES}
    ${ITEM_SOURCES}
    ${CRAFT_SOURCES}
    ${UI_SOURCES}
    ${WORLD_SOURCES}
    ${MAPGEN_SOURCES}
    ${LUA_SERVER_API_SOURCES}
    ${LUA_CLIENT_API_SOURCES}
    ${UTILS_SOURCES}
    ${LIB_PERLIN_SOURCES}
)

target_compile_features(terrarium PUBLIC cxx_std_17)
target_compile_options(terrarium PRIVATE -Wall -g)
target_include_directories(terrarium PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(terrarium pthread sfml-system sfml-window sfml-graphics sfml-audio ${LUA_LIBRARY})
