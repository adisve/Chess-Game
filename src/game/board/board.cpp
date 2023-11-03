//
// Created by Adis Veletanlic on 11/2/23.
//

#include "board.h"
#include <iostream>
#include "../pieces/piece.h"
#include "../pieces/rook/rook.h"
#include "../pieces/pawn/pawn.h"
#include "../pieces/knight/knight.h"
#include "../pieces/bishop/bishop.h"
#include "../pieces/king/king.h"
#include "../pieces/queen/queen.h"

sf::Color Board::DetermineSquareColor(int row, int col) const {
    if ((row == selectedPosition.x) && (col == selectedPosition.y)) {
        return {177, 167, 252};
    }
    return ((row + col) % 2 == 0) ? sf::Color(232, 237, 249) : sf::Color(183, 192, 216);
}

void Board::DrawAvailableMoves(sf::RenderWindow& window, const std::vector<sf::Vector2i>& availableMoves, int row, int col, int squareSize) const {
    if (std::find(availableMoves.begin(), availableMoves.end(), sf::Vector2i(row, col)) != availableMoves.end() &&
        !(selectedPosition.x == col && selectedPosition.y == row)) {
        sf::CircleShape circle(squareSize / 2 - 20);
        circle.setFillColor(sf::Color(123, 97, 255));
        circle.setPosition((col * squareSize) + 20, (row * squareSize) + 20);
        window.draw(circle);
    }
}

void Board::Draw(sf::RenderWindow& window) {
    const int squareSize = 100;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    auto availableMoves = GetAvailableMovesForSelectedPiece();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            square.setPosition(col * squareSize, row * squareSize);
            square.setFillColor(DetermineSquareColor(row, col));
            window.draw(square);

            DrawAvailableMoves(window, availableMoves, row, col, squareSize);

            auto piece = GetPieceAt(row, col);
            if (piece) {
                window.draw(piece->sprite);
            }
        }
    }
}


void Board::Populate() {
    for (int col = 0; col < 8; ++col) {
        board[1][col] = std::make_shared<Pawn>(1, col, Color::BLACK);
        board[6][col] = std::make_shared<Pawn>(6, col, Color::WHITE);
    }

    // Rooks
    board[0][0] = std::make_shared<Rook>(0, 0, Color::BLACK);
    board[0][7] = std::make_shared<Rook>(0, 7, Color::BLACK);
    board[7][0] = std::make_shared<Rook>(7, 0, Color::WHITE);
    board[7][7] = std::make_shared<Rook>(7, 7, Color::WHITE);

    // Knights
    board[0][1] = std::make_shared<Knight>(0, 1, Color::BLACK);
    board[0][6] = std::make_shared<Knight>(0, 6, Color::BLACK);
    board[7][1] = std::make_shared<Knight>(7, 1, Color::WHITE);
    board[7][6] = std::make_shared<Knight>(7, 6, Color::WHITE);

    // Bishops
    board[0][2] = std::make_shared<Bishop>(0, 2, Color::BLACK);
    board[0][5] = std::make_shared<Bishop>(0, 5, Color::BLACK);
    board[7][2] = std::make_shared<Bishop>(7, 2, Color::WHITE);
    board[7][5] = std::make_shared<Bishop>(7, 5, Color::WHITE);

    // Queens
    board[0][3] = std::make_shared<Queen>(0, 3, Color::BLACK);
    board[7][3] = std::make_shared<Queen>(7, 3, Color::WHITE);

    // Kings
    board[0][4] = std::make_shared<King>(0, 4, Color::BLACK);
    board[7][4] = std::make_shared<King>(7, 4, Color::WHITE);
}

std::shared_ptr<Piece> Board::GetPieceAt(int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw std::out_of_range("Coordinates out of board bounds.");
    }

    const auto& pieceOptional = board[row][col];

    return pieceOptional.value_or(nullptr);
}


bool Board::IsMoveLegal(int fromRow, int fromCol, int toRow, int toCol) {
    return false;
}

bool Board::IsKingInCheck(Color playerColor) {
    return false;
}

bool Board::MovePiece(int fromRow, int fromCol, int toRow, int toCol) {
    return false;
}

std::vector<sf::Vector2i> Board::GetAvailableMovesForSelectedPiece() const  {
    if (selectedPiece) {
        return selectedPiece->AvailableMoves(*this);
    }
    return {};
}
