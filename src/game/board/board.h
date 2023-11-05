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
#include "../pieces/piece.h"

#ifndef CHESSGAME_BOARD_H
#define CHESSGAME_BOARD_H

class Piece;

class Board {
private:
    using Row = std::array<std::optional<std::shared_ptr<Piece>>, 8>;
    std::array<Row, 8> board;

public:

    void Draw(sf::RenderWindow& window, const sf::Vector2i& selectedPosition, const std::vector<sf::Vector2i>& availableMoves);
    void Populate();
    void UpdateBoardPosition(const sf::Vector2i& position, std::shared_ptr<Piece> piece) {
        board[position.x][position.y] = std::move(piece);
    }
    [[nodiscard]] std::shared_ptr<Piece> GetPieceAt(sf::Vector2i position) const;
    [[nodiscard]] static bool IsWithinBounds(sf::Vector2i position) {
        return position.x >= 0 && position.x < 8 && position.y >= 0 && position.y < 8;
    }


private:
    void DrawAvailableMoves(sf::RenderWindow &window, const std::vector<sf::Vector2i> &availableMoves, const std::shared_ptr<Piece>& selectedPiece) const;
    [[nodiscard]] static sf::Color DetermineSquareColor(sf::Vector2i position, sf::Vector2i selectedPosition);
};

#endif //CHESSGAME_BOARD_H