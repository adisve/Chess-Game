//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../Piece.h"

#ifndef CHESSGAME_ROOK_H
#define CHESSGAME_ROOK_H

class Rook : public Piece {
public:

    Rook(sf::Vector2i position, PlayerColor color, PieceType type);

    std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const override;

    bool HasMoved() const;

    void SetHasMoved();

private:

    bool hasMoved = false;
};

#endif //CHESSGAME_ROOK_H
