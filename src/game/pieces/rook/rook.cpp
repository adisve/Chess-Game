//
// Created by Adis Veletanlic on 11/2/23.
//

#include "rook.h"
#include <iostream>
#include "../../board/board.h"

Rook::Rook(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("/Users/adisveletanlic/CLionProjects/chessgame/assets/sprites/pawn-black.png");
    } else {
        LoadTexture("/Users/adisveletanlic/CLionProjects/chessgame/assets/sprites/pawn-white.png");
    }
}

bool Rook::CanMove(int toRow, int toCol, const Board &board) const {
    return false;
}

const sf::Texture& Rook::GetTexture() const {
    return texture;
}
