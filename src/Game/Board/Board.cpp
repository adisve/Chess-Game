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
    sf::RectangleShape attackSquare(sf::Vector2f(squareSize, squareSize)); // Square shape for attacks
    sf::RectangleShape moveSquare(sf::Vector2f(squareSize, squareSize)); // Square shape for moves
    sf::CircleShape moveCircle((float)squareSize / 2 - 25);

    for (const auto& move : availableMoves) {
        auto regMove = move.moveToDirection;
        auto attackMove = move.attackingDirection;

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

sf::Color Board::DetermineSquareColor(Position position) {
    return ((position.x + position.y) % 2 == 0) ? sf::Color(183, 192, 216) : sf::Color(232, 237, 249);
}

void Board::DrawBoard(sf::RenderWindow& window) const {
    const int squareSize = 100;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            square.setPosition((float)row * squareSize, (float)col * squareSize);
            square.setFillColor(DetermineSquareColor(Position {row, col}));
            window.draw(square);
        }
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto piece = this->GetPieceAt({row, col});
            if (piece) {
                std::cout << "Drawing piece at " << piece->GetPosition().x << ", " << piece->GetPosition().y << std::endl;
                if (piece->sprite.getPosition().x != (float)piece->GetPosition().x * squareSize + 50 || piece->sprite.getPosition().y != (float)piece->GetPosition().y * squareSize + 50) {
                    std::cout << "Sprite position for piece is: (" << piece->sprite.getPosition().x << ", " << piece->sprite.getPosition().y << ")" << std::endl;
                    std::cout << "Piece position is: (" << piece->GetPosition().x << ", " << piece->GetPosition().y << ")" << std::endl;
                }

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