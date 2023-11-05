//
// Created by Adis Veletanlic on 11/4/23.
//
#include "piece.h"
#include "../board/board.h"


std::vector<sf::Vector2i> Piece::FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, Board board) const {
    std::vector<sf::Vector2i> moves;

    int i = GetPosition().x + rowIncrement;
    int j = GetPosition().y + colIncrement;
    while (i >= 0 && i < 8 && j >= 0 && j < 8) {
        if (board.GetPieceAt(i, j) == nullptr) {
            moves.emplace_back(i, j);
        } else {
            if (board.GetPieceAt(i, j)->color != color) {
                moves.emplace_back(i, j);
            }
            break;
        }
        i += rowIncrement;
        j += colIncrement;
    }
    return moves;
}