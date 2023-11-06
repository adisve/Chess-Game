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
    void HandleLeftMouseClick(sf::Vector2i position);
    bool MoveOutOfCheckPossible(const sf::Vector2i& move);
    bool IsValidMove(const sf::Vector2i& move);
    bool IsKingInCheck(Color color) const;

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

    void TogglePlayerTurn() {
        playerTurn = playerTurn ==  Color::White ? Color::Black : Color::White;
    }

    void MoveSelectedPieceTo(const sf::Vector2i& move);

    void DeselectPiece();

    bool CanMoveTo(const sf::Vector2i& move);

    void PromotePawnAt(const sf::Vector2i& position, PieceType type);

    void UpdateBoardWithMove(sf::Vector2i move);

    void UpdateKingPosition(const sf::Vector2i& move) {
        if (selectedPiece->GetType() == PieceType::King) {
            if (selectedPiece->GetColor() == Color::White) {
                whiteKingPosition = move;
            } else {
                blackKingPosition = move;
            }
        }
    }

    std::shared_ptr<Piece> GetSelectedPiece() const {
        return selectedPiece;
    }

    std::vector<sf::Vector2i> GetAvailableMovesForSelectedPiece();

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

    bool IsKnightThreat(const sf::Vector2i& kingPos, Color kingColor) const;
    bool IsPawnThreat(const sf::Vector2i& kingPos, Color kingColor) const;
    bool IsRookThreat(const sf::Vector2i& kingPos, Color kingColor) const;
    bool IsBishopThreat(const sf::Vector2i& kingPos, Color kingColor) const;
    bool IsQueenThreat(const sf::Vector2i& kingPos, Color kingColor) const;
};

#endif //CHESS_GAME_H
