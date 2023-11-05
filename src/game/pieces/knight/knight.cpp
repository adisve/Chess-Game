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

std::vector<sf::Vector2i> Knight::AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const {
    std::vector<sf::Vector2i> moves;

    std::vector<sf::Vector2i> potentialMoves = {
            { -2, -1 }, { -1, -2 }, { 1, -2 }, { 2, -1 },
            { 2, 1 }, { 1, 2 }, { -1, 2 }, { -2, 1 }
    };

    for (const auto& move : potentialMoves) {
        int newRow = GetPosition().x + move.x;
        int newCol = GetPosition().y + move.y;

        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            std::shared_ptr<Piece> pieceAtDestination = board.GetPieceAt({newRow, newCol});

            if (!pieceAtDestination || pieceAtDestination->GetColor() != this->GetColor()) {
                moves.emplace_back(newRow, newCol);
            }
        }
    }

    return moves;
}

const sf::Texture &Knight::GetTexture() const {
    return texture;
}
