//
// Created by Adis Veletanlic on 11/2/23.
//

#include "knight.h"
#include <iostream>
#include "../../board/board.h"

Knight::Knight(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/knight-black.png");
    } else {
        LoadTexture("assets/sprites/knight-white.png");
    }
}

std::vector<sf::Vector2i> Knight::AvailableMoves(Board board) const {
    std::vector<sf::Vector2i> moves;

    // Define all the 8 potential moves for a knight
    std::vector<sf::Vector2i> potentialMoves = {
            { -2, -1 }, { -1, -2 }, { 1, -2 }, { 2, -1 },
            { 2, 1 }, { 1, 2 }, { -1, 2 }, { -2, 1 }
    };

    for (const auto& move : potentialMoves) {
        int newRow = row + move.x;
        int newCol = col + move.y;

        // Check if move is inside board boundaries
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            std::shared_ptr<Piece> pieceAtDestination = board.GetPieceAt(newRow, newCol);

            // Ensure that the destination square is either empty or contains an opponent's piece
            if (!pieceAtDestination || pieceAtDestination->color != this->color) {
                moves.emplace_back(newRow, newCol);
            }
        }
    }

    return moves;
}

bool Knight::CanMove(int toRow, int toCol, const Board &board) const {
    return (std::abs(row - toRow) == 2 && std::abs(col - toCol) == 1) ||
           (std::abs(row - toRow) == 1 && std::abs(col - toCol) == 2);
}

const sf::Texture &Knight::GetTexture() const {
    return texture;
}
