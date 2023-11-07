//
// Created by Adis Veletanlic on 11/4/23.
//
#include "Piece.h"
#include "../Board/Board.h"


std::vector<Move> Piece::FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, const Board& board) const {
    std::vector<Move> moves;

    int i = GetPosition().x + rowIncrement;
    int j = GetPosition().y + colIncrement;
    while (Board::IsWithinBounds({i, j})) {
        if (board.GetPieceAt({i, j}) == nullptr) {
            sf::Vector2i movePosition = {i, j};
            moves.emplace_back(movePosition, movePosition, this->GetPosition());
        } else {
            if (board.GetPieceAt({i, j})->color != color) {
                sf::Vector2i attackPosition = {i, j};
                sf::Vector2i movePosition = {i, j};
                moves.emplace_back(movePosition, attackPosition, this->GetPosition());
            }
            break;
        }
        i += rowIncrement;
        j += colIncrement;
    }
    return moves;
}

