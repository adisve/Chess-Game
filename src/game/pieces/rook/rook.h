//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_ROOK_H
#define CHESSGAME_ROOK_H

class Rook : public Piece {
public:
    Rook(int row, int col, Color color);
    bool CanMove(int toRow, int toCol, const Board& board) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_ROOK_H
