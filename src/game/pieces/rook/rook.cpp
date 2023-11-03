//
// Created by Adis Veletanlic on 11/2/23.
//

#include "rook.h"
#include <iostream>
#include "../../board/board.h"

Rook::Rook(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/rook-black.png");
    } else {
        LoadTexture("assets/sprites/rook-white.png");
    }
}

std::vector<sf::Vector2i> Rook::AvailableMoves(Board board) const {
    std::vector<sf::Vector2i> moves;

    for (int i = row - 1; i >= 0; i--) {
        if (board.GetPieceAt(i, col) == nullptr) {
            moves.emplace_back(i, col);
        } else {
            if (board.GetPieceAt(i, col)->color != color) {
                moves.emplace_back(i, col);
            }
            break;
        }
    }

    // Check downwards, leftwards, and rightwards similarly...

    return moves;
}

bool Rook::CanMove(int toRow, int toCol, const Board &board) const {
    auto moves = AvailableMoves(board);
    return std::find(moves.begin(), moves.end(), sf::Vector2i(toRow, toCol)) != moves.end();
}


const sf::Texture& Rook::GetTexture() const {
    return texture;
}
