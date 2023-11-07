//
// Created by Adis Veletanlic on 11/2/23.
//

#pragma once
#include "PlayerColor.h"
#include "../Player/Move.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#ifndef CHESSGAME_PIECE_H
#define CHESSGAME_PIECE_H

class Board;

using Position = sf::Vector2i;

enum class PieceType {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

class Piece {
private:
    PieceType type;
    Position position;
    PlayerColor color;

public:

    virtual ~Piece() = default;

    Piece(sf::Vector2i position, PlayerColor color, PieceType type) : position(position), color(color), type(type) {}

    std::vector<Move> FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, const Board& board) const;

    sf::Sprite sprite;

    virtual std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const = 0;

    const sf::Texture& GetTexture() const;

    [[nodiscard]] Position GetPosition() const;

    PlayerColor GetColor() const;

    void SetPosition(const Position& newPosition);

    PieceType GetType() const;

protected:

    void LoadTexture(const std::string& path);

    sf::Texture texture;
};

#endif
