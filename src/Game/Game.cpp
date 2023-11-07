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
    if (event.type == sf::Event::Closed) {
        window.close();
    } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        HandleLeftMouseClick({event.mouseButton.x / 100, event.mouseButton.y / 100});
    }
}

void Game::HandleLeftMouseClick(Position position) {
    if (HasCurrentPlayerSelectedPiece()) {
        auto selectedPiece = GetCurrentPlayerSelectedPiece().value();
        if (selectedPiece->GetPosition() == position) {
            this->CurrentPlayer()->DeselectPiece();
        } else if (this->gameState.GetBoard()->GetPieceAt(position) != nullptr && this->gameState.GetBoard()->GetPieceAt(position)->GetColor() == this->CurrentPlayer()->GetColor()) {
            this->CurrentPlayer()->SelectPiece(position, *this->gameState.GetBoard(), this->moveManager.GetLastMove());
        } else {
            auto availableMoves = GetAvailableMovesCurrentPlayer();
            auto move = std::find_if(availableMoves.begin(), availableMoves.end(), [position](Move move) {
                return move.moveToDirection == position || move.attackingDirection == position;
            });
            if (move != availableMoves.end()) {
                this->moveManager.ExecuteMove(*move);
                this->gameState.MoveSelectedPieceTo(move->moveToDirection, move->moveFromDirection);
                this->gameState.ChangePlayerTurn();
                this->CurrentPlayer()->DeselectPiece();
            }
        }
    } else {
        auto piece = this->gameState.GetBoard()->GetPieceAt(position);
        if (piece != nullptr && piece->GetColor() == this->CurrentPlayer()->GetColor()) {
            this->CurrentPlayer()->SelectPiece(position, *this->gameState.GetBoard(), this->moveManager.GetLastMove());
        }
    }
}

void Game::RenderBoard() {
    this->gameState.UpdateBoard(this->window);
    this->gameState.RenderMovesAndAttacks(this->window, GetAvailableMovesCurrentPlayer(), GetCurrentPlayerSelectedPiece());
}

std::optional<std::shared_ptr<Piece>> Game::GetCurrentPlayerSelectedPiece() {
    return this->CurrentPlayer()->GetSelectedPiece(*this->gameState.GetBoard());
}

std::vector<Move> Game::GetAvailableMovesCurrentPlayer() {
    return this->CurrentPlayer()->GetAvailableMoves();
}

std::shared_ptr<Player> Game::CurrentPlayer() {
    return this->gameState.GetPlayerTurn() == PlayerColor::White ? whitePlayer : blackPlayer;
}

bool Game::HasCurrentPlayerSelectedPiece() {
    return this->CurrentPlayer()->HasSelectedPiece();
}

void Game::Render() {
    this->window.clear();
    RenderBoard();
    this->window.display();
}