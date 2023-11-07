//
// Created by Adis Veletanlic on 11/2/23.
//

#include "Board.h"
#include <iostream>
#include "../Piece/Knight/Knight.h"
#include "../Piece/Queen/Queen.h"
#include "../Piece/Rook/Rook.h"
#include "../Piece/Bishop/Bishop.h"
#include "../Piece/Pawn/Pawn.h"
#include "../Piece/King/King.h"

void Board::DrawAvailableMoves(sf::RenderWindow &window, const std::vector<Move>& availableMoves, const std::shared_ptr<Piece>& selectedPiece) const {
    const int squareSize = 100;
    sf::RectangleShape moveSquare(sf::Vector2f(squareSize, squareSize));

    for (const auto& move : availableMoves) {
        auto regMove = move.moveToDirection;
        auto attackMove = move.attackingDirection;

        auto attackedPiece = GetPieceAt({attackMove.x, attackMove.y});
        if (attackedPiece && attackedPiece->GetColor() != selectedPiece->GetColor()) {
            sf::Vector2f attackPosition((float)(attackMove.x * squareSize), (float)(attackMove.y * squareSize));
            sf::RectangleShape attackSquare(sf::Vector2f(squareSize, squareSize));
            attackSquare.setPosition(attackPosition);
            attackSquare.setFillColor(sf::Color(237, 190, 199, 150)); // Semi-transparent color
            window.draw(attackSquare);
        }

        sf::Vector2f movePosition((float)(regMove.x * squareSize), (float)(regMove.y * squareSize));
        sf::CircleShape moveCircle((float)squareSize / 2 - 25);
        moveCircle.setPosition(movePosition.x + 25, movePosition.y + 25);
        moveCircle.setFillColor(sf::Color(123, 97, 255, 150)); // Semi-transparent color
        window.draw(moveCircle);
    }
}

sf::Color Board::DetermineSquareColor(Position position, Position selectedPosition) {
    if (position == selectedPosition) {
        return {177, 167, 252};
    }
    return ((position.x + position.y) % 2 == 0) ? sf::Color(183, 192, 216) : sf::Color(232, 237, 249);
}

void Board::DrawBoard(sf::RenderWindow& window, const std::vector<Move>& availableMoves, const std::shared_ptr<Piece>& selectedPiece) const {
    const int squareSize = 100;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    const Position selectedPosition = selectedPiece == nullptr ? Position{-1, -1} : selectedPiece->GetPosition();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            square.setPosition((float)row * squareSize, (float)col * squareSize);
            square.setFillColor(DetermineSquareColor(Position {row, col}, selectedPosition));
            window.draw(square);
        }
    }

    if (!availableMoves.empty() || selectedPiece != nullptr) {
        std::cout << "Drawing available moves" << std::endl;
        // After drawing the board, draw the available move indicators.
        // This function must be called here to ensure indicators are drawn under the pieces.
        DrawAvailableMoves(window, availableMoves, selectedPiece);
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto piece = this->GetPieceAt({row, col});
            if (piece) {
                window.draw(piece->sprite);
            }
        }
    }
}

void Board::Populate() {

    for (int row = 0; row < 8; ++row) {
        board[row][1] = std::make_shared<Pawn>(sf::Vector2i{row, 1}, PlayerColor::Black, PieceType::Pawn);
        board[row][6] = std::make_shared<Pawn>(sf::Vector2i{row, 6}, PlayerColor::White, PieceType::Pawn);
    }

    // Rooks
    board[0][0] = std::make_shared<Rook>((sf::Vector2i){0, 0}, PlayerColor::Black, PieceType::Rook);
    board[7][0] = std::make_shared<Rook>((sf::Vector2i){7, 0}, PlayerColor::Black, PieceType::Rook);
    board[0][7] = std::make_shared<Rook>((sf::Vector2i){0, 7}, PlayerColor::White, PieceType::Rook);
    board[7][7] = std::make_shared<Rook>((sf::Vector2i){7, 7}, PlayerColor::White, PieceType::Rook);

    // Knights
    board[1][0] = std::make_shared<Knight>((sf::Vector2i){1, 0}, PlayerColor::Black, PieceType::Knight);
    board[6][0] = std::make_shared<Knight>((sf::Vector2i){6, 0}, PlayerColor::Black, PieceType::Knight);
    board[1][7] = std::make_shared<Knight>((sf::Vector2i){1, 7}, PlayerColor::White, PieceType::Knight);
    board[6][7] = std::make_shared<Knight>((sf::Vector2i){6, 7}, PlayerColor::White, PieceType::Knight);

    // Bishops
    board[2][0] = std::make_shared<Bishop>((sf::Vector2i){2, 0}, PlayerColor::Black, PieceType::Bishop);
    board[5][0] = std::make_shared<Bishop>((sf::Vector2i){5, 0}, PlayerColor::Black, PieceType::Bishop);
    board[2][7] = std::make_shared<Bishop>((sf::Vector2i){2, 7}, PlayerColor::White, PieceType::Bishop);
    board[5][7] = std::make_shared<Bishop>((sf::Vector2i){5, 7}, PlayerColor::White, PieceType::Bishop);

    // Queens
    board[3][0] = std::make_shared<Queen>((sf::Vector2i){3, 0}, PlayerColor::Black, PieceType::Queen);
    board[3][7] = std::make_shared<Queen>((sf::Vector2i){3, 7}, PlayerColor::White, PieceType::Queen);

    // Kings
    board[4][0] = std::make_shared<King>((sf::Vector2i){4, 0}, PlayerColor::Black, PieceType::King);
    board[4][7] = std::make_shared<King>((sf::Vector2i){4, 7}, PlayerColor::White, PieceType::King);
}


std::shared_ptr<Piece> Board::GetPieceAt(Position position) const {
    if (!Board::IsWithinBounds(position)) {
        return nullptr;
    }
    const auto& pieceOptional = this->board[position.x][position.y];
    return pieceOptional.value_or(nullptr);
}

void Board::SetPieceAt(Position position, const std::shared_ptr<Piece>& piece) {
    board[position.x][position.y] = piece;
}

void Board::Clear() {
    for (auto& row : board) {
        for (auto& piece : row) {
            piece = nullptr;
        }
    }
}