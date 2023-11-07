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
    Piece(sf::Vector2i position, PlayerColor color, PieceType type) : position(position), color(color), type(type) {}
    std::vector<Move> FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, const Board& board) const;
    virtual ~Piece() = default;
    sf::Sprite sprite;

    virtual std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const = 0;

    virtual const sf::Texture& GetTexture() const = 0;

    bool CanPromote(const sf::Vector2i& move) const {
        switch (color) {
            case PlayerColor::White:
                return type == PieceType::Pawn && move.y == 0;
            case PlayerColor::Black:
                return type == PieceType::Pawn && move.y == 7;
        }
    }

    [[nodiscard]] sf::Vector2i GetPosition() const {
        return position;
    }

    PlayerColor GetColor() const {
        return color;
    }

    void SetPosition(const Position& newPosition) {
        position = newPosition;
        sprite.setPosition((float)position.x * 100 + 50, (float)position.y * 100 + 50);
    }

    PieceType GetType() const {
        return type;
    }

protected:
    void LoadTexture(const std::string& path) {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Error loading texture: " << path << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setOrigin((float)texture.getSize().x / 2, (float)texture.getSize().y / 2);
        sprite.setPosition((float)position.x * 100 + 50, (float)position.y * 100 + 50);
    }

    sf::Texture texture;
};

#endif
