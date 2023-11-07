//
// Created by Adis Veletanlic on 2023-11-06.
//

#ifndef CHESSGAME_GAMESTATE_H
#define CHESSGAME_GAMESTATE_H

#include "../Board/Board.h"
#include "../Player/Player.h"

using Position = sf::Vector2i;

class GameState {
public:

    GameState() {
        this->board = std::make_shared<Board>();
    };

    [[nodiscard]] Position GetKingPosition(PlayerColor color) const;

    void InitializeBoard();

    [[nodiscard]] bool IsKingInCheck() const;

    bool IsCheckmate(const Player& player);

    void PromotePawnAt(const Position& position, PieceType type);

    void ChangePlayerTurn();

    std::shared_ptr<Board> GetBoard();

    PlayerColor GetPlayerTurn();

    bool IsValidMove(const sf::Vector2i &move, const Player& currentPlayer);

    void MoveSelectedPieceTo(const Position& moveTo, const Position& moveFrom);

    void UpdateKingPosition(const Position& position, const std::shared_ptr<Piece>& piece, Position& kingPosition);

    void UpdateBoard(sf::RenderWindow& window);

    void RenderMovesAndAttacks(sf::RenderWindow& window, const std::vector<Move>& availableMoves, const std::optional<std::shared_ptr<Piece>>& selectedPiece);

    static bool CanMoveTo(const sf::Vector2i &move, const std::vector<Move>& availableMoves);

    std::shared_ptr<Piece> GetPieceOnBoard(Position position);

private:
    std::shared_ptr<Board> board;

    Position blackKingPosition = {4, 0};

    Position whiteKingPosition = {4, 7};

    PlayerColor playerTurn = PlayerColor::White;
};

#endif //CHESSGAME_GAMESTATE_H
