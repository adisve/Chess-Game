//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_BISHOP_H
#define CHESSGAME_BISHOP_H

class Bishop : public Piece {
public:
    Bishop(int row, int col, Color color);  // Explicit constructor declaration
    bool CanMove(int toRow, int toCol, const Board& board) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_BISHOP_H
