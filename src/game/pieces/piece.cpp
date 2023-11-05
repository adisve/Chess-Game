//
// Created by Adis Veletanlic on 11/4/23.
//
#include "piece.h"
#include "../board/board.h"


std::vector<sf::Vector2i> Piece::FindMovesInDirectionForPiece(int rowIncrement, int colIncrement, const Board& board) const {
    std::vector<sf::Vector2i> moves;

    int i = GetPosition().x + rowIncrement;
    int j = GetPosition().y + colIncrement;
    while (i >= 0 && i < 8 && j >= 0 && j < 8) {
        if (board.GetPieceAt({i, j}) == nullptr) {
            moves.emplace_back(i, j);
        } else {
            if (board.GetPieceAt({i, j})->color != color) {
                moves.emplace_back(i, j);
            }
            break;
        }
        i += rowIncrement;
        j += colIncrement;
    }
    return moves;
}

bool Piece::CanThreatenKing(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition, const Board& board) const {
    switch (GetType()) {
        case PieceType::Pawn:
            return IsPawnThreat(piecePosition, kingPosition);
        case PieceType::Knight:
            return IsKnightThreat(piecePosition, kingPosition);
        case PieceType::Rook:
            return IsRookThreat(piecePosition, kingPosition, board);
        case PieceType::Bishop:
            return IsBishopThreat(piecePosition, kingPosition, board);
        case PieceType::Queen:
            return IsQueenThreat(piecePosition, kingPosition, board);
        case PieceType::King:
            return IsKingThreat(piecePosition, kingPosition);
    }
    return false;
}

bool Piece::IsPawnThreat(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition) const {
    int direction = (GetColor() == Color::White) ? 1 : -1;
    sf::Vector2i threatPosition1(piecePosition.x - 1, piecePosition.y + direction);
    sf::Vector2i threatPosition2(piecePosition.x + 1, piecePosition.y + direction);

    bool isThreat = kingPosition == threatPosition1 || kingPosition == threatPosition2;

    std::cout << "Is pawn threat: " << isThreat << std::endl;

    return isThreat;
}

bool Piece::IsKnightThreat(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition) {
    // Knights threaten in an "L" shape: 2 by 1 or 1 by 2 squares away
    static const std::vector<sf::Vector2i> knightMoves = {
            {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
            {2, 1}, {1, 2}, {-1, 2}, {-2, 1}
    };

    for (const auto& move : knightMoves) {
        if (kingPosition == sf::Vector2i(piecePosition.x + move.x, piecePosition.y + move.y)) {
            std::cout << "Is knight threat: true" << std::endl;
            return true;
        }
    }
    return false;
}

bool Piece::IsRookThreat(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition, const Board& board) {
    // Rooks threaten along the same row or column
    if (piecePosition.x != kingPosition.x && piecePosition.y != kingPosition.y) {
        return false;
    }

    // Check the path between the rook and the king for other pieces
    int min, max;
    if (piecePosition.x == kingPosition.x) {
        min = std::min(piecePosition.y, kingPosition.y) + 1;
        max = std::max(piecePosition.y, kingPosition.y);
        for (int y = min; y < max; y++) {
            if (board.GetPieceAt({piecePosition.x, y})) return false;
        }
    } else {
        min = std::min(piecePosition.x, kingPosition.x) + 1;
        max = std::max(piecePosition.x, kingPosition.x);
        for (int x = min; x < max; x++) {
            if (board.GetPieceAt({x, piecePosition.y})) return false;
        }
    }
    std::cout << "Is rook threat: true" << std::endl;
    return true;
}

bool Piece::IsBishopThreat(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition, const Board& board) {
    // Bishops threaten along diagonals
    if (std::abs(piecePosition.x - kingPosition.x) != std::abs(piecePosition.y - kingPosition.y)) {
        return false;
    }

    int xDirection = (kingPosition.x - piecePosition.x) / std::abs(kingPosition.x - piecePosition.x);
    int yDirection = (kingPosition.y - piecePosition.y) / std::abs(kingPosition.y - piecePosition.y);
    sf::Vector2i current = piecePosition + sf::Vector2i(xDirection, yDirection);

    while (current != kingPosition) {
        if (board.GetPieceAt({current.x, current.y})) return false;
        current += sf::Vector2i(xDirection, yDirection);
    }
    std::cout << "Is bishop threat: true" << std::endl;
    return true;
}

bool Piece::IsQueenThreat(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition, const Board& board) {
    // Queen threatens like both a rook and a bishop
    bool isThreat = IsRookThreat(piecePosition, kingPosition, board) || IsBishopThreat(piecePosition, kingPosition, board);
    std::cout << "Is queen threat: " << isThreat << std::endl;
    return isThreat;
}

bool Piece::IsKingThreat(const sf::Vector2i& piecePosition, const sf::Vector2i& kingPosition) {
    // King can threaten the square next to it in any direction
    bool isThreat = std::abs(piecePosition.x - kingPosition.x) <= 1 && std::abs(piecePosition.y - kingPosition.y) <= 1;
    std::cout << "Is king threat: " << isThreat << std::endl;
    return isThreat;
}
