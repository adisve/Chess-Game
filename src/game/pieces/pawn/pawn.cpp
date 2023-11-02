//
// Created by Adis Veletanlic on 11/2/23.
//

#include "pawn.h"
#include <iostream>
#include "../../board/board.h"

Pawn::Pawn(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/pawn-black.png");
    } else {
        LoadTexture("assets/sprites/pawn-white.png");
    }
}

bool Pawn::CanMove(int toRow, int toCol, const Board &board) const {
    if (color == Color::WHITE) {
        if (col == toCol) {
            return (row - toRow == 1) || (row == 6 && toRow == 4);
        } else if (std::abs(col - toCol) == 1) {
            return row - toRow == 1;
        }
    } else {
        if (col == toCol) {
            return (toRow - row == 1) || (row == 1 && toRow == 3);
        } else if (std::abs(col - toCol) == 1) {
            return toRow - row == 1;
        }
    }
    return false;
}

const sf::Texture &Pawn::GetTexture() const {
    return texture;
}

