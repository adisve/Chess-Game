//
// Created by Adis Veletanlic on 2023-11-06.
//

#ifndef CHESSGAME_GAMESTATE_H
#define CHESSGAME_GAMESTATE_H

#include "../Board/Board.h"
#include "../Player/Player.h"
#include "../Piece/Pawn/Pawn.h"
#include "../AudioManager/AudioManager.h"

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

    void ExecuteMove(const Move& move);

    void PromotePawn(const Position& position, PieceType type);

    void ChangePlayerTurn();

    std::shared_ptr<Board> GetBoard();

    PlayerColor GetPlayerTurn();

    bool IsLegalMove(const sf::Vector2i &move, const std::shared_ptr<Piece>& piece);

    void MoveSelectedPieceTo(const Position& moveTo, const Position& moveFrom);

    void CapturePieceAt(const Position& attackedPosition);

    void UpdateKingPosition(const Position& position);

    void UpdateBoard(sf::RenderWindow& window, const std::vector<Move>& availableMoves, const std::optional<std::shared_ptr<Piece>>& selectedPiece);

    bool CheckForPawnPromotion(const std::shared_ptr<Piece>& piece, const Move& move);

    static void CheckForPieceFirstMove(const std::shared_ptr<Piece>& piece);

    std::vector<Move> GetAvailableMovesCurrentPlayer();

    std::shared_ptr<Player> CurrentPlayer();

    std::optional<std::shared_ptr<Piece>> GetCurrentPlayerSelectedPiece();

    bool IsCheckmate(const Move& lastMove);

    bool PerformCapture(const Move& move);

    bool PerformMove(const Move& move);

    bool PerformCastling(const Move& move, const std::shared_ptr<Piece>& selectedPiece);

    bool PerformPromotion(const Move& move, const std::shared_ptr<Piece>& selectedPiece);

    void HandleMoveSounds(bool didCapture, bool didMove, bool didCastle, bool didPromote, const Move& lastMove);

    bool isClick = false;

    bool isDragging = false;

private:
    std::shared_ptr<Board> board;

    AudioManager audioManager;

    Position blackKingPosition = {4, 0};

    Position whiteKingPosition = {4, 7};

    std::shared_ptr<Player> blackPlayer;

    std::shared_ptr<Player> whitePlayer;

    PlayerColor playerTurn = PlayerColor::White;
};

#endif //CHESSGAME_GAMESTATE_H
