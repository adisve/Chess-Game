//
// Created by Adis Veletanlic on 11/3/23.
//

#include "game.h"

Game::Game() : window(sf::VideoMode(800, 800), "Chess Game") {
    chessBoard.Populate();
}

void Game::Run() {
    while (window.isOpen()) {
        window.clear();
        sf::Event event{};
        while (window.pollEvent(event)) {
            HandleEvent(event);
        }
        chessBoard.Draw(window);
        window.display();
    }
}

void Game::HandleEvent(sf::Event& event) {
    if (event.type == sf::Event::Closed)
        window.close();

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            int row = event.mouseButton.y / 100;
            int col = event.mouseButton.x / 100;

            auto piece = chessBoard.GetPieceAt(row, col);
            if (piece) {
                chessBoard.selectedPiece = piece;
                chessBoard.selectedPosition = {piece->row, piece->col};
            }
        }
    }
}
