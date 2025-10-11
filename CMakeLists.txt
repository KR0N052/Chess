cmake_minimum_required(VERSION 3.16)
project(chess_server LANGUAGES CXX)

# C++ szabvány
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(backend
    backend/logic/Game.cpp
    backend/logic/Bitboards.cpp
    backend/logic/Bitboards.h
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

enable_testing()

# Forrásfájlok összegyűjtése
set(SOURCES
    backend/main.cpp
    backend/logic/Game.cpp
    backend/logic/Bitboards.cpp
)

set(HEADERS
    backend/logic/Game.h
    backend/logic/Board.h
    backend/logic/Move.h
    backend/logic/Bitboards.h
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

add_executable(basic_piece_tests backend/tests/basic_piece_tests.cpp)

target_link_libraries(basic_piece_tests PRIVATE backend)

add_test(NAME PawnTests   COMMAND basic_piece_tests p)
add_test(NAME RookTests   COMMAND basic_piece_tests r)
add_test(NAME KnightTests COMMAND basic_piece_tests k)
add_test(NAME BishopTests COMMAND basic_piece_tests b)
add_test(NAME QueenTests  COMMAND basic_piece_tests q)
add_test(NAME KingTests   COMMAND basic_piece_tests k)

add_executable(special_moves_tests backend/tests/special_moves_tests.cpp)

target_link_libraries(special_moves_tests PRIVATE backend)

add_test(NAME CastlingTests        COMMAND special_moves_tests castle)
add_test(NAME EnPassantTests       COMMAND special_moves_tests enpassant)
add_test(NAME PromotionTests       COMMAND special_moves_tests promotion)

add_executable(global_tests backend/tests/global_tests.cpp)

target_link_libraries(global_tests PRIVATE backend)

add_test(NAME CheckMateTest       COMMAND global_tests checkmate)
add_test(NAME AttackedTest       COMMAND global_tests attacked)
add_test(NAME StalemateTest       COMMAND global_tests stalemate)
add_test(NAME WouldBeInCheckTest       COMMAND global_tests wouldbeincheck)