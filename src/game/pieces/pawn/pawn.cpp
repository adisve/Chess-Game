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

std::vector<sf::Vector2i> Pawn::AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const {
    std::vector<sf::Vector2i> moves;
    const int direction = (GetColor() == Color::White) ? -1 : 1;
    const sf::Vector2i currentPosition = GetPosition();

    // Forward moves
    AddSingleAndDoubleForwardMoves(moves, currentPosition, direction, board);

    // Diagonal captures
    AddDiagonalCaptureMoves(moves, currentPosition, direction, board);

    // En passant captures
    AddEnPassantMoves(moves, currentPosition, direction, board, lastMovedPiece, lastMovedPiecePreviousPosition);

    return moves;
}

void Pawn::AddSingleAndDoubleForwardMoves(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const {
    const sf::Vector2i oneStepForward(currentPosition.x + direction, currentPosition.y);
    if (IsMovePossible(oneStepForward, board)) {
        moves.push_back(oneStepForward);

        const sf::Vector2i twoStepsForward(currentPosition.x + 2 * direction, currentPosition.y);
        if (!hasMoved && IsMovePossible(twoStepsForward, board)) {
            moves.push_back(twoStepsForward);
        }
    }
}

void Pawn::AddDiagonalCaptureMoves(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const {
    const sf::Vector2i leftDiagonal(currentPosition.x + direction, currentPosition.y - 1);
    const sf::Vector2i rightDiagonal(currentPosition.x + direction, currentPosition.y + 1);

    AddCaptureMoveIfValid(moves, leftDiagonal, board);
    AddCaptureMoveIfValid(moves, rightDiagonal, board);
}

void Pawn::AddCaptureMoveIfValid(std::vector<sf::Vector2i>& moves, sf::Vector2i capturePosition, const Board& board) const {
    if (Board::IsWithinBounds(capturePosition) && CanCaptureAt(capturePosition, board)) {
        moves.push_back(capturePosition);
    }
}

void Pawn::AddEnPassantMoves(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, int direction, const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const {
    if (lastMovedPiece && dynamic_cast<Pawn*>(lastMovedPiece.get())) {
        if (std::abs(lastMovedPiecePreviousPosition.x - currentPosition.x) == 2) {
            AddEnPassantMoveIfValid(moves, currentPosition, lastMovedPiecePreviousPosition, direction, board);
        }
    }
}

void Pawn::AddEnPassantMoveIfValid(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, sf::Vector2i lastMovedPiecePrevPos, int direction, const Board& board) const {
    const sf::Vector2i leftEnPassant(currentPosition.x + direction, currentPosition.y - 1);
    const sf::Vector2i rightEnPassant(currentPosition.x + direction, currentPosition.y + 1);

    if (CanEnPassant(leftEnPassant, lastMovedPiecePrevPos, board)) {
        moves.push_back(leftEnPassant);
    }

    if (CanEnPassant(rightEnPassant, lastMovedPiecePrevPos, board)) {
        moves.push_back(rightEnPassant);
    }
}

bool Pawn::IsMovePossible(sf::Vector2i position, const Board& board) {
    return Board::IsWithinBounds(position) && board.GetPieceAt({position.x, position.y}) == nullptr;
}

bool Pawn::CanCaptureAt(sf::Vector2i position, const Board& board) const {
    auto piece = board.GetPieceAt({position.x, position.y});
    return piece && piece->GetColor() != this->GetColor();
}

bool Pawn::CanEnPassant(sf::Vector2i enPassantPosition, sf::Vector2i lastMovedPiecePrevPos, const Board& board) const {
    return Board::IsWithinBounds(enPassantPosition) &&
           GetPosition().y - 1 == lastMovedPiecePrevPos.y &&
           CanCaptureAt(enPassantPosition, board);
}

const sf::Texture &Pawn::GetTexture() const {
    return texture;
}

