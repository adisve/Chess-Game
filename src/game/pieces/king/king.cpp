//
// Created by Adis Veletanlic on 11/2/23.
//

#include "king.h"
#include <iostream>
#include "../../board/board.h"

King::King(sf::Vector2i position, Color color, PieceType type) : Piece(position, color, type) {
    if (color == Color::Black) {
        LoadTexture("assets/sprites/king-black.png");
    } else {
        LoadTexture("assets/sprites/king-white.png");
    }
}

std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> King::AvailableMoves(const Board& board, const std::optional<std::tuple<sf::Vector2i, sf::Vector2i>>& lastMove) const {
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> moves;

    std::vector<sf::Vector2i> potentialMoves = {
            { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 },
            { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }
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

const sf::Texture &King::GetTexture() const {
    return texture;
}
