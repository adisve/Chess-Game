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
public:
    int row;
    int col;
    Color color;

    Piece(int row, int col, Color color) : row(row), col(col), color(color) {}
    std::vector<sf::Vector2i> FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, Board board) const;
    virtual ~Piece() = default;
    virtual bool CanMove(int toRow, int toCol, const Board& board) const = 0;
    virtual std::vector<sf::Vector2i> AvailableMoves(Board board) const = 0;
    virtual const sf::Texture& GetTexture() const = 0;
    sf::Sprite sprite;

protected:
    void LoadTexture(const std::string& path) {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Error loading texture: " << path << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
        sprite.setPosition(col * 100 + 50, row * 100 + 50);
    }

    sf::Texture texture;
};

#endif
