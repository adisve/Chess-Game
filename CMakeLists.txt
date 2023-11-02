cmake_minimum_required(VERSION 3.26)
project(chessgame)
set(CMAKE_CXX_STANDARD 17)

add_executable(chessgame src/main.cpp
        src/game/board/board.cpp
        src/game/board/board.h
        src/game/pieces/piece.h
        src/game/pieces/color.h
        src/game/pieces/pawn/pawn.cpp
        src/game/pieces/pawn/pawn.h
        src/game/pieces/knight/knight.cpp
        src/game/pieces/knight/knight.h
        src/game/pieces/rook/rook.cpp
        src/game/pieces/rook/rook.h
        src/game/pieces/king/king.cpp
        src/game/pieces/king/king.h
        src/game/pieces/queen/queen.cpp
        src/game/pieces/queen/queen.h
        src/game/pieces/bishop/bishop.cpp
        src/game/pieces/bishop/bishop.h)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(chessgame sfml-system sfml-window sfml-graphics sfml-audio sfml-network)