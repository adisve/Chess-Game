#include <SFML/Graphics.hpp>
#include "game/board/board.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Game");
    Board chessBoard = Board();
    chessBoard.Populate();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        chessBoard.Draw(window);
        window.display();
    }

    return 0;
}
