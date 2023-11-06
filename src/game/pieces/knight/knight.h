//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_KNIGHT_H
#define CHESSGAME_KNIGHT_H

class Knight : public Piece {
public:
    Knight(sf::Vector2i position, Color color, PieceType type);
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> AvailableMoves(const Board& board, const std::optional<std::tuple<sf::Vector2i, sf::Vector2i>>& lastMove) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_KNIGHT_H

