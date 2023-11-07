//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../Piece.h"

#ifndef CHESSGAME_BISHOP_H
#define CHESSGAME_BISHOP_H

class Bishop : public Piece {
public:
    Bishop(sf::Vector2i position, PlayerColor color, PieceType type);
    std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_BISHOP_H
