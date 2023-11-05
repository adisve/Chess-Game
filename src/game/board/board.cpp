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

void Board::DrawAvailableMoves(sf::RenderWindow &window, const std::vector<sf::Vector2i> &availableMoves) {
    for (const auto& move : availableMoves) {
        auto piece = GetPieceAt(move.x, move.y);

        sf::CircleShape circle((float)100 / 2 - 25);

        if (piece && piece->GetColor() != selectedPiece->GetColor()) {
            circle.setFillColor(sf::Color(237, 92, 114));
        } else {
            circle.setFillColor(sf::Color(123, 97, 255));
        }

        circle.setPosition((float)(move.y * 100) + 25, (float)(move.x * 100) + 25);
        window.draw(circle);
    }
}


void Board::Draw(sf::RenderWindow& window) {
    const int squareSize = 100;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    auto availableMoves = GetAvailableMovesForSelectedPiece();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            square.setPosition((float)col * squareSize, (float)row * squareSize);
            square.setFillColor(DetermineSquareColor(row, col));
            window.draw(square);
        }
    }

    DrawAvailableMoves(window, availableMoves);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto piece = GetPieceAt(row, col);
            if (piece) {
                window.draw(piece->sprite);
            }
        }
    }

}


void Board::Populate() {
    for (int col = 0; col < 8; ++col) {
        board[1][col] = std::make_shared<Pawn>(sf::Vector2i{1, col}, Color::BLACK);
        board[6][col] = std::make_shared<Pawn>(sf::Vector2i{6, col}, Color::WHITE);
    }

    // Rooks
    board[0][0] = std::make_shared<Rook>((sf::Vector2i){0, 0}, Color::BLACK);
    board[0][7] = std::make_shared<Rook>((sf::Vector2i){0, 7}, Color::BLACK);
    board[7][0] = std::make_shared<Rook>((sf::Vector2i){7, 0}, Color::WHITE);
    board[7][7] = std::make_shared<Rook>((sf::Vector2i){7, 7}, Color::WHITE);

    // Knights
    board[0][1] = std::make_shared<Knight>((sf::Vector2i){0, 1}, Color::BLACK);
    board[0][6] = std::make_shared<Knight>((sf::Vector2i){0, 6}, Color::BLACK);
    board[7][1] = std::make_shared<Knight>((sf::Vector2i){7, 1}, Color::WHITE);
    board[7][6] = std::make_shared<Knight>((sf::Vector2i){7, 6}, Color::WHITE);

    // Bishops
    board[0][2] = std::make_shared<Bishop>((sf::Vector2i){0, 2}, Color::BLACK);
    board[0][5] = std::make_shared<Bishop>((sf::Vector2i){0, 5}, Color::BLACK);
    board[7][2] = std::make_shared<Bishop>((sf::Vector2i){7, 2}, Color::WHITE);
    board[7][5] = std::make_shared<Bishop>((sf::Vector2i){7, 5}, Color::WHITE);

    // Queens
    board[0][3] = std::make_shared<Queen>((sf::Vector2i){0, 3}, Color::BLACK);
    board[7][3] = std::make_shared<Queen>((sf::Vector2i){7, 3}, Color::WHITE);

    // Kings
    board[0][4] = std::make_shared<King>((sf::Vector2i){0, 4}, Color::BLACK);
    board[7][4] = std::make_shared<King>((sf::Vector2i){7, 4}, Color::WHITE);
}

std::vector<sf::Vector2i> Board::GetAvailableMovesForSelectedPiece() const  {
    if (selectedPiece) {
        return selectedPiece->AvailableMoves(*this);
    }
    return {};
}

std::shared_ptr<Piece> Board::GetPieceAt(int row, int col) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        throw std::out_of_range("Coordinates out of board bounds.");
    }

    const auto& pieceOptional = board[row][col];

    return pieceOptional.value_or(nullptr);
}


void Board::MoveSelectedPiece(int toRow, int toCol) {
    auto attackedPiece = GetPieceAt(toRow, toCol);

    if (attackedPiece && attackedPiece->GetColor() != selectedPiece->GetColor()) {
        capturedPieces.push_back(attackedPiece);
        board[toRow][toCol] = nullptr;
    }

    board[selectedPiece->GetPosition().x][selectedPiece->GetPosition().y] = nullptr;
    selectedPiece->SetPosition({toRow, toCol});
    board[toRow][toCol] = selectedPiece;
    selectedPiece->sprite.setOrigin((float)selectedPiece->GetTexture().getSize().x / 2, (float)selectedPiece->GetTexture().getSize().y / 2);
    selectedPiece->sprite.setPosition((float)toCol * 100 + 50, (float)toRow * 100 + 50);
}


