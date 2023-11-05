//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_ROOK_H
#define CHESSGAME_ROOK_H

class Rook : public Piece {
public:
    Rook(sf::Vector2i position, Color color, PieceType type);
    std::vector<sf::Vector2i> AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const override;
    const sf::Texture& GetTexture() const override;
};

#endif //CHESSGAME_ROOK_H
