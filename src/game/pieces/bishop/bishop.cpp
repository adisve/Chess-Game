//
// Created by Adis Veletanlic on 11/2/23.
//

#include "bishop.h"
#include <iostream>
#include "../../board/board.h"

Bishop::Bishop(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/bishop-black.png");
    } else {
        LoadTexture("assets/sprites/bishop-white.png");
    }
}

bool Bishop::CanMove(int toRow, int toCol, const Board& board) const {
    return std::abs(row - toRow) == std::abs(col - toCol);
}

const sf::Texture &Bishop::GetTexture() const {
    return texture;
}
