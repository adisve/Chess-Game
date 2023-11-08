//
// Created by Adis Veletanlic on 11/3/23.
//

#include "Game.h"
#include "Piece/Knight/Knight.h"
#include "Piece/Queen/Queen.h"
#include "Piece/Rook/Rook.h"
#include "Piece/Bishop/Bishop.h"
#include "Piece/Pawn/Pawn.h"

const int SQUARE_SIZE = 100;

void Game::Run() {
    while (window.isOpen()) {
        window.clear();
        sf::Event event{};
        while (window.pollEvent(event)) {
            HandleEvent(event);
        }
        Render();
    }
}

void Game::HandleEvent(sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                HandleLeftMouseClick({event.mouseButton.x / SQUARE_SIZE, event.mouseButton.y / SQUARE_SIZE});
            }
            break;
        default:
            break;
    }
}

void Game::HandleLeftMouseClick(Position position) {
    auto currentPlayer = this->gameState.CurrentPlayer();
    auto playerTurn = this->gameState.GetPlayerTurn();
    auto clickedPiece = this->gameState.GetBoard()->GetPieceAt(position);

    if (currentPlayer->HasSelectedPiece()) {
        auto selectedPiece = currentPlayer->GetSelectedPiece(*this->gameState.GetBoard()).value();

        if (selectedPiece->GetPosition() == position) {
            currentPlayer->DeselectPiece();
        }
        else if (clickedPiece && clickedPiece->GetColor() == playerTurn) {
            currentPlayer->SelectPiece(position, *this->gameState.GetBoard(), this->moveManager.GetLastMove());
        }
        else {
            this->ExecuteMove(position);
        }
    }
    else if (clickedPiece && clickedPiece->GetColor() == playerTurn) {
        currentPlayer->SelectPiece(position, *this->gameState.GetBoard(), this->moveManager.GetLastMove());
    }
}


void Game::ExecuteMove(Position position) {
    if (const auto& move = this->gameState.CurrentPlayer()->CanMovePieceTo(position)) {
        this->moveManager.ExecuteMove(*move);
        this->gameState.ExecuteMove(*move);
        this->gameState.ChangePlayerTurn();
    }
}

void Game::RenderBoard() {
    this->gameState.UpdateBoard(this->window, this->gameState.GetAvailableMovesCurrentPlayer(), this->gameState.GetCurrentPlayerSelectedPiece());
}

void Game::Render() {
    this->window.clear();
    RenderBoard();
    this->window.display();
}