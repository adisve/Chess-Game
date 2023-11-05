//
// Created by Adis Veletanlic on 11/3/23.
//

#include <iostream>
#include "game.h"
#include "pieces/color.h"

Game::Game() : window(sf::VideoMode(800, 800), "Chess Game") {
    chessBoard.Populate();
    this->turn = Color::WHITE;
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
    if (event.type == sf::Event::Closed) {
        window.close();
    } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        HandleLeftMouseClick(event.mouseButton.x, event.mouseButton.y);
    }
}

void Game::HandleLeftMouseClick(int mouseX, int mouseY) {
    int row = mouseY / 100;
    int col = mouseX / 100;

    if (chessBoard.GetSelectedPiece() == nullptr) {
        SelectPieceAt(row, col);
    } else {
        sf::Vector2i move = {row, col};
        if (IsValidMove(move)) {
            MoveSelectedPieceTo(move);
        } else {
            if (chessBoard.GetPieceAt(row, col)) {
                SelectPieceAt(row, col);
            } DeselectPiece();
        }
    }
}

void Game::SelectPieceAt(int row, int col) {
    auto piece = chessBoard.GetPieceAt(row, col);
    if (piece) {
        if (piece->GetColor() == turn) {
            chessBoard.SetSelectedPosition({row, col});
            chessBoard.SetSelectedPiece(piece);
        } else {
            DeselectPiece();
        }
    }
}

bool Game::IsValidMove(const sf::Vector2i& move) {
    std::vector<sf::Vector2i> availableMoves = chessBoard.GetSelectedPiece()->AvailableMoves(chessBoard);
    return std::find(availableMoves.begin(), availableMoves.end(), move) != availableMoves.end();
}

void Game::MoveSelectedPieceTo(const sf::Vector2i& move) {

    chessBoard.MoveSelectedPiece(move.x, move.y);
    switch (turn) {
        case Color::BLACK:
            turn = Color::WHITE;
            break;
        case Color::WHITE:
            turn = Color::BLACK;
    }
    DeselectPiece();
}

void Game::DeselectPiece() {
    chessBoard.SetSelectedPiece(nullptr);
    chessBoard.SetSelectedPosition({-1, -1});
}

