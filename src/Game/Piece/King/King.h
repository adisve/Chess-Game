//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../Piece.h"

#ifndef CHESSGAME_KING_H
#define CHESSGAME_KING_H

class King : public Piece {
public:
    King(sf::Vector2i position, PlayerColor color, PieceType type);
    std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_KING_H