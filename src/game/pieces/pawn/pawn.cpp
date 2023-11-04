//
// Created by Adis Veletanlic on 11/2/23.
//

#include "pawn.h"
#include <iostream>
#include "../../board/board.h"

Pawn::Pawn(int row, int col, Color color) : Piece(row, col, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/pawn-black.png");
    } else {
        LoadTexture("assets/sprites/pawn-white.png");
    }
}

std::vector<sf::Vector2i> Pawn::AvailableMoves(Board board) const {
    std::vector<sf::Vector2i> moves;
    int direction = (color == Color::WHITE) ? -1 : 1;

    // Helper lambda to check if a position is within board bounds
    auto isWithinBounds = [](int r, int c) {
        return r >= 0 && r < 8 && c >= 0 && c < 8;
    };

    // Single square forward
    if (isWithinBounds(row + direction, col) && board.GetPieceAt(row + direction, col) == nullptr) {
        moves.emplace_back(row + direction, col);

        // Double square forward for pawns that haven't moved
        if (!hasMoved && isWithinBounds(row + 2 * direction, col) && board.GetPieceAt(row + 2 * direction, col) == nullptr) {
            moves.emplace_back(row + 2 * direction, col);
        }
    }

    // Diagonal captures
    if (isWithinBounds(row + direction, col - 1)) {
        auto leftCapture = board.GetPieceAt(row + direction, col - 1);
        if (leftCapture && leftCapture->color != this->color) {
            moves.emplace_back(row + direction, col - 1);
        }
    }

    if (isWithinBounds(row + direction, col + 1)) {
        auto rightCapture = board.GetPieceAt(row + direction, col + 1);
        if (rightCapture && rightCapture->color != this->color) {
            moves.emplace_back(row + direction, col + 1);
        }
    }

    auto lastMovedPiece = board.GetLastMovedPiece();
    auto lastMovedPiecePrevPos = board.GetLastMovedPiecePreviousPosition();

    if (lastMovedPiece && dynamic_cast<Pawn*>(lastMovedPiece.get()) && abs(lastMovedPiecePrevPos.x - row) == 2) {
        // Left en passant
        if (isWithinBounds(row + direction, col - 1) && col - 1 == lastMovedPiecePrevPos.y && row + direction == lastMovedPiecePrevPos.x) {
            moves.emplace_back(row + direction, col - 1);
        }

        // Right en passant
        if (isWithinBounds(row + direction, col + 1) && col + 1 == lastMovedPiecePrevPos.y && row + direction == lastMovedPiecePrevPos.x) {
            moves.emplace_back(row + direction, col + 1);
        }
    }

    return moves;
}



bool Pawn::CanMove(int toRow, int toCol, const Board &board) const {
    if (color == Color::WHITE) {
        if (col == toCol) {
            return (row - toRow == 1) || (row == 6 && toRow == 4);
        } else if (std::abs(col - toCol) == 1) {
            return row - toRow == 1;
        }
    } else {
        if (col == toCol) {
            return (toRow - row == 1) || (row == 1 && toRow == 3);
        } else if (std::abs(col - toCol) == 1) {
            return toRow - row == 1;
        }
    }
    return false;
}

const sf::Texture &Pawn::GetTexture() const {
    return texture;
}

