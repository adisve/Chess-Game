//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_KING_H
#define CHESSGAME_KING_H

class King : public Piece {
public:
    King(sf::Vector2i position, Color color);
    bool CanMove(sf::Vector2i toPosition, const Board& board) const override;
    std::vector<sf::Vector2i> AvailableMoves(Board board) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_KING_H