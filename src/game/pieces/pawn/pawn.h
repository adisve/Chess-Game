//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_PAWN_H
#define CHESSGAME_PAWN_H

class Pawn : public Piece {
public:
    Pawn(int row, int col, Color color);
    bool hasMoved = false;

    bool CanMove(int toRow, int toCol, const Board& board) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_PAWN_H

