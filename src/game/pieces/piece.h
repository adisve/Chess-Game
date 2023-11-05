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

class Piece {
private:

    sf::Vector2i position;
    Color color;

public:
    Piece(sf::Vector2i position, Color color) : position(position), color(color) {}
    std::vector<sf::Vector2i> FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, Board board) const;
    virtual ~Piece() = default;
    sf::Sprite sprite;

    virtual bool CanMove(sf::Vector2i toPosition, const Board& board) const = 0;
    virtual std::vector<sf::Vector2i> AvailableMoves(Board board) const = 0;
    virtual const sf::Texture& GetTexture() const = 0;

    [[nodiscard]] sf::Vector2i GetPosition() const {
        return position;
    }

    Color GetColor() const {
        return color;
    }

    void SetPosition(const sf::Vector2i& newPosition) {
        position = newPosition;
    }

protected:
    void LoadTexture(const std::string& path) {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Error loading texture: " << path << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setOrigin((float)texture.getSize().x / 2, (float)texture.getSize().y / 2);
        sprite.setPosition((float)position.y * 100 + 50, (float)position.x * 100 + 50);
    }

    sf::Texture texture;
};

#endif
