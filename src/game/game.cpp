//
// Created by Adis Veletanlic on 11/3/23.
//

#include <iostream>
#include "game.h"
#include "pieces/color.h"
#include "pieces/knight/knight.h"
#include "pieces/queen/queen.h"
#include "pieces/rook/rook.h"
#include "pieces/bishop/bishop.h"
#include "pieces/pawn/pawn.h"

Game::Game() : window(sf::VideoMode(800, 800), "Chess Game") {
    chessBoard.Populate();
    this->playerTurn = Color::White;
}

void Game::Run() {
    while (window.isOpen()) {
        window.clear();
        sf::Event event{};
        while (window.pollEvent(event)) {
            HandleEvent(event);
        }
        chessBoard.Draw(window, selectedPosition, GetAvailableMovesForSelectedPiece());
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
    sf::Vector2i position = {mouseX / 100, mouseY / 100};
    std::cout << "Position = x: " << position.x << ", y: " << position.y << std::endl;
    if (!GetSelectedPiece()) {
        SelectPieceAt(position);
    } else {
        if (IsValidMove(position)) {
            MoveSelectedPieceTo(position);
        } else {
            if (chessBoard.GetPieceAt(position)) {
                SelectPieceAt(position);
            } else {
                DeselectPiece();
            }
        }
    }
}


void Game::SelectPieceAt(sf::Vector2i position) {
    auto piece = chessBoard.GetPieceAt(position);
    if (piece) {
        if (piece->GetColor() == playerTurn) {
            SetSelectedPosition(position);
            SetSelectedPiece(piece);
        } else {
            DeselectPiece();
        }
    }
}

bool Game::IsValidMove(const sf::Vector2i& move) const {
    std::vector<sf::Vector2i> availableMoves = GetAvailableMovesForSelectedPiece();
    return std::find(availableMoves.begin(), availableMoves.end(), move) != availableMoves.end();
}

void Game::DeselectPiece() {
    SetSelectedPiece(nullptr);
    SetSelectedPosition({-1, -1});
}

void Game::MoveSelectedPieceTo(const sf::Vector2i& move) {
    auto attackedPiece = chessBoard.GetPieceAt({move.x, move.y});

    if (attackedPiece && attackedPiece->GetColor() != selectedPiece->GetColor()) {
        capturedPieces.push_back(attackedPiece);
        chessBoard.UpdateBoardPosition(move, nullptr);
    }

    SetLastMovedPiece(selectedPiece);
    SetLastMovedPiecePreviousPosition(selectedPiece->GetPosition());

    UpdateBoardWithMove(move);
    SetPlayerTurn(playerTurn == Color::White ? Color::Black : Color::White);
    DeselectPiece();

    std::cout << "Moved a piece to x: " << move.x << ", y: " << move.y << std::endl;
}

void Game::UpdateBoardWithMove(sf::Vector2i move) {
    chessBoard.UpdateBoardPosition(selectedPiece->GetPosition(), nullptr);
    chessBoard.UpdateBoardPosition(move, selectedPiece);
    selectedPiece->SetPosition(move);

    if (selectedPiece->CanPromote(move)) {
        PromotePawnAt(move, PieceType::Queen);
    }

    selectedPiece->sprite.setOrigin((float)selectedPiece->GetTexture().getSize().x / 2, (float)selectedPiece->GetTexture().getSize().y / 2);
    selectedPiece->sprite.setPosition((float)move.x * 100 + 50, (float)move.y * 100 + 50);

    if (selectedPiece->GetType() == PieceType::Pawn) {
        auto pawn = std::dynamic_pointer_cast<Pawn>(selectedPiece);
        pawn->SetHasMoved();
    }
}

void Game::PromotePawnAt(const sf::Vector2i& position, PieceType type) {
    auto pawn = chessBoard.GetPieceAt(position);
    switch (type) {
        case PieceType::Queen:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Queen>(position, pawn->GetColor(), pawn->GetType()));
            break;
        case PieceType::Rook:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Rook>(position, pawn->GetColor(), pawn->GetType()));
            break;
        case PieceType::Bishop:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Bishop>(position, pawn->GetColor(), pawn->GetType()));
            break;
        case PieceType::Knight:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Knight>(position, pawn->GetColor(), pawn->GetType()));
            break;
        case PieceType::Pawn:
        case PieceType::King:
            break;
    }
}

std::vector<sf::Vector2i> Game::GetAvailableMovesForSelectedPiece() const {
    std::vector<sf::Vector2i> legalMoves;
    if (selectedPiece) {
        auto availableMoves = selectedPiece->AvailableMoves(chessBoard, lastMovedPiece, lastMovedPiecePreviousPosition);
        for (const auto& move : availableMoves) {
            if (!IsKingInCheck(playerTurn)) {
                legalMoves.push_back(move);
            }
        }
    }
    return legalMoves;
}

bool Game::IsKingInCheck(Color color) const {
    auto kingPosition = GetKingPosition(color);

    // Check for pawns
    if (CheckForPawns(kingPosition, color)) {
        std::cout << "THERE IS A PAWN ATTACKING !!!!!" << std::endl;
        return true;
    } else {
        std::cout << "There are no pawns attacking your king" << std::endl;
    }


    return false;
}

bool Game::CheckForPawns(sf::Vector2i kingPosition, Color color) const {
    std::vector<sf::Vector2i> possiblePawnAttacks;

    if (color == Color::White) {
        possiblePawnAttacks.emplace_back(kingPosition.x - 1, kingPosition.y - 1);
        possiblePawnAttacks.emplace_back(kingPosition.x + 1, kingPosition.y - 1);
    } else {
        possiblePawnAttacks.emplace_back(kingPosition.x - 1, kingPosition.y + 1);
        possiblePawnAttacks.emplace_back(kingPosition.x + 1, kingPosition.y + 1);
    }

    // Now use std::any_of to check these vectors
    return std::any_of(possiblePawnAttacks.begin(), possiblePawnAttacks.end(),
       [this, color](const sf::Vector2i& attackPosition) {
        std::cout << "Checking for attacking pawn at x: " << attackPosition.x << ", y: " << attackPosition.y << std::endl;
        // First, make sure the attack position is within the bounds of the board
        if (Board::IsWithinBounds(attackPosition)) {
            return false;
        }

        auto pawn = chessBoard.GetPieceAt(attackPosition);
        // Make sure there is a pawn at the attack position and it's an opposing pawn
        return pawn && pawn->GetType() == PieceType::Pawn && pawn->GetColor() != color;
    });
}
