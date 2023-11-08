cmake_minimum_required(VERSION 3.26)
project(chessgame)
set(CMAKE_CXX_STANDARD 17)

add_executable(chessgame main.cpp
        Game/Board/Board.cpp
        Game/Board/Board.h
        Game/Piece/Piece.h
        Game/Piece/PlayerColor.h
        Game/Piece/Pawn/Pawn.cpp
        Game/Piece/Pawn/Pawn.h
        Game/Piece/Knight/Knight.cpp
        Game/Piece/Knight/Knight.h
        Game/Piece/Rook/Rook.cpp
        Game/Piece/Rook/Rook.h
        Game/Piece/King/King.cpp
        Game/Piece/King/King.h
        Game/Piece/Queen/Queen.cpp
        Game/Piece/Queen/Queen.h
        Game/Piece/Bishop/Bishop.cpp
        Game/Piece/Bishop/Bishop.h
        Game/Game.cpp
        Game/Game.h
        Game/Piece/Piece.cpp
        Game/GameState/GameState.cpp
        Game/GameState/GameState.h
        Game/Player/Player.cpp
        Game/Player/Player.h
        Game/Player/Move.h
        Game/MoveManager/MoveManager.cpp
        Game/MoveManager/MoveManager.h
        Game/AudioManager/AudioManager.cpp
        Game/AudioManager/AudioManager.h
)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(chessgame sfml-system sfml-window sfml-graphics sfml-audio sfml-network)