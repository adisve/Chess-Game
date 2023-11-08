//
// Created by Adis Veletanlic on 11/3/23.
//

#include <SFML/Graphics.hpp>
#include "Piece/Piece.h"
#include "Board/Board.h"
#include "GameState/GameState.h"
#include "MoveManager/MoveManager.h"
#include <memory>
#include <utility>
#pragma once

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

class Game {
public:
    Game() : window(sf::VideoMode(800, 800), "Chess Game", sf::Style::Default) {
        gameState.InitializeBoard();
    }

    void Run();

private:

    GameState gameState;

    MoveManager moveManager;

    sf::RenderWindow window;

    void HandleEvent(sf::Event& event);

    void HandleLeftMouseClick(sf::Vector2i position);

    void MovePiece(Position position);

    void CapturePiece(Position position);

    void ExecuteMove(Position position);

    void RenderBoard();

    void Render();

};

#endif //CHESS_GAME_H
