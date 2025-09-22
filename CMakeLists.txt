cmake_minimum_required(VERSION 3.16)
project(chess_server LANGUAGES CXX)

# C++ szabvány
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Forrásfájlok összegyűjtése
set(SOURCES
    backend/main.cpp
    backend/logic/Game.cpp
)

set(HEADERS
    backend/logic/Game.h
    backend/logic/Board.h
    backend/logic/Move.h
    backend/logic/pieces/Piece.h
    backend/logic/pieces/King.h
    backend/logic/pieces/Pawn.h
    backend/logic/pieces/Bishop.h
    backend/logic/pieces/Rook.h
    backend/logic/pieces/Queen.h
    backend/logic/pieces/Knight.h
)

# Crow és nlohmann/json include mappák
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/backend/logic
    ${CMAKE_SOURCE_DIR}/backend/logic/pieces
)

# Végrehajtható fájl
add_executable(server ${SOURCES} ${HEADERS})

# Ha a Crow header-only, nem kell linkelni, csak include
# Ha van külső lib, itt lehet linkelni:
# target_link_libraries(server PRIVATE pthread)

# Build mappa beállítása
set_target_properties(server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

# Frontend statikus fájlok másolása build-be
add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/frontend
        ${CMAKE_SOURCE_DIR}/build/frontend
)

target_link_libraries(server
    PRIVATE
        ws2_32
        mswsock
        advapi32
)