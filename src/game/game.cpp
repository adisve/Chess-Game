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

Game::Game() : window(Game::GetGameWindow()) {
    chessBoard.Populate();
    this->playerTurn = Color::White;
}

sf::RenderWindow Game::GetGameWindow() {
    unsigned int antialiasingLevel = 8;
    sf::ContextSettings settings;
    settings.antialiasingLevel = antialiasingLevel;
    return { sf::VideoMode(800, 800), "Chess Game", sf::Style::Default, settings };
}

void Game::Run() {
    while (window.isOpen()) {
        window.clear();
        sf::Event event{};
        while (window.pollEvent(event)) {
            HandleEvent(event);
        }
        chessBoard.DrawBoard(window, selectedPosition, availableMovesForSelectedPiece);
        window.display();
    }
}

void Game::HandleEvent(sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        HandleLeftMouseClick({event.mouseButton.x / 100, event.mouseButton.y / 100});
    }
}

void Game::HandleLeftMouseClick(sf::Vector2i position) {
    if (!GetSelectedPiece()) {
        SelectPieceAt(position);
    } else {
        if (CanMoveTo(position)) {
            MovePieceTo(position);
        } else {
            if (chessBoard.GetPieceAt(position)) {
                SelectPieceAt(position);
            } else {
                DeselectCurrentPiece();
            }
        }
    }
}

bool Game::CanMoveTo(const sf::Vector2i& move) {
    std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> availableMoves = GetAvailableMovesForSelectedPiece();
    return std::any_of(availableMoves.begin(), availableMoves.end(),
                       [&move](const std::tuple<sf::Vector2i, sf::Vector2i>& availableMove) {
                           return std::get<0>(availableMove) == move;
                       });
}


void Game::SelectPieceAt(sf::Vector2i position) {
    auto piece = chessBoard.GetPieceAt(position);
    if (piece) {
        if (piece->GetColor() == playerTurn) {
            SetSelectedPosition(position);
            SetSelectedPiece(piece);
            SetAvailableMovesForSelectedPiece();
        } else {
            DeselectCurrentPiece();
        }
    }
}

bool Game::IsValidMove(const sf::Vector2i& move) {
    auto tempMovedPiece = selectedPiece;
    sf::Vector2i originalPosition = selectedPosition;
    auto pieceAtDestination = chessBoard.GetPieceAt(move);

    chessBoard.UpdateBoardPosition(originalPosition, nullptr);
    chessBoard.UpdateBoardPosition(move, tempMovedPiece);
    tempMovedPiece->SetPosition(move);

    CheckUpdateKingPosition(move);

    bool movePutsKingInCheck = IsKingInCheck(playerTurn);

    chessBoard.UpdateBoardPosition(move, pieceAtDestination);
    chessBoard.UpdateBoardPosition(originalPosition, tempMovedPiece);
    tempMovedPiece->SetPosition(originalPosition);

    CheckUpdateKingPosition(originalPosition);

    return !movePutsKingInCheck;
}


void Game::DeselectCurrentPiece() {
    SetSelectedPiece(nullptr);
    SetSelectedPosition({-1, -1});
    SetAvailableMovesForSelectedPiece();
}

void Game::MovePieceTo(const sf::Vector2i& destination) {
    auto it = std::find_if(availableMovesForSelectedPiece.begin(), availableMovesForSelectedPiece.end(),
                           [&destination](const std::tuple<sf::Vector2i, sf::Vector2i>& availableMove) {
                               return std::get<0>(availableMove) == destination;
                           });

    if (it != availableMovesForSelectedPiece.end()) {
        std::tuple<sf::Vector2i, sf::Vector2i> moveTuple = *it;

        UpdateBoardAfterMove(moveTuple);

        CheckUpdateKingPosition(std::get<0>(moveTuple));

        TogglePlayerTurn();

        DeselectCurrentPiece();
    }
}

void Game::UpdateBoardAfterMove(std::tuple<sf::Vector2i, sf::Vector2i> move) {

    const auto movePosition = std::get<0>(move);
    const auto attackPosition = std::get<1>(move);

    chessBoard.UpdateBoardPosition(selectedPosition, nullptr);
    chessBoard.UpdateBoardPosition(attackPosition, nullptr);

    if (selectedPiece->CanPromote(movePosition)) {
        PromotePawnAt(movePosition, PieceType::Queen);
        SetSelectedPiece(chessBoard.GetPieceAt(movePosition));
    } else {
        chessBoard.UpdateBoardPosition(movePosition, selectedPiece);
    }

    chessBoard.AddMoveDone(selectedPosition, movePosition);

    selectedPiece->SetPosition(movePosition);

    selectedPiece->sprite.setOrigin((float)selectedPiece->GetTexture().getSize().x / 2, (float)selectedPiece->GetTexture().getSize().y / 2);
    selectedPiece->sprite.setPosition((float)movePosition.x * 100 + 50, (float)movePosition.y * 100 + 50);

    if (selectedPiece->GetType() == PieceType::Pawn) {
        auto pawn = std::dynamic_pointer_cast<Pawn>(selectedPiece);
        pawn->SetHasMoved();
    }
}

