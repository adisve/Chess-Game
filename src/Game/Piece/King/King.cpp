//
// Created by Adis Veletanlic on 11/2/23.
//

#include "King.h"
#include <iostream>
#include "../../Board/Board.h"

King::King(sf::Vector2i position, PlayerColor color, PieceType type) : Piece(position, color, type) {
    if (color == PlayerColor::Black) {
        LoadTexture("assets/sprites/King-black.png");
    } else {
        LoadTexture("assets/sprites/King-white.png");
    }
}

std::vector<Move> King::AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const {
    std::vector<Move> moves;

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
                moves.emplace_back(this->GetPosition(), attackPosition, movePosition);
            }
        }
    }
    return moves;
}

const sf::Texture &King::GetTexture() const {
    return texture;
}
