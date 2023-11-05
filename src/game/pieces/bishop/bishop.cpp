//
// Created by Adis Veletanlic on 11/2/23.
//

#include "bishop.h"
#include <iostream>
#include "../../board/board.h"

Bishop::Bishop(sf::Vector2i position, Color color, PieceType type) : Piece(position, color, type) {
    if (color == Color::Black) {
        LoadTexture("assets/sprites/bishop-black.png");
    } else {
        LoadTexture("assets/sprites/bishop-white.png");
    }
}

std::vector<sf::Vector2i> Bishop::AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const {
    std::vector<sf::Vector2i> moves;
    std::vector<std::pair<int, int>> directions = {
            {-1, 1}, {1, 1}, {1, -1}, {-1, -1}
    };
    for (const auto& dir : directions) {
        std::vector<sf::Vector2i> directionMoves = FindMovesInDirectionForPiece(dir.first, dir.second, board);
        moves.insert(moves.end(), directionMoves.begin(), directionMoves.end());
    }
    return moves;
}

const sf::Texture &Bishop::GetTexture() const {
    return texture;
}