void Game::PromotePawnAt(const sf::Vector2i& position, PieceType type) {
    auto pawn = chessBoard.GetPieceAt(position);
    switch (type) {
        case PieceType::Queen:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Queen>(position, playerTurn, type));
            break;
        case PieceType::Rook:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Rook>(position, playerTurn, type));
            break;
        case PieceType::Bishop:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Bishop>(position, playerTurn, type));
            break;
        case PieceType::Knight:
            chessBoard.UpdateBoardPosition(position, std::make_shared<Knight>(position, playerTurn, type));
            break;
        case PieceType::Pawn:
        case PieceType::King:
            break;
    }
}

void Game::SetAvailableMovesForSelectedPiece() {
    if (selectedPiece) {
        std::vector<std::tuple<sf::Vector2i, sf::Vector2i>> legalMoves = {};
        std::tuple<sf::Vector2i, sf::Vector2i> lastMove;

        if (!chessBoard.GetMovesDone().empty()) {
            lastMove = chessBoard.GetMovesDone().back();
        }

        if (selectedPiece) {
            auto availableMoves = selectedPiece->AvailableMoves(chessBoard, lastMove);
            if (!IsKingInCheck(playerTurn)) {
                for (const auto& move : availableMoves) {
                    const auto movePosition = std::get<0>(move);
                    if (IsValidMove(movePosition)) {
                        legalMoves.push_back(move);
                    }
                }
            } else if (IsKingInCheck(playerTurn)) {
                for (const auto& move : availableMoves) {
                    const auto movePosition = std::get<0>(move);
                    if (IsValidMove(movePosition)) {
                        legalMoves.push_back(move);
                    }
                }
            }
        }
        availableMovesForSelectedPiece = legalMoves;
    } else {
        availableMovesForSelectedPiece = {};
    }
}

bool Game::IsKingInCheck(Color color) const {
    sf::Vector2i kingPosition = GetKingPosition(color);

    // Pawn Threats
    int direction = (color == Color::White) ? -1 : 1;
    std::vector<sf::Vector2i> pawnThreats = {
            {kingPosition.x - 1, kingPosition.y + direction},
            {kingPosition.x + 1, kingPosition.y + direction}
    };
    for (const auto& move : pawnThreats) {
        if (Board::IsWithinBounds(move)) {
            auto piece = chessBoard.GetPieceAt(move);
            if (piece && piece->GetType() == PieceType::Pawn && piece->GetColor() != color) {
                return true;
            }
        }
    }

    // Knight Threats
    std::vector<sf::Vector2i> knightThreats = {
            {kingPosition.x - 1, kingPosition.y - 2}, {kingPosition.x + 1, kingPosition.y - 2},
            {kingPosition.x + 2, kingPosition.y - 1}, {kingPosition.x + 2, kingPosition.y + 1},
            {kingPosition.x + 1, kingPosition.y + 2}, {kingPosition.x - 1, kingPosition.y + 2},
            {kingPosition.x - 2, kingPosition.y + 1}, {kingPosition.x - 2, kingPosition.y - 1}
    };
    for (const auto& move : knightThreats) {
        if (Board::IsWithinBounds(move)) {
            auto piece = chessBoard.GetPieceAt(move);
            if (piece && piece->GetType() == PieceType::Knight && piece->GetColor() != color) {
                return true;
            }
        }
    }

    // Rook, Bishop, and Queen Threats
    std::vector<sf::Vector2i> directions = {
            {0, -1}, {0, 1}, {-1, 0}, {1, 0},   // Rook and Queen
            {-1, -1}, {1, -1}, {-1, 1}, {1, 1}  // Bishop and Queen
    };
    for (const auto& dir : directions) {
        sf::Vector2i currentPos = kingPosition;
        while (Board::IsWithinBounds(currentPos += dir)) {
            auto piece = chessBoard.GetPieceAt(currentPos);
            if (piece) {
                PieceType type = piece->GetType();
                if ((type == PieceType::Rook || type == PieceType::Bishop || type == PieceType::Queen) && piece->GetColor() != color) {
                    if (type == PieceType::Queen) return true; // Queen threat
                    if (dir.x == 0 || dir.y == 0) { // Horizontal or vertical
                        if (type == PieceType::Rook) return true; // Rook threat
                    } else { // Diagonal
                        if (type == PieceType::Bishop) return true; // Bishop threat
                    }
                }
                break; // Blocked by another piece
            }
        }
    }
    return false;
}

