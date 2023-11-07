//
// Created by Adis Veletanlic on 11/2/23.
//

#include "Pawn.h"
#include <iostream>
#include "../../Board/Board.h"

Pawn::Pawn(sf::Vector2i position, PlayerColor color, PieceType type) : Piece(position, color, type) {
    if (color == PlayerColor::Black) {
        LoadTexture("assets/sprites/Pawn-black.png");
    } else {
        LoadTexture("assets/sprites/Pawn-white.png");
    }
}

std::vector<Move> Pawn::AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const {

    std::vector<Move> moves;
    const int direction = (GetColor() == PlayerColor::White) ? -1 : 1;
    const sf::Vector2i currentPosition = GetPosition();

    AddSingleAndDoubleForwardMoves(moves, currentPosition, direction, board);
    AddDiagonalCaptureMoves(moves, currentPosition, direction, board);

    if (lastMove.has_value()) {
        const auto& lastMovedPiece = board.GetPieceAt(lastMove->attackingDirection);
        AddEnPassantMoves(moves, currentPosition, direction, board, lastMovedPiece, lastMove.value());
    }

    return moves;
}

void Pawn::AddSingleAndDoubleForwardMoves(std::vector<Move>& moves, Position currentPosition, int direction, const Board& board) const {
    const sf::Vector2i oneStepForward(currentPosition.x, currentPosition.y + direction);
    if (IsMovePossible(oneStepForward, board)) {
        moves.emplace_back(oneStepForward, oneStepForward, currentPosition);

        const sf::Vector2i twoStepsForward(currentPosition.x, currentPosition.y + 2 * direction);
        if (!hasMoved && IsMovePossible(twoStepsForward, board)) {
            moves.emplace_back(twoStepsForward, twoStepsForward, currentPosition);
        }
    }
}

void Pawn::AddDiagonalCaptureMoves(std::vector<Move>& moves, Position currentPosition, int direction, const Board& board) const {
    const sf::Vector2i leftDiagonal(currentPosition.x - 1, currentPosition.y + direction);
    const sf::Vector2i rightDiagonal(currentPosition.x + 1, currentPosition.y + direction);

    AddCaptureMoveIfValid(moves, currentPosition, leftDiagonal, board);
    AddCaptureMoveIfValid(moves, currentPosition, rightDiagonal, board);
}

void Pawn::AddCaptureMoveIfValid(std::vector<Move>& moves, Position currentPosition, Position capturePosition, const Board& board) const {
    if (Board::IsWithinBounds(capturePosition) && CanCaptureAt(capturePosition, board)) {
        moves.emplace_back(capturePosition, capturePosition, currentPosition);
    }
}

void Pawn::AddEnPassantMoves(std::vector<Move>& moves, Position currentPosition, int direction, const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, const Move& lastMove) const {
    auto lastMoveFromPos = lastMove.moveFromDirection;
    if (lastMovedPiece && dynamic_cast<Pawn*>(lastMovedPiece.get())) {
        if (std::abs(lastMoveFromPos.y - currentPosition.y) == 2) { // If last moved Pawn moved two steps forward
            AddEnPassantMoveIfValid(moves, currentPosition, lastMove, direction, board);
        }
    }
}

void Pawn::AddEnPassantMoveIfValid(std::vector<Move>& moves, Position currentPosition, const Move& lastMove, int direction, const Board& board) const {
    const sf::Vector2i leftEnPassant(currentPosition.x - 1, currentPosition.y + direction);
    const sf::Vector2i rightEnPassant(currentPosition.x + 1, currentPosition.y + direction);

    const sf::Vector2i leftEnPassantAttackPosition(currentPosition.x - 1, currentPosition.y);
    const sf::Vector2i rightEnPassantAttackPosition(currentPosition.x + 1, currentPosition.y);

    if (CanEnPassant(leftEnPassant, direction, lastMove, board)) {
        moves.emplace_back(leftEnPassant, leftEnPassantAttackPosition, currentPosition);
    }

    if (CanEnPassant(rightEnPassant, direction, lastMove, board)) {
        moves.emplace_back(rightEnPassant, rightEnPassantAttackPosition, currentPosition);
    }
}

bool Pawn::IsMovePossible(Position position, const Board& board) {
    return Board::IsWithinBounds(position) && board.GetPieceAt({position.x, position.y}) == nullptr;
}

bool Pawn::CanCaptureAt(Position position, const Board& board) const {
    auto piece = board.GetPieceAt({position.x, position.y});
    return piece && piece->GetColor() != this->GetColor();
}

bool Pawn::CanEnPassant(Position enPassantPosition, int direction, const Move& lastMove, const Board& board) const {

    auto lastMoveTo = lastMove.moveToDirection;
    auto lastMoveFrom = lastMove.moveFromDirection;

    if (board.GetPieceAt(lastMoveTo)->GetType() != PieceType::Pawn ||
        abs(lastMoveFrom.y - lastMoveTo.y) != 2) {
        return false;
    }

    // Your Pawn must be on the correct rank to perform en passant (5th for white, 4th for black)
    int enPassantRank = (GetColor() == PlayerColor::White) ? 3 : 4; // Adjust these rank numbers according to your Board's coordinate system
    if (GetPosition().y != enPassantRank) {
        return false;
    }

    // The enemy Pawn must be adjacent to your Pawn horizontally
    if (lastMoveTo.x != enPassantPosition.x ||
            lastMoveTo.y != GetPosition().y) {
        return false;
    }

    // Check if the en passant move leads to a capture position behind the enemy Pawn
    if (enPassantPosition != sf::Vector2i(lastMoveTo.x, GetPosition().y + direction)) {
        return false;
    }

    // If all conditions are met, return true.
    return true;
}




