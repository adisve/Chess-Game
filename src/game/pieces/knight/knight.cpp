//
// Created by Adis Veletanlic on 11/2/23.
//

#include "knight.h"
#include <iostream>
#include "../../board/board.h"

Knight::Knight(sf::Vector2i position, Color color, PieceType type) : Piece(position, color, type) {
    if (color == Color::Black) {
        LoadTexture("assets/sprites/knight-black.png");
    } else {
        LoadTexture("assets/sprites/knight-white.png");
    }
}

std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> Knight::AvailableMoves(const Board& board, const std::optional<std::tuple<sf::Vector2i, sf::Vector2i>>& lastMove) const {
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> moves;

    std::vector<sf::Vector2i> potentialMoves = {
            { -2, -1 }, { -1, -2 }, { 1, -2 }, { 2, -1 },
            { 2, 1 }, { 1, 2 }, { -1, 2 }, { -2, 1 }
    };

    for (const auto& move : potentialMoves) {
        int newRow = GetPosition().x + move.x;
        int newCol = GetPosition().y + move.y;

        if (Board::IsWithinBounds({newRow, newCol})) {
            std::shared_ptr<Piece> pieceAtDestination = board.GetPieceAt({newRow, newCol});

            if (!pieceAtDestination || pieceAtDestination->GetColor() != this->GetColor()) {
                sf::Vector2i attackPosition = {newRow, newCol};
                sf::Vector2i movePosition = {newRow, newCol};
                moves.emplace_back(attackPosition, movePosition);
            }
        }
    }

    return moves;
}

const sf::Texture &Knight::GetTexture() const {
    return texture;
}
