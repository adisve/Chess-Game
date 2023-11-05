//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_BISHOP_H
#define CHESSGAME_BISHOP_H

class Bishop : public Piece {
public:
    Bishop(sf::Vector2i position, Color color, PieceType type);
    std::vector<sf::Vector2i> AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_BISHOP_H
