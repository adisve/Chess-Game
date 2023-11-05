//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_QUEEN_H
#define CHESSGAME_QUEEN_H

class Queen : public Piece {
public:
    Queen(sf::Vector2i position, Color color);
    bool CanMove(sf::Vector2i toPosition, const Board& board) const override;
    std::vector<sf::Vector2i> AvailableMoves(Board board) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_QUEEN_H

