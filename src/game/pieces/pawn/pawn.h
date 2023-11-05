//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_PAWN_H
#define CHESSGAME_PAWN_H

class Pawn : public Piece {
public:
    Pawn(sf::Vector2i position, Color color, PieceType type);
    bool hasMoved = false;

    std::vector<sf::Vector2i> AvailableMoves(const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const override;
    const sf::Texture& GetTexture() const override;
    void AddSingleAndDoubleForwardMoves(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const;

    void AddDiagonalCaptureMoves(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const;
    void AddCaptureMoveIfValid(std::vector<sf::Vector2i>& moves, sf::Vector2i capturePosition, const Board& board) const;
    void AddEnPassantMoves(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, int direction, const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, sf::Vector2i lastMovedPiecePreviousPosition) const;
    void AddEnPassantMoveIfValid(std::vector<sf::Vector2i>& moves, sf::Vector2i currentPosition, sf::Vector2i lastMovedPiecePrevPos, int direction, const Board& board) const;

    static bool IsMovePossible(sf::Vector2i position, const Board& board) ;
    bool CanCaptureAt(sf::Vector2i position, const Board& board) const;
    bool CanEnPassant(sf::Vector2i enPassantPosition, sf::Vector2i lastMovedPiecePrevPos, const Board& board) const;
};

#endif //CHESSGAME_PAWN_H

