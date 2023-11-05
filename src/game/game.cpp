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
    sf::Vector2i position = {mouseY / 100, mouseX / 100};
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

    switch (playerTurn) {
        case Color::Black:
            playerTurn = Color::White;
            break;
        case Color::White:
            playerTurn = Color::Black;
    }
    DeselectPiece();
}

void Game::UpdateBoardWithMove(sf::Vector2i move) {
    chessBoard.UpdateBoardPosition(selectedPiece->GetPosition(), nullptr);
    chessBoard.UpdateBoardPosition(move, selectedPiece);
    selectedPiece->SetPosition(move);

    if (selectedPiece->CanPromote(move)) {
        PromotePawnAt(move, PieceType::Queen);
    }

    selectedPiece->sprite.setOrigin((float)selectedPiece->GetTexture().getSize().x / 2, (float)selectedPiece->GetTexture().getSize().y / 2);
    selectedPiece->sprite.setPosition((float)move.y * 100 + 50, (float)move.x * 100 + 50);

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
            // 1. Validate all moves before showing them to the player
            //    - Check if the move would put the players king in check
            //    - If it would, then the move is not valid
            // 2. If the move is valid, then show it to the player by adding to legalMoves

            // For now just return all available moves
            legalMoves.push_back(move);
        }
    }
    return legalMoves;
}

bool Game::IsKingInCheck(Color color) const {
    std::cout << "Checking if king is in check for color " << (color == Color::White ? "white" : "black") << std::endl;
    sf::Vector2i kingPosition = GetKingPosition(color);
    std::cout << "King position: " << kingPosition.x << ", " << kingPosition.y << std::endl;
    auto threatPositions = Board::GetThreateningPiecesPositions(kingPosition, color);

    return std::any_of(threatPositions.begin(), threatPositions.end(),
                       [this, kingPosition, color](const sf::Vector2i& pos) {
                           std::shared_ptr<Piece> piece = chessBoard.GetPieceAt({pos.x, pos.y});
                           // If there is a piece of opposing color on this square that can threaten the king, then the king is in check.
                           std::cout << "Checking if piece at " << pos.x << ", " << pos.y << " can threaten king at " << kingPosition.x << ", " << kingPosition.y << std::endl;
                           return piece && piece->GetColor() != color && piece->CanThreatenKing(pos, kingPosition, chessBoard);
                       });
}
