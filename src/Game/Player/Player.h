//
// Created by Adis Veletanlic on 2023-11-06.
//

#ifndef CHESSGAME_PLAYER_H
#define CHESSGAME_PLAYER_H

#include <SFML/System/Vector2.hpp>
#include <memory>
#include "../Piece/PlayerColor.h"
#include "../Piece/Piece.h"
#include "Move.h"

using Position = sf::Vector2i;

class Player {
public:
    Player(PlayerColor color) : color(color) {}

    void SelectPiece(Position position, const Board& board, const std::optional<Move>& lastMove);

    void DeselectPiece();

    PlayerColor GetColor();

    [[nodiscard]] std::optional<std::shared_ptr<Piece>> GetSelectedPiece(const Board& board) const;

    std::vector<Move> GetAvailableMoves();

    Position GetSelectedPiecePosition();

    bool HasSelectedPiece() const;

private:
    Position selectedPiecePosition = { -1, -1 };
    std::vector<Move> availableMoves;
    PlayerColor color;
};

#endif //CHESSGAME_PLAYER_H
