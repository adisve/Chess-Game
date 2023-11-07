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
    auto currentPlayer = this->gameState.CurrentPlayer();
    auto availableMoves = currentPlayer->GetAvailableMoves();
    auto selectedPiece = currentPlayer->GetSelectedPiece(*this->gameState.GetBoard()).value();

    auto moveIt = std::find_if(availableMoves.begin(), availableMoves.end(),
                               [position](const Move& move) {
                                   return move.moveToDirection == position || move.attackingDirection == position;
                               });

    if (moveIt != availableMoves.end()) {
        this->moveManager.ExecuteMove(*moveIt);

        if (selectedPiece->GetType() == PieceType::Pawn) {
            this->gameState.CheckForPawnFirstMove(selectedPiece);
            this->gameState.CheckForPawnPromotion(selectedPiece, *moveIt);
        }

        this->gameState.CapturePieceAt(moveIt->attackingDirection);
        this->gameState.MoveSelectedPieceTo(moveIt->moveToDirection, moveIt->moveFromDirection);

        if (this->gameState.IsKingInCheck()) {
            if (this->gameState.IsCheckmate(*currentPlayer)) {
                // End the game or handle the checkmate scenario
            }
        }

        this->gameState.UpdateKingPosition(moveIt->moveToDirection);

        this->gameState.ChangePlayerTurn();

        currentPlayer->DeselectPiece();
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