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
    return ((position.x + position.y) % 2 == 0) ? sf::Color(232, 237, 249) : sf::Color(183, 192, 216);
}

void Board::DrawAvailableMoves(sf::RenderWindow &window, const std::vector<sf::Vector2i> &availableMoves, const std::shared_ptr<Piece>& selectedPiece) const {
    for (const auto& move : availableMoves) {
        auto piece = GetPieceAt({move.x, move.y});

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


void Board::Draw(sf::RenderWindow& window, const sf::Vector2i& selectedPosition, const std::vector<sf::Vector2i>& availableMoves) {
    const int squareSize = 100;
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            square.setPosition((float)col * squareSize, (float)row * squareSize);
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
    for (int col = 0; col < 8; ++col) {
        board[1][col] = std::make_shared<Pawn>(sf::Vector2i{1, col}, Color::Black, PieceType::Pawn);
        board[6][col] = std::make_shared<Pawn>(sf::Vector2i{6, col}, Color::White, PieceType::Pawn);
    }

    // Rooks
    board[0][0] = std::make_shared<Rook>((sf::Vector2i){0, 0}, Color::Black, PieceType::Rook);
    board[0][7] = std::make_shared<Rook>((sf::Vector2i){0, 7}, Color::Black, PieceType::Rook);
    board[7][0] = std::make_shared<Rook>((sf::Vector2i){7, 0}, Color::White, PieceType::Rook);
    board[7][7] = std::make_shared<Rook>((sf::Vector2i){7, 7}, Color::White, PieceType::Rook);

    // Knights
    board[0][1] = std::make_shared<Knight>((sf::Vector2i){0, 1}, Color::Black, PieceType::Knight);
    board[0][6] = std::make_shared<Knight>((sf::Vector2i){0, 6}, Color::Black, PieceType::Knight);
    board[7][1] = std::make_shared<Knight>((sf::Vector2i){7, 1}, Color::White, PieceType::Knight);
    board[7][6] = std::make_shared<Knight>((sf::Vector2i){7, 6}, Color::White, PieceType::Knight);

    // Bishops
    board[0][2] = std::make_shared<Bishop>((sf::Vector2i){0, 2}, Color::Black, PieceType::Bishop);
    board[0][5] = std::make_shared<Bishop>((sf::Vector2i){0, 5}, Color::Black, PieceType::Bishop);
    board[7][2] = std::make_shared<Bishop>((sf::Vector2i){7, 2}, Color::White, PieceType::Bishop);
    board[7][5] = std::make_shared<Bishop>((sf::Vector2i){7, 5}, Color::White, PieceType::Bishop);

    // Queens
    board[0][3] = std::make_shared<Queen>((sf::Vector2i){0, 3}, Color::Black, PieceType::Queen);
    board[7][3] = std::make_shared<Queen>((sf::Vector2i){7, 3}, Color::White, PieceType::Queen);

    // Kings
    board[0][4] = std::make_shared<King>((sf::Vector2i){0, 4}, Color::Black, PieceType::King);
    board[7][4] = std::make_shared<King>((sf::Vector2i){7, 4}, Color::White, PieceType::King);
}


std::shared_ptr<Piece> Board::GetPieceAt(sf::Vector2i position) const {
    if (position.x < 0 || position.x >= 8 || position.y < 0 || position.y >= 8) {
        throw std::out_of_range("Coordinates out of board bounds.");
    }

    const auto& pieceOptional = board[position.x][position.y];

    return pieceOptional.value_or(nullptr);
}

std::vector<sf::Vector2i> Board::GetThreateningPiecesPositions(const sf::Vector2i& kingPosition, Color kingColor) {
    std::vector<sf::Vector2i> threatPositions;

    std::vector<sf::Vector2i> pawnThreats;

    if (kingColor == Color::Black) {
        // Black king is threatened by white pawns moving down (decreasing x)
        pawnThreats = {
                {kingPosition.x - 1, kingPosition.y - 1},
                {kingPosition.x - 1, kingPosition.y + 1}
        };
    } else {
        // White king is threatened by black pawns moving up (increasing x)
        pawnThreats = {
                {kingPosition.x + 1, kingPosition.y - 1},
                {kingPosition.x + 1, kingPosition.y + 1}
        };
    }

    threatPositions.insert(threatPositions.end(), pawnThreats.begin(), pawnThreats.end());

    // Knight threats
    std::vector<sf::Vector2i> knightThreats = {
            {kingPosition.x - 2, kingPosition.y - 1},
            {kingPosition.x - 1, kingPosition.y - 2},
            {kingPosition.x + 1, kingPosition.y - 2},
            {kingPosition.x + 2, kingPosition.y - 1},
            {kingPosition.x + 2, kingPosition.y + 1},
            {kingPosition.x + 1, kingPosition.y + 2},
            {kingPosition.x - 1, kingPosition.y + 2},
            {kingPosition.x - 2, kingPosition.y + 1}
    };
    threatPositions.insert(threatPositions.end(), knightThreats.begin(), knightThreats.end());

    // Line threats (for rooks and queens)
    for (int i = -7; i <= 7; ++i) {
        if (i != 0) {
            threatPositions.emplace_back(kingPosition.x, kingPosition.y + i); // Column
            threatPositions.emplace_back(kingPosition.x + i, kingPosition.y); // Row
        }
    }

    // Diagonal threats (for bishops and queens)
    for (int i = -7; i <= 7; ++i) {
        if (i != 0) {
            threatPositions.emplace_back(kingPosition.x + i, kingPosition.y + i);
            threatPositions.emplace_back(kingPosition.x + i, kingPosition.y - i);
        }
    }

    // King threats
    std::vector<sf::Vector2i> kingThreats = {
            {kingPosition.x - 1, kingPosition.y - 1},
            {kingPosition.x, kingPosition.y - 1},
            {kingPosition.x + 1, kingPosition.y - 1},
            {kingPosition.x + 1, kingPosition.y},
            {kingPosition.x + 1, kingPosition.y + 1},
            {kingPosition.x, kingPosition.y + 1},
            {kingPosition.x - 1, kingPosition.y + 1},
            {kingPosition.x - 1, kingPosition.y}
    };
    threatPositions.insert(threatPositions.end(), kingThreats.begin(), kingThreats.end());

    // Remove positions that are off the board
    threatPositions.erase(std::remove_if(threatPositions.begin(), threatPositions.end(),
                                         [](const sf::Vector2i& pos) { return pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7; }),
                          threatPositions.end());

    return threatPositions;
}
