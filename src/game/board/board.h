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
private:
    using Row = std::array<std::optional<std::shared_ptr<Piece>>, 8>;
    std::vector<std::shared_ptr<Piece>> capturedPieces;
    std::array<Row, 8> board; // Holding board state
    std::shared_ptr<Piece> lastMovedPiece; // Pointer to the last moved piece
    std::shared_ptr<Piece> selectedPiece = nullptr;
    sf::Vector2i lastMovedPiecePreviousPosition;
    sf::Vector2i selectedPosition = {-1, -1};
public:

    void Draw(sf::RenderWindow& window);
    void Populate();
    void MoveSelectedPiece(int toRow, int toCol);
    [[nodiscard]] std::vector<sf::Vector2i> GetAvailableMovesForSelectedPiece() const;
    std::shared_ptr<Piece> GetPieceAt(int row, int col);

    [[nodiscard]] const std::vector<std::shared_ptr<Piece>>& GetCapturedPieces() const {
        return capturedPieces;
    }

    [[nodiscard]] const std::array<Row, 8>& GetBoard() const {
        return board;
    }

    [[nodiscard]] std::shared_ptr<Piece> GetLastMovedPiece() const {
        return lastMovedPiece;
    }

    [[nodiscard]] std::shared_ptr<Piece> GetSelectedPiece() const {
        return selectedPiece;
    }

    [[nodiscard]] sf::Vector2i GetLastMovedPiecePreviousPosition() const {
        return lastMovedPiecePreviousPosition;
    }

    [[nodiscard]] sf::Vector2i GetSelectedPosition() const {
        return selectedPosition;
    }

    // Setters for the private variables
    void SetLastMovedPiece(const std::shared_ptr<Piece>& piece) {
        lastMovedPiece = piece;
    }

    void SetSelectedPiece(const std::shared_ptr<Piece>& piece) {
        selectedPiece = piece;
    }

    void SetLastMovedPiecePreviousPosition(const sf::Vector2i& position) {
        lastMovedPiecePreviousPosition = position;
    }

    void SetSelectedPosition(const sf::Vector2i& position) {
        selectedPosition = position;
    }

private:
    void DrawAvailableMoves(sf::RenderWindow &window, const std::vector<sf::Vector2i> &availableMoves);
    [[nodiscard]] sf::Color DetermineSquareColor(int row, int col) const;
};

#endif //CHESSGAME_BOARD_H