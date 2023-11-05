//
// Created by Adis Veletanlic on 11/3/23.
//

#include <SFML/Graphics.hpp>
#include "board/board.h"
#include "pieces/piece.h"
#include <memory>
#pragma once

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

class Game {
private:
    sf::RenderWindow window;
    Board chessBoard;
    std::shared_ptr<Piece> selectedPiece;
    sf::Vector2i selectedPosition;
    Color turn;

    void HandleLeftMouseClick(int mouseX, int mouseY);
    void SelectPieceAt(int row, int col);
    bool IsValidMove(const sf::Vector2i& move);
    void MoveSelectedPieceTo(const sf::Vector2i& move);
    void DeselectPiece();

public:
    Game();
    void Run();
    void HandleEvent(sf::Event& event);
};

#endif //CHESS_GAME_H
