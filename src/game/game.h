//
// Created by Adis Veletanlic on 11/3/23.
//

#include <SFML/Graphics.hpp>
#include "board/board.h"
#include "pieces/piece.h"
#include <memory>
#include <utility>
#pragma once

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

class Game {
public:
    Game();
    void Run();
    void HandleEvent(sf::Event& event);
    void HandleLeftMouseClick(int mouseX, int mouseY);
    bool IsValidMove(const sf::Vector2i& move) const;
    bool IsKingInCheck(Color color) const; // Determines if any opponent pieces are currently attacking the players king
    bool IsKingInCheckmate(Color color) const; // Determines if the player is in checkmate


    sf::Vector2i GetKingPosition(Color color) const {
        return color == Color::White ? whiteKingPosition : blackKingPosition;
    };

    void SelectPieceAt(sf::Vector2i position);

    void SetSelectedPiece(std::shared_ptr<Piece> piece) {
        selectedPiece = std::move(piece);
    }

    void SetSelectedPosition(const sf::Vector2i& position) {
        selectedPosition = position;
    }

    void SetLastMovedPiece(const std::shared_ptr<Piece>& piece) {
        lastMovedPiece = piece;
    }

    void SetLastMovedPiecePreviousPosition(const sf::Vector2i& position) {
        lastMovedPiecePreviousPosition = position;
    }

    void SetPlayerTurn(Color color) {
        playerTurn = color;
    }

    void MoveSelectedPieceTo(const sf::Vector2i& move);

    void DeselectPiece();

    void PromotePawnAt(const sf::Vector2i& position, PieceType type);

    void UpdateBoardWithMove(sf::Vector2i move);

    Color GetPlayerTurn() const {
        return playerTurn;
    }

    std::shared_ptr<Piece> GetSelectedPiece() const {
        return selectedPiece;
    }

    sf::Vector2i GetSelectedPosition() const {
        return selectedPosition;
    }

    std::shared_ptr<Piece> GetLastMovedPiece() const {
        return lastMovedPiece;
    }

    sf::Vector2i GetLastMovedPiecePreviousPosition() const {
        return lastMovedPiecePreviousPosition;
    }

    std::vector<sf::Vector2i> GetAvailableMovesForSelectedPiece() const;

private:
    Board chessBoard;
    sf::RenderWindow window;
    Color playerTurn = Color::White;
    std::shared_ptr<Piece> lastMovedPiece;
    std::shared_ptr<Piece> selectedPiece = nullptr;
    std::vector<std::shared_ptr<Piece>> capturedPieces;
    sf::Vector2i lastMovedPiecePreviousPosition;
    sf::Vector2i selectedPosition = {-1, -1};
    sf::Vector2i blackKingPosition = {4, 0};
    sf::Vector2i whiteKingPosition = {4, 7};

    bool CheckForPawns(sf::Vector2i vector2, Color color) const;
};

#endif //CHESS_GAME_H
