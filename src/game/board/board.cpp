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

void Board::Draw(sf::RenderWindow& window) {
    const int squareSize = 100;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    sf::Color darkColor = sf::Color(183, 192, 216);
    sf::Color lightColor = sf::Color(232, 237, 249);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            square.setPosition(col * squareSize, row * squareSize);

            if ((row + col) % 2 == 0) {
                square.setFillColor(lightColor);
            } else {
                square.setFillColor(darkColor);
            }

            window.draw(square);

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