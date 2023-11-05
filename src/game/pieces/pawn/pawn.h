//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_PAWN_H
#define CHESSGAME_PAWN_H

class Pawn : public Piece {
public:
    Pawn(sf::Vector2i position, Color color);
    bool hasMoved = false;

    bool CanMove(sf::Vector2i toPosition, const Board& board) const override;
    std::vector<sf::Vector2i> AvailableMoves(Board board) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_PAWN_H

