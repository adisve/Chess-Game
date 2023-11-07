//
// Created by Adis Veletanlic on 11/4/23.
//
#include "Piece.h"
#include "../Board/Board.h"

const int SQUARE_SIZE = 100;

std::vector<Move> Piece::FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, const Board& board) const {
    std::vector<Move> moves;

    int i = GetPosition().x + rowIncrement;
    int j = GetPosition().y + colIncrement;
    while (Board::IsWithinBounds({i, j})) {
        if (board.GetPieceAt({i, j}) == nullptr) {
            sf::Vector2i movePosition = {i, j};
            moves.emplace_back(movePosition, movePosition, this->GetPosition());
        } else {
            if (board.GetPieceAt({i, j})->color != color) {
                sf::Vector2i attackPosition = {i, j};
                sf::Vector2i movePosition = {i, j};
                moves.emplace_back(movePosition, attackPosition, this->GetPosition());
            }
            break;
        }
        i += rowIncrement;
        j += colIncrement;
    }
    return moves;
}

void Piece::SetPosition(const Position& newPosition) {
    this->position = newPosition;
    this->sprite.setPosition((float)newPosition.x * SQUARE_SIZE + 50, (float)newPosition.y * SQUARE_SIZE + 50);
}

void Piece::LoadTexture(const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cout << "Error loading texture from path: " << path << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setOrigin((float)texture.getSize().x / 2, (float)texture.getSize().y / 2);
    sprite.setPosition((float)position.x * SQUARE_SIZE + 50, (float)position.y * SQUARE_SIZE + 50);
}

PieceType Piece::GetType() const {
    return type;
}

Position Piece::GetPosition() const {
    return position;
}

PlayerColor Piece::GetColor() const {
    return color;
}

const sf::Texture& Piece::GetTexture() const {
    return texture;
}