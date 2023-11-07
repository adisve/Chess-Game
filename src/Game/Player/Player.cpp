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
    //std::cout << "Trying to get piece at (" << this->selectedPiecePosition.x << ", " << this->selectedPiecePosition.y << ")" << std::endl;
    return board.GetPieceAt(this->selectedPiecePosition);
}

bool Player::HasSelectedPiece() const {
    return this->selectedPiecePosition.x != -1 && this->selectedPiecePosition.y != -1;
}