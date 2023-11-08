//
// Created by Adis Veletanlic on 2023-11-06.
//

#include "Player.h"
#include "../Board/Board.h"

void Player::SelectPiece(Position position, const Board& board, const std::optional<Move>& lastMove)  {
    if (Board::IsWithinBounds(position)) {
        this->selectedPiecePosition = position;
        auto pieceAtPosition = board.GetPieceAt(position);
        if (pieceAtPosition) {
            this->availableMoves = pieceAtPosition->AvailableMoves(board, lastMove);
        }
    }
}

Position Player::GetSelectedPiecePosition() {
    return this->selectedPiecePosition;
}

std::vector<Move> Player::GetAvailableMoves() {
    return availableMoves;
}

void Player::DeselectPiece() {
    selectedPiecePosition = { -1, -1 };
    availableMoves.clear();
}

PlayerColor Player::GetColor() {
    return this->color;
}

std::optional<std::shared_ptr<Piece>> Player::GetSelectedPiece(const Board& board) const {
    return board.GetPieceAt(this->selectedPiecePosition);
}

bool Player::HasSelectedPiece() const {
    return this->selectedPiecePosition.x != -1 && this->selectedPiecePosition.y != -1;
}

std::optional<Move> Player::CanMovePieceTo(const Position& position) const {
    auto moveIt = std::find_if(availableMoves.begin(), availableMoves.end(),
                               [position](const Move& move) {
                                   return move.moveToDirection == position || move.attackingDirection == position;
                               });

    if (moveIt != availableMoves.end()) {
        return *moveIt;
    } else {
        return std::nullopt;
    }
}
