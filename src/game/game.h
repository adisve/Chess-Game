//
// Created by Adis Veletanlic on 11/3/23.
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <SFML/Graphics.hpp>
#include "board/board.h"
#include "pieces/piece.h"

class Game {
private:
    sf::RenderWindow window;
    Board chessBoard;

public:
    Game();
    void Run();
    void HandleEvent(sf::Event& event);
};

#endif //CHESS_GAME_H

