//
// Created by Adis Veletanlic on 11/2/23.
//

#include "bishop.h"
#include <iostream>
#include "../../board/board.h"

Bishop::Bishop(sf::Vector2i position, Color color) : Piece(position, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/bishop-black.png");
    } else {
        LoadTexture("assets/sprites/bishop-white.png");
    }
}

std::vector<sf::Vector2i> Bishop::AvailableMoves(Board board) const {
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

bool Bishop::CanMove(sf::Vector2i toPosition, const Board& board) const {
    return std::abs(GetPosition().x - toPosition.x) == std::abs(GetPosition().y - toPosition.y);
}

const sf::Texture &Bishop::GetTexture() const {
    return texture;
}
