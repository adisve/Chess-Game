//
// Created by Adis Veletanlic on 11/2/23.
//

#include "pawn.h"
#include <iostream>
#include "../../board/board.h"

Pawn::Pawn(sf::Vector2i position, Color color, PieceType type) : Piece(position, color, type) {
    if (color == Color::Black) {
        LoadTexture("assets/sprites/pawn-black.png");
    } else {
        LoadTexture("assets/sprites/pawn-white.png");
    }
}

std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> Pawn::AvailableMoves(const Board& board, const std::optional<std::tuple<sf::Vector2i, sf::Vector2i>>& lastMove) const {

    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> moves;
    const int direction = (GetColor() == Color::White) ? -1 : 1;
    const sf::Vector2i currentPosition = GetPosition();

    AddSingleAndDoubleForwardMoves(moves, currentPosition, direction, board);
    AddDiagonalCaptureMoves(moves, currentPosition, direction, board);

    if (lastMove.has_value()) {
        const auto& lastMovedPiece = board.GetPieceAt(std::get<1>(lastMove.value()));
        AddEnPassantMoves(moves, currentPosition, direction, board, lastMovedPiece, lastMove.value());
    }

    return moves;
}

void Pawn::AddSingleAndDoubleForwardMoves(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const {
    const sf::Vector2i oneStepForward(currentPosition.x, currentPosition.y + direction);
    if (IsMovePossible(oneStepForward, board)) {
        moves.emplace_back(oneStepForward, oneStepForward);

        const sf::Vector2i twoStepsForward(currentPosition.x, currentPosition.y + 2 * direction);
        if (!hasMoved && IsMovePossible(twoStepsForward, board)) {
            moves.emplace_back(twoStepsForward, twoStepsForward);
        }
    }
}

void Pawn::AddDiagonalCaptureMoves(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const {
    const sf::Vector2i leftDiagonal(currentPosition.x - 1, currentPosition.y + direction);
    const sf::Vector2i rightDiagonal(currentPosition.x + 1, currentPosition.y + direction);

    AddCaptureMoveIfValid(moves, leftDiagonal, board);
    AddCaptureMoveIfValid(moves, rightDiagonal, board);
}

void Pawn::AddCaptureMoveIfValid(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i capturePosition, const Board& board) const {
    if (Board::IsWithinBounds(capturePosition) && CanCaptureAt(capturePosition, board)) {
        moves.emplace_back(capturePosition, capturePosition);
    }
}

void Pawn::AddEnPassantMoves(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, int direction, const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, const std::tuple<sf::Vector2i, sf::Vector2i>& lastMove) const {
    auto lastMoveFromPos = std::get<0>(lastMove);
    auto lastMoveToPos = std::get<1>(lastMove);
    if (lastMovedPiece && dynamic_cast<Pawn*>(lastMovedPiece.get())) {
        if (std::abs(lastMoveFromPos.y - lastMoveToPos.y) == 2) { // If last moved Pawn moved two steps forward
            AddEnPassantMoveIfValid(moves, currentPosition, lastMove, direction, board);
        }
    }
}

void Pawn::AddEnPassantMoveIfValid(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, const std::tuple<sf::Vector2i, sf::Vector2i>& lastMove, int direction, const Board& board) const {
    const sf::Vector2i leftEnPassant(currentPosition.x - 1, currentPosition.y + direction);
    const sf::Vector2i rightEnPassant(currentPosition.x + 1, currentPosition.y + direction);

    const sf::Vector2i leftEnPassantAttackPosition(currentPosition.x - 1, currentPosition.y);
    const sf::Vector2i rightEnPassantAttackPosition(currentPosition.x + 1, currentPosition.y);

    if (CanEnPassant(leftEnPassant, direction, lastMove, board)) {
        moves.emplace_back(leftEnPassant, leftEnPassantAttackPosition);
    }

    if (CanEnPassant(rightEnPassant, direction, lastMove, board)) {
        moves.emplace_back(rightEnPassant, rightEnPassantAttackPosition);
    }
}

bool Pawn::IsMovePossible(sf::Vector2i position, const Board& board) {
    return Board::IsWithinBounds(position) && board.GetPieceAt({position.x, position.y}) == nullptr;
}

bool Pawn::CanCaptureAt(sf::Vector2i position, const Board& board) const {
    auto piece = board.GetPieceAt({position.x, position.y});
    return piece && piece->GetColor() != this->GetColor();
}

bool Pawn::CanEnPassant(sf::Vector2i enPassantPosition, int direction, const std::tuple<sf::Vector2i, sf::Vector2i>& lastMove, const Board& board) const {
    auto [lastMoveStart, lastMoveEnd] = lastMove;

    if (board.GetPieceAt(lastMoveEnd)->GetType() != PieceType::Pawn ||
        abs(lastMoveStart.y - lastMoveEnd.y) != 2) {
        return false;
    }

    // Your pawn must be on the correct rank to perform en passant (5th for white, 4th for black)
    int enPassantRank = (GetColor() == Color::White) ? 3 : 4; // Adjust these rank numbers according to your board's coordinate system
    if (GetPosition().y != enPassantRank) {
        return false;
    }

    // The enemy pawn must be adjacent to your pawn horizontally
    if (lastMoveEnd.x != enPassantPosition.x ||
        lastMoveEnd.y != GetPosition().y) {
        return false;
    }

    // Check if the en passant move leads to a capture position behind the enemy pawn
    if (enPassantPosition != sf::Vector2i(lastMoveEnd.x, GetPosition().y + direction)) {
        return false;
    }

    // If all conditions are met, return true.
    return true;
}




