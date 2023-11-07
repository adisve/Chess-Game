//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../Piece.h"

#ifndef CHESSGAME_KNIGHT_H
#define CHESSGAME_KNIGHT_H

class Knight : public Piece {
public:

    Knight(sf::Vector2i position, PlayerColor color, PieceType type);

    std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const override;
};

#endif //CHESSGAME_KNIGHT_H

