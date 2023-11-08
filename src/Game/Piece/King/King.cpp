//
// Created by Adis Veletanlic on 11/2/23.
//

#include "King.h"
#include <iostream>
#include "../../Board/Board.h"
#include "../Rook/Rook.h"

King::King(sf::Vector2i position, PlayerColor color, PieceType type) : Piece(position, color, type) {
    if (color == PlayerColor::Black) {
        LoadTexture("Assets/Sprites/King-black.png");
    } else {
        LoadTexture("Assets/Sprites/King-white.png");
    }
}

std::vector<Move> King::AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const {
    std::vector<Move> moves;

    std::vector<Position> potentialMoves = {
            { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 },
            { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }
    };

    for (const auto& move : potentialMoves) {
        int newX = GetPosition().x + move.x;
        int newY = GetPosition().y + move.y;

        if (Board::IsWithinBounds({newX, newY})) {
            std::shared_ptr<Piece> pieceAtDestination = board.GetPieceAt({newX, newY});

            if (!pieceAtDestination || pieceAtDestination->GetColor() != this->GetColor()) {
                Position attackPosition = {newX, newY};
                Position movePosition = {newX, newY};
                moves.emplace_back(movePosition, attackPosition, this->GetPosition());
            }
        }
    }

    if (!this->HasMoved()) {
        // Positions for castling rooks
        std::vector<Position> rookPositions;
        if (this->GetColor() == PlayerColor::White) {
            rookPositions = { {0, 7}, {7, 7} };
        } else {
            rookPositions = { {0, 0}, {7, 0} };
        }

        for (const auto& rookPos : rookPositions) {
            std::shared_ptr<Piece> rook = board.GetPieceAt(rookPos);
            // Check if the piece is a rook and if it has moved
            if (rook && rook->GetType() == PieceType::Rook && !std::static_pointer_cast<Rook>(rook)->HasMoved()) {
                // Check if the squares between the king and the rook are empty
                bool pathClear = true;
                int direction = (rookPos.x < GetPosition().x) ? -1 : 1;
                for (int x = GetPosition().x + direction; x != rookPos.x; x += direction) {
                    if (board.GetPieceAt({x, GetPosition().y})) {
                        pathClear = false;
                        break;
                    }
                }
                if (pathClear) {
                    Position castlingMove = {GetPosition().x + 2 * direction, GetPosition().y};
                    moves.emplace_back(castlingMove, castlingMove, this->GetPosition());
                }
            }
        }
    }

    return moves;
}

bool King::HasMoved() const {
    return hasMoved;
}

void King::SetHasMoved() {
    hasMoved = true;
}