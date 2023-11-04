//
// Created by Adis Veletanlic on 11/2/23.
//

#include "queen.h"
#include <iostream>
#include "../../board/board.h"

Queen::Queen(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/queen-black.png");
    } else {
        LoadTexture("assets/sprites/queen-white.png");
    }
}

std::vector<sf::Vector2i> Queen::AvailableMoves(Board board) const {
    std::vector<sf::Vector2i> moves;
    std::vector<std::pair<int, int>> directions = {
            {-1, 1}, {1, 1}, {1, -1}, {-1, -1},
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    for (const auto& dir : directions) {
        std::vector<sf::Vector2i> directionMoves = FindMovesInDirectionForPiece(dir.first, dir.second, board);
        moves.insert(moves.end(), directionMoves.begin(), directionMoves.end());
    }
    return moves;
}

bool Queen::CanMove(int toRow, int toCol, const Board &board) const {
    return false;
}

const sf::Texture &Queen::GetTexture() const {
    return texture;
}
