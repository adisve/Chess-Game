//
// Created by Adis Veletanlic on 11/2/23.
//

#include "king.h"
#include <iostream>
#include "../../board/board.h"

King::King(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/king-black.png");
    } else {
        LoadTexture("assets/sprites/king-white.png");
    }
}

std::vector<sf::Vector2i> King::AvailableMoves(Board board) const {

}

bool King::CanMove(int toRow, int toCol, const Board &board) const {
    return false;
}

const sf::Texture &King::GetTexture() const {
    return texture;
}
