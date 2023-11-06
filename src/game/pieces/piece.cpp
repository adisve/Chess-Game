//
// Created by Adis Veletanlic on 11/4/23.
//
#include "piece.h"
#include "../board/board.h"


std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> Piece::FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, const Board& board) const {
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> moves;

    int i = GetPosition().x + rowIncrement;
    int j = GetPosition().y + colIncrement;
    while (Board::IsWithinBounds({i, j})) {
        if (board.GetPieceAt({i, j}) == nullptr) {
            sf::Vector2i movePosition = {i, j};
            moves.emplace_back(movePosition, movePosition);
        } else {
            if (board.GetPieceAt({i, j})->color != color) {
                sf::Vector2i attackPosition = {i, j};
                sf::Vector2i movePosition = {i, j};
                moves.emplace_back(attackPosition, movePosition);
            }
            break;
        }
        i += rowIncrement;
        j += colIncrement;
    }
    return moves;
}

