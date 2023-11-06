//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_QUEEN_H
#define CHESSGAME_QUEEN_H

class Queen : public Piece {
public:
    Queen(sf::Vector2i position, Color color, PieceType type);
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> AvailableMoves(const Board& board, const std::optional<std::tuple<sf::Vector2i, sf::Vector2i>>& lastMove) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_QUEEN_H

