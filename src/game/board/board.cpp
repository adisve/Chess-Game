//
// Created by Adis Veletanlic on 11/2/23.
//

#include "board.h"
#include <iostream>
#include "../pieces/rook/rook.h"
#include "../pieces/pawn/pawn.h"
#include "../pieces/knight/knight.h"
#include "../pieces/bishop/bishop.h"
#include "../pieces/king/king.h"
#include "../pieces/queen/queen.h"

sf::Color Board::DetermineSquareColor(sf::Vector2i position, sf::Vector2i selectedPosition) {
    if ((position.x == selectedPosition.x) && (position.y == selectedPosition.y)) {
        return {177, 167, 252};
    }
    return ((position.x + position.y) % 2 == 0) ? sf::Color(183, 192, 216) : sf::Color(232, 237, 249);
}

void Board::DrawAvailableMoves(sf::RenderWindow &window, const std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& availableMoves, const std::shared_ptr<Piece>& selectedPiece) const {
    const int squareSize = 100;
    sf::RectangleShape attackSquare(sf::Vector2f(squareSize, squareSize)); // Square shape for attacks
    sf::RectangleShape moveSquare(sf::Vector2f(squareSize, squareSize)); // Square shape for moves
    sf::CircleShape moveCircle((float)squareSize / 2 - 25);

    for (const auto& move : availableMoves) {
        auto regMove = std::get<0>(move);
        auto attackMove = std::get<1>(move);

        auto attackedPiece = GetPieceAt({attackMove.x, attackMove.y});

        sf::Vector2f attackPosition((float)(attackMove.x * squareSize), (float)(attackMove.y * squareSize));
        sf::Vector2f movePosition((float)(regMove.x * squareSize), (float)(regMove.y * squareSize));
        attackSquare.setPosition(attackPosition);
        moveCircle.setPosition(movePosition.x + 25, movePosition.y + 25);

        if (attackedPiece && attackedPiece->GetColor() != selectedPiece->GetColor()) {
            attackSquare.setFillColor(sf::Color(237, 190, 199));
            window.draw(attackSquare);
        }
        moveCircle.setFillColor(sf::Color(123, 97, 255));
        window.draw(moveCircle);
    }
}



void Board::DrawBoard(sf::RenderWindow& window, const sf::Vector2i& selectedPosition, const std::vector<std::tuple<sf::Vector2i, sf::Vector2i>>& availableMoves) {
    const int squareSize = 100;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            square.setPosition((float)row * squareSize, (float)col * squareSize);
            square.setFillColor(DetermineSquareColor(sf::Vector2i {row, col}, selectedPosition));
            window.draw(square);
        }
    }

    if (selectedPosition.x != -1 && selectedPosition.y != -1) {
        DrawAvailableMoves(window, availableMoves, GetPieceAt(selectedPosition));
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto piece = GetPieceAt({row, col});
            if (piece) {
                window.draw(piece->sprite);
            }
        }
    }

}

void Board::Populate() {
    for (int row = 0; row < 8; ++row) {
        board[row][1] = std::make_shared<Pawn>(sf::Vector2i{row, 1}, Color::Black, PieceType::Pawn);
        board[row][6] = std::make_shared<Pawn>(sf::Vector2i{row, 6}, Color::White, PieceType::Pawn);
    }

    // Rooks
    board[0][0] = std::make_shared<Rook>((sf::Vector2i){0, 0}, Color::Black, PieceType::Rook);
    board[7][0] = std::make_shared<Rook>((sf::Vector2i){7, 0}, Color::Black, PieceType::Rook);
    board[0][7] = std::make_shared<Rook>((sf::Vector2i){0, 7}, Color::White, PieceType::Rook);
    board[7][7] = std::make_shared<Rook>((sf::Vector2i){7, 7}, Color::White, PieceType::Rook);

    // Knights
    board[1][0] = std::make_shared<Knight>((sf::Vector2i){1, 0}, Color::Black, PieceType::Knight);
    board[6][0] = std::make_shared<Knight>((sf::Vector2i){6, 0}, Color::Black, PieceType::Knight);
    board[1][7] = std::make_shared<Knight>((sf::Vector2i){1, 7}, Color::White, PieceType::Knight);
    board[6][7] = std::make_shared<Knight>((sf::Vector2i){6, 7}, Color::White, PieceType::Knight);

    // Bishops
    board[2][0] = std::make_shared<Bishop>((sf::Vector2i){2, 0}, Color::Black, PieceType::Bishop);
    board[5][0] = std::make_shared<Bishop>((sf::Vector2i){5, 0}, Color::Black, PieceType::Bishop);
    board[2][7] = std::make_shared<Bishop>((sf::Vector2i){2, 7}, Color::White, PieceType::Bishop);
    board[5][7] = std::make_shared<Bishop>((sf::Vector2i){5, 7}, Color::White, PieceType::Bishop);

    // Queens
    board[3][0] = std::make_shared<Queen>((sf::Vector2i){3, 0}, Color::Black, PieceType::Queen);
    board[3][7] = std::make_shared<Queen>((sf::Vector2i){3, 7}, Color::White, PieceType::Queen);

    // Kings
    board[4][0] = std::make_shared<King>((sf::Vector2i){4, 0}, Color::Black, PieceType::King);
    board[4][7] = std::make_shared<King>((sf::Vector2i){4, 7}, Color::White, PieceType::King);
}


std::shared_ptr<Piece> Board::GetPieceAt(sf::Vector2i position) const {
    if (!Board::IsWithinBounds(position)) {
        throw std::out_of_range("Coordinates out of board bounds.");
    }

    const auto& pieceOptional = board[position.x][position.y];

    return pieceOptional.value_or(nullptr);
}