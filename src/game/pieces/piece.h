//
// Created by Adis Veletanlic on 11/2/23.
//

#pragma once
#include "color.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#ifndef CHESSGAME_PIECE_H
#define CHESSGAME_PIECE_H

class Board;

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
    sf::Vector2i position;
    Color color;

public:
    Piece(sf::Vector2i position, Color color, PieceType type) : position(position), color(color), type(type) {}
    std::vector<sf::Vector2i> FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, const Board& board) const;
    virtual ~Piece() = default;
    sf::Sprite sprite;

    bool CanThreatenKing(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition, const Board& board) const;
    virtual std::vector<sf::Vector2i> AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const = 0;
    virtual const sf::Texture& GetTexture() const = 0;

    bool CanPromote(const sf::Vector2i& move) const {
        switch (color) {
            case Color::White:
                return type == PieceType::Pawn && move.x == 0;
            case Color::Black:
                return type == PieceType::Pawn && move.x == 7;
        }
    }

    [[nodiscard]] sf::Vector2i GetPosition() const {
        return position;
    }

    Color GetColor() const {
        return color;
    }

    void SetPosition(const sf::Vector2i& newPosition) {
        std::cout << "Setting position to: " << newPosition.x << ", " << newPosition.y << std::endl;
        position = newPosition;
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
