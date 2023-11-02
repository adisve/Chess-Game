//
// Created by Adis Veletanlic on 11/2/23.
//

#include "queen.h"
#include <iostream>
#include "../../board/board.h"

Queen::Queen(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("/Users/adisveletanlic/CLionProjects/chessgame/assets/sprites/queen-black.png");
    } else {
        LoadTexture("/Users/adisveletanlic/CLionProjects/chessgame/assets/sprites/queen-white.png");
    }
}

bool Queen::CanMove(int toRow, int toCol, const Board &board) const {
    return false;
}

const sf::Texture &Queen::GetTexture() const {
    return texture;
}
