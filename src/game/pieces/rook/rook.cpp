//
// Created by Adis Veletanlic on 11/2/23.
//

#include "rook.h"
#include <iostream>
#include "../../board/board.h"

Rook::Rook(sf::Vector2i position, Color color) : Piece(position, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/rook-black.png");
    } else {
        LoadTexture("assets/sprites/rook-white.png");
    }
}

std::vector<sf::Vector2i> Rook::AvailableMoves(Board board) const {
    std::vector<sf::Vector2i> moves;
    std::vector<std::pair<int, int>> directions = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    for (const auto& dir : directions) {
        std::vector<sf::Vector2i> directionMoves = FindMovesInDirectionForPiece(dir.first, dir.second, board);
        moves.insert(moves.end(), directionMoves.begin(), directionMoves.end());
    }
    return moves;
}


bool Rook::CanMove(sf::Vector2i toPosition, const Board &board) const {
    auto moves = AvailableMoves(board);
    return std::find(moves.begin(), moves.end(), sf::Vector2i(toPosition.x, toPosition.y)) != moves.end();
}


const sf::Texture& Rook::GetTexture() const {
    return texture;
}
