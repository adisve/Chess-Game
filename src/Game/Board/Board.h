//
// Created by Adis Veletanlic on 11/2/23.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include <memory>
#include <utility>
#include "../Piece/Piece.h"
#include "../Player/Move.h"

#ifndef CHESSGAME_BOARD_H
#define CHESSGAME_BOARD_H

class Piece;

class Board {
private:
    using Row = std::array<std::optional<std::shared_ptr<Piece>>, 8>;
    std::array<Row, 8> board;
public:

    void DrawBoard(sf::RenderWindow& window) const;

    void Populate();

    [[nodiscard]] std::shared_ptr<Piece> GetPieceAt(sf::Vector2i position) const;

    [[nodiscard]] static bool IsWithinBounds(Position position) {
        return position.x >= 0 && position.x < 8 && position.y >= 0 && position.y < 8;
    }

    void DrawAvailableMoves(sf::RenderWindow &window, const std::vector<Move>& availableMoves, const std::shared_ptr<Piece>& selectedPiece) const;

    void Clear();

    void SetPieceAt(Position position, const std::shared_ptr<Piece>& piece);

private:
    [[nodiscard]] static sf::Color DetermineSquareColor(sf::Vector2i position);
};

#endif //CHESSGAME_BOARD_H