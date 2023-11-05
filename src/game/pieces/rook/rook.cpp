//
// Created by Adis Veletanlic on 11/2/23.
//

#include "rook.h"
#include <iostream>
#include "../../board/board.h"

Rook::Rook(sf::Vector2i position, Color color, PieceType type) : Piece(position, color, type) {
    if (color == Color::Black) {
        LoadTexture("assets/sprites/rook-black.png");
    } else {
        LoadTexture("assets/sprites/rook-white.png");
    }
}

std::vector<sf::Vector2i> Rook::AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const {
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

const sf::Texture& Rook::GetTexture() const {
    return texture;
}
