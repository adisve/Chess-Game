//
// Created by Adis Veletanlic on 11/3/23.
//

#include "Game.h"
#include "Piece/Knight/Knight.h"
#include "Piece/Queen/Queen.h"
#include "Piece/Rook/Rook.h"
#include "Piece/Bishop/Bishop.h"
#include "Piece/Pawn/Pawn.h"

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
                this->StartDragging({event.mouseButton.x / SQUARE_SIZE, event.mouseButton.y / SQUARE_SIZE});
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left) {
                this->StopDragging({event.mouseButton.x / SQUARE_SIZE, event.mouseButton.y / SQUARE_SIZE});
            }
            break;
        case sf::Event::MouseMoved:
            if (this->gameState.isDragging) {
                this->UpdateDragging({event.mouseMove.x, event.mouseMove.y});
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


bool Game::ExecuteMove(Position position) {
    auto currentPlayer = this->gameState.CurrentPlayer();
    auto selectedPiece = currentPlayer->GetSelectedPiece(*this->gameState.GetBoard());
    if (selectedPiece && this->gameState.IsLegalMove(position, selectedPiece.value())) {
        if (const auto& move = currentPlayer->CanMovePieceTo(position)) {
            this->moveManager.ExecuteMove(*move);
            this->gameState.ExecuteMove(*move);
            this->gameState.ChangePlayerTurn();
            return true;
        }
    }
    return false;
}

void Game::RenderBoard() {
    this->gameState.UpdateBoard(this->window, this->gameState.GetAvailableMovesCurrentPlayer(), this->gameState.GetCurrentPlayerSelectedPiece());
}

void Game::Render() {
    this->window.clear();
    RenderBoard();

    auto draggedPiece = this->gameState.CurrentPlayer()->GetSelectedPiece(*this->gameState.GetBoard());
    if (this->gameState.isDragging && draggedPiece.has_value()) {
        draggedPiece.value()->Render(this->window);
    }

    this->window.display();
}

void Game::StartDragging(sf::Vector2i position) {
    auto currentPlayer = this->gameState.CurrentPlayer();
    auto piece = this->gameState.GetBoard()->GetPieceAt(position);

    if (piece && piece->GetColor() == this->gameState.GetPlayerTurn()) {
        this->gameState.isDragging = true;
        currentPlayer->SelectPiece(position, *this->gameState.GetBoard(), this->moveManager.GetLastMove());

        auto selectedPiece = currentPlayer->GetSelectedPiece(*this->gameState.GetBoard());

        if (selectedPiece.has_value()) {
            selectedPiece.value()->SetIsDragged(true);
        }
    }
}

void Game::StopDragging(Position position) {
    auto currentPlayer = this->gameState.CurrentPlayer();
    auto playerSelectedPiece = currentPlayer->GetSelectedPiece(*this->gameState.GetBoard())->get();

    if (this->gameState.isDragging && playerSelectedPiece && currentPlayer->CanMovePieceTo(position)) {
        Position boardPosition = {position.x, position.y};
        if (this->ExecuteMove(boardPosition)) {
            playerSelectedPiece->SetLogicalPosition(position);
        }
        playerSelectedPiece->UpdateVisualPositionFromLogical();
        playerSelectedPiece->SetIsDragged(false);

    } else if (playerSelectedPiece) {
        currentPlayer->DeselectPiece();
        playerSelectedPiece->UpdateVisualPositionFromLogical();
        playerSelectedPiece->SetIsDragged(false);
    }
    this->gameState.isDragging = false;
}

void Game::UpdateDragging(sf::Vector2i position) {
    auto newPosition = sf::Vector2f(position);
    this->gameState.CurrentPlayer()->GetSelectedPiece(*this->gameState.GetBoard())->get()->SetVisualPosition(newPosition);
}
