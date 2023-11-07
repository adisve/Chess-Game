//
// Created by Adis Veletanlic on 11/2/23.
//

#include "Knight.h"
#include <iostream>
#include "../../Board/Board.h"

Knight::Knight(sf::Vector2i position, PlayerColor color, PieceType type) : Piece(position, color, type) {
    if (color == PlayerColor::Black) {
        LoadTexture("assets/sprites/Knight-black.png");
    } else {
        LoadTexture("assets/sprites/Knight-white.png");
    }
}

std::vector<Move> Knight::AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const {
    std::vector<Move> moves;

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
                moves.emplace_back(movePosition, attackPosition, this->GetPosition());
            }
        }
    }

    return moves;
}
