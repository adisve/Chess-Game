//
// Created by Adis Veletanlic on 11/2/23.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include <memory>
#include "../pieces/color.h"

#ifndef CHESSGAME_BOARD_H
#define CHESSGAME_BOARD_H

class Piece;

class Board {
public:
    using Row = std::array<std::optional<std::shared_ptr<Piece>>, 8>;
    std::array<Row, 8> board; // Holding board state

    void Draw(sf::RenderWindow& window);
    void Populate();
    bool IsMoveLegal(int fromRow, int fromCol, int toRow, int toCol);
    bool MovePiece(int fromRow, int fromCol, int toRow, int toCol);
    std::shared_ptr<Piece> GetPieceAt(int row, int col);
private:
    bool IsKingInCheck(Color playerColor);
};

#endif //CHESSGAME_BOARD_H