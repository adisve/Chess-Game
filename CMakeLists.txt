cmake_minimum_required(VERSION 3.26)
project(chessgame)
set(CMAKE_CXX_STANDARD 17)

add_executable(chessgame src/main.cpp
        src/Game/Board/Board.cpp
        src/Game/Board/Board.h
        src/Game/Piece/Piece.h
        src/Game/Piece/PlayerColor.h
        src/Game/Piece/Pawn/Pawn.cpp
        src/Game/Piece/Pawn/Pawn.h
        src/Game/Piece/Knight/Knight.cpp
        src/Game/Piece/Knight/Knight.h
        src/Game/Piece/Rook/Rook.cpp
        src/Game/Piece/Rook/Rook.h
        src/Game/Piece/King/King.cpp
        src/Game/Piece/King/King.h
        src/Game/Piece/Queen/Queen.cpp
        src/Game/Piece/Queen/Queen.h
        src/Game/Piece/Bishop/Bishop.cpp
        src/Game/Piece/Bishop/Bishop.h
        src/Game/Game.cpp
        src/Game/Game.h
        src/Game/Piece/Piece.cpp
        src/Game/GameState/GameState.cpp
        src/Game/GameState/GameState.h
        src/Game/Player/Player.cpp
        src/Game/Player/Player.h
        src/Game/Player/Move.h
        src/Game/MovementManager/MoveManager.cpp
        src/Game/MovementManager/MoveManager.h
)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(chessgame sfml-system sfml-window sfml-graphics sfml-audio sfml-network)