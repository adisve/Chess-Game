//
// Created by Adis Veletanlic on 11/3/23.
//

#include <SFML/Graphics.hpp>
#include "Piece/Piece.h"
#include "Board/Board.h"
#include "GameState/GameState.h"
#include "MovementManager/MoveManager.h"
#include <memory>
#include <utility>
#pragma once

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

class Game {
public:
    Game() :
    whitePlayer(std::make_shared<Player>(PlayerColor::White)),
    blackPlayer(std::make_shared<Player>(PlayerColor::White)),
    window(sf::VideoMode(800, 800), "Chess Game", sf::Style::Default) {
        gameState.InitializeBoard();
    }

    void Run();

    void HandleEvent(sf::Event& event);

    void HandleLeftMouseClick(sf::Vector2i position);

    std::shared_ptr<Player> CurrentPlayer();

private:

    GameState gameState;

    MoveManager moveManager;

    sf::RenderWindow window;

    std::shared_ptr<Player> blackPlayer;

    std::shared_ptr<Player> whitePlayer;

    void RenderBoard();
    void Render();
    std::vector<Move> GetAvailableMovesCurrentPlayer();
    std::optional<std::shared_ptr<Piece>> GetCurrentPlayerSelectedPiece();
    bool HasCurrentPlayerSelectedPiece();
};

#endif //CHESS_GAME_H
