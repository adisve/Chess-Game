//
// Created by Adis Veletanlic on 11/2/23.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include <memory>
#include <utility>
#include "../pieces/color.h"

#ifndef CHESSGAME_BOARD_H
#define CHESSGAME_BOARD_H

class Piece;

class Board {
public:
    using Row = std::array<std::optional<std::shared_ptr<Piece>>, 8>;
    std::array<Row, 8> board; // Holding board state
    std::shared_ptr<Piece> lastMovedPiece; // Pointer to the last moved piece
    sf::Vector2i lastMovedPiecePreviousPosition;
    std::shared_ptr<Piece> selectedPiece = nullptr;
    sf::Vector2i selectedPosition = {-1, -1};

    void Draw(sf::RenderWindow& window);
    void Populate();
    bool IsMoveLegal(int fromRow, int fromCol, int toRow, int toCol);
    bool MovePiece(int fromRow, int fromCol, int toRow, int toCol);
    [[nodiscard]] std::vector<sf::Vector2i> GetAvailableMovesForSelectedPiece() const;
    std::shared_ptr<Piece> GetPieceAt(int row, int col);

private:
    bool IsKingInCheck(Color playerColor);
    void DrawAvailableMoves(sf::RenderWindow& window, const std::vector<sf::Vector2i>& availableMoves, int row, int col, int squareSize) const;
    [[nodiscard]] sf::Color DetermineSquareColor(int row, int col) const;
};

#endif //CHESSGAME_BOARD_H