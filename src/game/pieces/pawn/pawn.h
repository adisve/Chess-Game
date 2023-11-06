//
// Created by Adis Veletanlic on 11/2/23.
//

#include "../piece.h"

#ifndef CHESSGAME_PAWN_H
#define CHESSGAME_PAWN_H

class Pawn : public Piece {
public:
    Pawn(sf::Vector2i position, Color color, PieceType type);

    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> AvailableMoves(const Board& board, const std::optional<std::tuple<sf::Vector2i, sf::Vector2i>>& lastMove) const override;
    void AddSingleAndDoubleForwardMoves(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const;
    void AddDiagonalCaptureMoves(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, int direction, const Board& board) const;
    void AddCaptureMoveIfValid(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i capturePosition, const Board& board) const;
    void AddEnPassantMoves(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, int direction, const Board& board, const std::shared_ptr<Piece>& lastMovedPiece, const std::tuple<sf::Vector2i, sf::Vector2i>& lastMove) const;
    void AddEnPassantMoveIfValid(std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& moves, sf::Vector2i currentPosition, const std::tuple<sf::Vector2i, sf::Vector2i>& lastMove, int direction, const Board& board) const;

    static bool IsMovePossible(sf::Vector2i position, const Board& board) ;
    bool CanCaptureAt(sf::Vector2i position, const Board& board) const;
    bool CanEnPassant(sf::Vector2i enPassantPosition, int direction, const std::tuple<sf::Vector2i, sf::Vector2i>& lastMove, const Board& board) const;

    void SetHasMoved() {
        hasMoved = true;
    }

    const sf::Texture& GetTexture() const override {
        return texture;
    }

private:
    bool hasMoved = false;
};

#endif //CHESSGAME_PAWN_H

