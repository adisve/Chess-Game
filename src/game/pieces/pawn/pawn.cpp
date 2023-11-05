//
// Created by Adis Veletanlic on 11/2/23.
//

#include "pawn.h"
#include <iostream>
#include "../../board/board.h"

Pawn::Pawn(sf::Vector2i position, Color color) : Piece(position, color) {
    if (color == Color::BLACK) {
        LoadTexture("assets/sprites/pawn-black.png");
    } else {
        LoadTexture("assets/sprites/pawn-white.png");
    }
}

std::vector<sf::Vector2i> Pawn::AvailableMoves(Board board) const {
    std::vector<sf::Vector2i> moves;
    int direction = (GetColor() == Color::WHITE) ? -1 : 1;

    // Helper lambda to check if a position is within board bounds
    auto isWithinBounds = [](int r, int c) {
        return r >= 0 && r < 8 && c >= 0 && c < 8;
    };

    // Single square forward
    if (isWithinBounds(GetPosition().x + direction, GetPosition().y) && board.GetPieceAt(GetPosition().x + direction, GetPosition().y) == nullptr) {
        moves.emplace_back(GetPosition().x + direction, GetPosition().y);

        // Double square forward for pawns that haven't moved
        if (!hasMoved && isWithinBounds(GetPosition().x + 2 * direction, GetPosition().y) && board.GetPieceAt(GetPosition().x + 2 * direction, GetPosition().y) == nullptr) {
            moves.emplace_back(GetPosition().x + 2 * direction, GetPosition().y);
        }
    }

    // Diagonal captures
    if (isWithinBounds(GetPosition().x + direction, GetPosition().y - 1)) {
        auto leftCapture = board.GetPieceAt(GetPosition().x + direction, GetPosition().y - 1);
        if (leftCapture && leftCapture->GetColor() != this->GetColor()) {
            moves.emplace_back(GetPosition().x + direction, GetPosition().y - 1);
        }
    }

    if (isWithinBounds(GetPosition().x + direction, GetPosition().y + 1)) {
        auto rightCapture = board.GetPieceAt(GetPosition().x + direction, GetPosition().y + 1);
        if (rightCapture && rightCapture->GetColor() != this->GetColor()) {
            moves.emplace_back(GetPosition().x + direction, GetPosition().y + 1);
        }
    }

    auto lastMovedPiece = board.GetLastMovedPiece();
    auto lastMovedPiecePrevPos = board.GetLastMovedPiecePreviousPosition();

    if (lastMovedPiece && dynamic_cast<Pawn*>(lastMovedPiece.get()) && abs(lastMovedPiecePrevPos.x - GetPosition().x) == 2) {
        // Left en passant
        if (isWithinBounds(GetPosition().x + direction, GetPosition().y - 1) && GetPosition().y - 1 == lastMovedPiecePrevPos.y && GetPosition().x + direction == lastMovedPiecePrevPos.x) {
            moves.emplace_back(GetPosition().x + direction, GetPosition().y - 1);
        }

        // Right en passant
        if (isWithinBounds(GetPosition().x + direction, GetPosition().y + 1) && GetPosition().y + 1 == lastMovedPiecePrevPos.y && GetPosition().x + direction == lastMovedPiecePrevPos.x) {
            moves.emplace_back(GetPosition().x + direction, GetPosition().y + 1);
        }
    }

    return moves;
}



bool Pawn::CanMove(sf::Vector2i toPosition, const Board &board) const {
    if (GetColor() == Color::WHITE) {
        if (GetPosition().y == toPosition.y) {
            return (GetPosition().x - toPosition.x == 1) || (GetPosition().x == 6 && toPosition.x == 4);
        } else if (std::abs(GetPosition().y - toPosition.y) == 1) {
            return GetPosition().x - toPosition.x == 1;
        }
    } else {
        if (GetPosition().y == toPosition.y) {
            return (toPosition.x - GetPosition().x == 1) || (GetPosition().x == 1 && toPosition.x == 3);
        } else if (std::abs(GetPosition().y - toPosition.y) == 1) {
            return toPosition.x - GetPosition().x == 1;
        }
    }
    return false;
}

const sf::Texture &Pawn::GetTexture() const {
    return texture;
}

