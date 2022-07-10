cmake_minimum_required(VERSION 3.10)

project(terrarium VERSION 0.1)

# Maybe i could just write list of files
file(GLOB ENTITY_SOURCES "src/entity/*.cpp")
file(GLOB GRAPHICS_SOURCES "src/graphics/*.cpp")
file(GLOB PLAYER_SOURCES "src/player/*.cpp")
file(GLOB ITEM_SOURCES "src/item/*.cpp")
file(GLOB UI_SOURCES "src/ui/*.cpp")

set(SOURCES
    src/main.cpp
    ${ENTITY_SOURCES}
    ${GRAPHICS_SOURCES}
    ${PLAYER_SOURCES}
    ${ITEM_SOURCES}
    ${UI_SOURCES}
)

add_definitions("-Wall")
add_executable(terrarium ${SOURCES})

# Config file
configure_file(src/terrarium_config.hpp.in terrarium_config.hpp)

# Add config header to includes
target_include_directories(terrarium PUBLIC "${PROJECT_BINARY_DIR}")

# Add libs
target_link_libraries(terrarium sfml-system sfml-window sfml-graphics sfml-audio)
