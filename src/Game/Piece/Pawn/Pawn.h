//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../Piece.h"

#ifndef CHESSGAME_PAWN_H
#define CHESSGAME_PAWN_H

class Pawn : public Piece {
public:
    Pawn(Position position, PlayerColor color, PieceType type);

    std::vector<Move> AvailableMoves(const Board& board, const std::optional<Move>& lastMove) const override;

    void AddSingleAndDoubleForwardMoves(std::vector<Move>& moves, Position currentPosition, int direction, const Board& board) const;

    void AddDiagonalCaptureMoves(std::vector<Move>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const;

    void AddCaptureMoveIfValid(std::vector<Move>& moves, Position currentPosition, Position capturePosition, const Board& board) const;

    void AddEnPassantMoves(std::vector<Move>& moves, Position currentPosition, int direction, const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, const Move& lastMove) const;

    void AddEnPassantMoveIfValid(std::vector<Move>& moves, Position currentPosition, const Move& lastMove, int direction, const Board& board) const;

    static bool IsMovePossible(Position position, const Board& board);

    bool CanCaptureAt(Position position, const Board& board) const;

    bool CanPromote(const Move& move) const;

    bool CanEnPassant(Position enPassantPosition, int direction, const Move& lastMove, const Board& board) const;

    void SetHasMoved();

    bool GetHasMoved() const;

private:
    bool hasMoved = false;
};

#endif //CHESSGAME_PAWN_H

