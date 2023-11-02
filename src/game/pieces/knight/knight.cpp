//
// Created by Adis Veletanlic on 11/2/23.
//

#include "knight.h"
#include <iostream>
#include "../../board/board.h"

Knight::Knight(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("/Users/adisveletanlic/CLionProjects/chessgame/assets/sprites/knight-black.png");
    } else {
        LoadTexture("/Users/adisveletanlic/CLionProjects/chessgame/assets/sprites/knight-white.png");
    }
}

bool Knight::CanMove(int toRow, int toCol, const Board &board) const {
    return (std::abs(row - toRow) == 2 && std::abs(col - toCol) == 1) ||
           (std::abs(row - toRow) == 1 && std::abs(col - toCol) == 2);
}

const sf::Texture &Knight::GetTexture() const {
    return texture;
}
