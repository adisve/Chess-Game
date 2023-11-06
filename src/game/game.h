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

    void TogglePlayerTurn() {
        playerTurn = playerTurn ==  Color::White ? Color::Black : Color::White;
    }

    void MovePieceTo(const sf::Vector2i& move);

    void DeselectCurrentPiece();

    bool CanMoveTo(const sf::Vector2i& move);

    void PromotePawnAt(const sf::Vector2i& position, PieceType type);

    void UpdateBoardAfterMove(std::tuple<sf::Vector2i, sf::Vector2i>);

    void CheckUpdateKingPosition(const sf::Vector2i& move) {
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

    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> GetAvailableMovesForSelectedPiece() {
        return availableMovesForSelectedPiece;
    }

    void SetAvailableMovesForSelectedPiece();

private:
    Board chessBoard;
    sf::RenderWindow window;
    Color playerTurn = Color::White;
    std::shared_ptr<Piece> selectedPiece = nullptr;
    sf::Vector2i selectedPosition = {-1, -1};
    sf::Vector2i blackKingPosition = {4, 0};
    sf::Vector2i whiteKingPosition = {4, 7};
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> availableMovesForSelectedPiece = {};
    static sf::RenderWindow GetGameWindow();
};

#endif //CHESS_GAME_H
