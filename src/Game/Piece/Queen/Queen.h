//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../Piece.h"

#ifndef CHESSGAME_QUEEN_H
#define CHESSGAME_QUEEN_H

class Queen : public Piece {
public:

    Queen(sf::Vector2i position, PlayerColor color, PieceType type);

    std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const override;
};

#endif //CHESSGAME_QUEEN_H

