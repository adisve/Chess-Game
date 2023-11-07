//
// Created by Adis Veletanlic on 11/2/23.
//

#include "Bishop.h"
#include <iostream>
#include "../../Board/Board.h"

Bishop::Bishop(sf::Vector2i position, PlayerColor color, PieceType type) : Piece(position, color, type) {
    if (color == PlayerColor::Black) {
        LoadTexture("assets/sprites/Bishop-black.png");
    } else {
        LoadTexture("assets/sprites/Bishop-white.png");
    }
}

std::vector<Move> Bishop::AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const {
    std::vector<Move> moves;
    std::vector<std::pair<int, int>> directions = {
            {-1, 1}, {1, 1}, {1, -1}, {-1, -1}
    };
    for (const auto& dir : directions) {
        std::vector<Move> directionMoves = FindMovesInDirectionForPiece(dir.first, dir.second, board);
        moves.insert(moves.end(), directionMoves.begin(), directionMoves.end());
    }
    return moves;
}

const sf::Texture &Bishop::GetTexture() const {
    return texture;
}
