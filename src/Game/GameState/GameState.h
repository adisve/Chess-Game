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

    GameState():
            whitePlayer(std::make_shared<Player>(PlayerColor::White)),
            blackPlayer(std::make_shared<Player>(PlayerColor::White)),
            board(std::make_shared<Board>()){};

    [[nodiscard]] Position GetKingPosition() const;

    void InitializeBoard();

    [[nodiscard]] bool IsKingInCheck() const;

    bool IsCheckmate(const Player& player);

    void PromotePawnAt(const Position& position, PieceType type);

    void ChangePlayerTurn();

    std::shared_ptr<Board> GetBoard();

    PlayerColor GetPlayerTurn();

    bool IsValidMove(const sf::Vector2i &move, const Player& currentPlayer);

    void MoveSelectedPieceTo(const Position& moveTo, const Position& moveFrom);

    void CapturePieceAt(const Position& attackedPosition);

    void UpdateKingPosition(const Position& position);

    void UpdateBoard(sf::RenderWindow& window, const std::vector<Move>& availableMoves, const std::optional<std::shared_ptr<Piece>>& selectedPiece);

    std::vector<Move> GetAvailableMovesCurrentPlayer();

    std::shared_ptr<Player> CurrentPlayer();

    std::optional<std::shared_ptr<Piece>> GetCurrentPlayerSelectedPiece();

private:
    std::shared_ptr<Board> board;

    Position blackKingPosition = {4, 0};

    Position whiteKingPosition = {4, 7};

    std::shared_ptr<Player> blackPlayer;

    std::shared_ptr<Player> whitePlayer;

    PlayerColor playerTurn = PlayerColor::White;
};

#endif //CHESSGAME_GAMESTATE_H
