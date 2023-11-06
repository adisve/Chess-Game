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
        HandleLeftMouseClick({event.mouseButton.x / 100, event.mouseButton.y / 100});
    }
}

void Game::HandleLeftMouseClick(sf::Vector2i position) {
    if (!GetSelectedPiece()) {
        SelectPieceAt(position);
    } else {
        if (CanMoveTo(position)) {
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

bool Game::CanMoveTo(const sf::Vector2i& move) {
    std::vector<sf::Vector2i> availableMoves = GetAvailableMovesForSelectedPiece();
    return std::find(availableMoves.begin(), availableMoves.end(), move) != availableMoves.end();
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

bool Game::IsValidMove(const sf::Vector2i& move) {
    auto tempMovedPiece = selectedPiece;
    sf::Vector2i originalPosition = selectedPiece->GetPosition();
    auto pieceAtDestination = chessBoard.GetPieceAt(move);

    chessBoard.UpdateBoardPosition(originalPosition, nullptr);
    chessBoard.UpdateBoardPosition(move, tempMovedPiece);
    tempMovedPiece->SetPosition(move);

    bool movePutsKingInCheck = IsKingInCheck(playerTurn);

    chessBoard.UpdateBoardPosition(move, pieceAtDestination);
    chessBoard.UpdateBoardPosition(originalPosition, tempMovedPiece);
    tempMovedPiece->SetPosition(originalPosition);

    return !movePutsKingInCheck;
}


void Game::DeselectPiece() {
    SetSelectedPiece(nullptr);
    SetSelectedPosition({-1, -1});
}

void Game::MoveSelectedPieceTo(const sf::Vector2i& move) {
    auto attackedPiece = chessBoard.GetPieceAt(move);

    if (attackedPiece && attackedPiece->GetColor() != selectedPiece->GetColor()) {
        capturedPieces.push_back(attackedPiece);
        chessBoard.UpdateBoardPosition(move, nullptr);
    }

    SetLastMovedPiece(selectedPiece);
    SetLastMovedPiecePreviousPosition(selectedPiece->GetPosition());

    UpdateBoardWithMove(move);
    UpdateKingPosition(move);
    TogglePlayerTurn();
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

std::vector<sf::Vector2i> Game::GetAvailableMovesForSelectedPiece() {
    std::vector<sf::Vector2i> legalMoves;

    if (selectedPiece) {
        auto availableMoves = selectedPiece->AvailableMoves(chessBoard, lastMovedPiece, lastMovedPiecePreviousPosition);
        if (!IsKingInCheck(playerTurn)) {
            for (const auto& move : availableMoves) {
                if (IsValidMove(move)) {
                    legalMoves.push_back(move);
                }
            }
        } else if (IsKingInCheck(playerTurn)) {
            for (const auto& move : availableMoves) {
                if (MoveOutOfCheckPossible(move) && IsValidMove(move)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }
    return legalMoves;
}

bool Game::IsKingInCheck(Color color) const {
    auto kingPosition = GetKingPosition(color);

    return IsPawnThreat(kingPosition, color) ||
           IsKnightThreat(kingPosition, color) ||
           IsBishopThreat(kingPosition, color) ||
           IsRookThreat(kingPosition, color) ||
           IsQueenThreat(kingPosition, color);
}

bool Game::IsPawnThreat(const sf::Vector2i& kingPos, Color kingColor) const {
    int direction = (kingColor == Color::White) ? -1 : 1;
    std::vector<sf::Vector2i> pawnMoves = {
            {kingPos.x - 1, kingPos.y + direction},
            {kingPos.x + 1, kingPos.y + direction}
    };

    return std::any_of(pawnMoves.begin(), pawnMoves.end(),
                       [this, kingColor](const sf::Vector2i& move) {
                           if (Board::IsWithinBounds(move)) {
                               auto piece = chessBoard.GetPieceAt(move);
                               return piece && piece->GetType() == PieceType::Pawn && piece->GetColor() != kingColor;
                           }
                           return false;
    });
}

bool Game::IsKnightThreat(const sf::Vector2i& kingPos, Color kingColor) const {
    std::vector<sf::Vector2i> knightMoves = {
            {kingPos.x - 1, kingPos.y - 2}, {kingPos.x + 1, kingPos.y - 2},
            {kingPos.x + 2, kingPos.y - 1}, {kingPos.x + 2, kingPos.y + 1},
            {kingPos.x + 1, kingPos.y + 2}, {kingPos.x - 1, kingPos.y + 2},
            {kingPos.x - 2, kingPos.y + 1}, {kingPos.x - 2, kingPos.y - 1}
    };

    return std::any_of(knightMoves.begin(), knightMoves.end(), [this, kingColor](const sf::Vector2i& move) {
        if (Board::IsWithinBounds(move)) {
            auto piece = chessBoard.GetPieceAt(move);
            return piece && piece->GetType() == PieceType::Knight && piece->GetColor() != kingColor;
        }
        return false;
    });
}

bool Game::IsRookThreat(const sf::Vector2i& kingPos, Color kingColor) const {
    std::vector<sf::Vector2i> directions = {
            {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    for (const auto& dir : directions) {
        sf::Vector2i currentPos = kingPos;
        while (true) {
            currentPos += dir;
            if (!Board::IsWithinBounds(currentPos)) break;
            auto piece = chessBoard.GetPieceAt(currentPos);
            if (piece) {
                auto type = piece->GetType();
                if ((type == PieceType::Rook || type == PieceType::Queen) && piece->GetColor() != kingColor) {
                    return true;
                } else {
                    break;
                }
            }
        }
    }
    return false;
}

bool Game::IsBishopThreat(const sf::Vector2i& kingPos, Color kingColor) const {
    std::vector<sf::Vector2i> directions = {
            {-1, -1}, {1, -1}, {-1, 1}, {1, 1}
    };

    for (const auto& dir : directions) {
        sf::Vector2i currentPos = kingPos;
        while (true) {
            currentPos += dir;
            if (!Board::IsWithinBounds(currentPos)) break;

            auto piece = chessBoard.GetPieceAt(currentPos);
            if (piece) {
                auto type = piece->GetType();
                if ((type == PieceType::Bishop || type == PieceType::Queen) && piece->GetColor() != kingColor) {
                    return true;
                } else {
                    break;
                }
            }
        }
    }
    return false;
}

bool Game::IsQueenThreat(const sf::Vector2i& kingPos, Color kingColor) const {
    return IsRookThreat(kingPos, kingColor) || IsBishopThreat(kingPos, kingColor);
}

bool Game::MoveOutOfCheckPossible(const sf::Vector2i& move) {
    auto originalPosition = selectedPiece->GetPosition();
    auto pieceAtMovePosition = chessBoard.GetPieceAt(move);

    chessBoard.UpdateBoardPosition(originalPosition, nullptr);
    chessBoard.UpdateBoardPosition(move, selectedPiece);
    selectedPiece->SetPosition(move);

    UpdateKingPosition(move);

    bool isKingInCheckAfterMove = IsKingInCheck(playerTurn);

    chessBoard.UpdateBoardPosition(move, pieceAtMovePosition);
    chessBoard.UpdateBoardPosition(originalPosition, selectedPiece);
    selectedPiece->SetPosition(originalPosition);

    UpdateKingPosition(originalPosition);
    return !isKingInCheckAfterMove;
}
