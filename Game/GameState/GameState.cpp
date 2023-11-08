//
// Created by Adis Veletanlic on 2023-11-06.
//

#include "GameState.h"
#include "../Piece/Queen/Queen.h"
#include "../Piece/Rook/Rook.h"
#include "../Piece/Bishop/Bishop.h"
#include "../Piece/Knight/Knight.h"
#include "../Piece/Pawn/Pawn.h"
#include "../Piece/King/King.h"

const std::vector<PieceType> moveRestrictions = {
        PieceType::King,
        PieceType::Rook,
        PieceType::Pawn
};

Position GameState::GetKingPosition() const {
    return playerTurn == PlayerColor::White ? whiteKingPosition : blackKingPosition;
}

void GameState::ChangePlayerTurn() {
    this->playerTurn = this->playerTurn == PlayerColor::White ? PlayerColor::Black : PlayerColor::White;
}

void GameState::UpdateBoard(sf::RenderWindow& window, const std::vector<Move>& availableMoves, const std::optional<std::shared_ptr<Piece>>& selectedPiece) {
    if (selectedPiece.has_value()) {
        this->board->DrawBoard(window, availableMoves, selectedPiece.value());
    } else {
        this->board->DrawBoard(window, availableMoves, nullptr);
    }
}

bool GameState::IsKingInCheck() const {
    Position kingPosition = GetKingPosition();

    int direction = (playerTurn == PlayerColor::White) ? -1 : 1;
    std::vector<sf::Vector2i> pawnThreats = {
            {kingPosition.x - 1, kingPosition.y + direction},
            {kingPosition.x + 1, kingPosition.y + direction}
    };
    for (const auto& move : pawnThreats) {
        if (Board::IsWithinBounds(move)) {
            auto piece = board->GetPieceAt(move);
            if (piece && piece->GetType() == PieceType::Pawn && piece->GetColor() != playerTurn) {
                return true;
            }
        }
    }

    std::vector<sf::Vector2i> knightThreats = {
            {kingPosition.x - 1, kingPosition.y - 2}, {kingPosition.x + 1, kingPosition.y - 2},
            {kingPosition.x + 2, kingPosition.y - 1}, {kingPosition.x + 2, kingPosition.y + 1},
            {kingPosition.x + 1, kingPosition.y + 2}, {kingPosition.x - 1, kingPosition.y + 2},
            {kingPosition.x - 2, kingPosition.y + 1}, {kingPosition.x - 2, kingPosition.y - 1}
    };
    for (const auto& move : knightThreats) {
        if (Board::IsWithinBounds(move)) {
            auto piece = board->GetPieceAt(move);
            if (piece && piece->GetType() == PieceType::Knight && piece->GetColor() != playerTurn) {
                return true;
            }
        }
    }

    std::vector<sf::Vector2i> directions = {
            {0, -1}, {0, 1}, {-1, 0}, {1, 0},   // Rook and Queen
            {-1, -1}, {1, -1}, {-1, 1}, {1, 1}  // Bishop and Queen
    };
    for (const auto& dir : directions) {
        sf::Vector2i currentPos = kingPosition;
        while (Board::IsWithinBounds(currentPos += dir)) {
            auto piece = board->GetPieceAt(currentPos);
            if (piece) {
                PieceType type = piece->GetType();
                if ((type == PieceType::Rook || type == PieceType::Bishop || type == PieceType::Queen) && piece->GetColor() != playerTurn) {
                    if (type == PieceType::Queen) return true; // Queen threat
                    if (dir.x == 0 || dir.y == 0) { // Horizontal or vertical
                        if (type == PieceType::Rook) return true; // Rook threat
                    } else { // Diagonal
                        if (type == PieceType::Bishop) return true; // Bishop threat
                    }
                }
                break;
            }
        }
    }

    return false;
}

bool GameState::IsCheckmate(const Move& lastMove) {
    if (!this->IsKingInCheck()) {
        return false;
    }
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            sf::Vector2i pos(x, y);
            auto piece = board->GetPieceAt(pos);
            if (piece && piece->GetColor() == playerTurn) {
                auto moves = piece->AvailableMoves(*this->board, lastMove);
                for (const auto& move : moves) {
                    if (this->IsLegalMove(move.moveToDirection, piece)) {
                        return false;
                    }
                }
            }
        }
    }

    this->audioManager.PlayCheckmateSound();
    return true;
}

std::shared_ptr<Board> GameState::GetBoard() {
    return this->board;
}

void GameState::PromotePawn(const Position& position, PieceType type) {
    auto pawn = this->board->GetPieceAt(position);
    switch (type) {
        case PieceType::Queen:
            this->board->SetPieceAt(position, std::make_shared<Queen>(position, playerTurn, type));
            break;
        case PieceType::Rook:
            this->board->SetPieceAt(position, std::make_shared<Rook>(position, playerTurn, type));
            break;
        case PieceType::Bishop:
            this->board->SetPieceAt(position, std::make_shared<Bishop>(position, playerTurn, type));
            break;
        case PieceType::Knight:
            this->board->SetPieceAt(position, std::make_shared<Knight>(position, playerTurn, type));
            break;
        case PieceType::Pawn:
        case PieceType::King:
            break;
    }
}

bool GameState::IsLegalMove(const sf::Vector2i &move, const std::shared_ptr<Piece>& piece) {
    Position originalPosition = piece->GetPosition();
    auto pieceAtDestination = board->GetPieceAt(move);

    this->MoveSelectedPieceTo(move, originalPosition);
    this->board->SetPieceAt(originalPosition, nullptr);
    this->UpdateKingPosition(move);

    bool movePutsKingInCheck = this->IsKingInCheck();

    this->MoveSelectedPieceTo(originalPosition, move);
    this->board->SetPieceAt(move, pieceAtDestination);
    this->UpdateKingPosition(originalPosition);

    return !movePutsKingInCheck;
}


void GameState::MoveSelectedPieceTo(const Position& moveTo, const Position& moveFrom) {
    auto movingPiece = this->board->GetPieceAt(moveFrom);
    this->board->SetPieceAt(moveTo, this->board->GetPieceAt(moveFrom));
    movingPiece->SetLogicalPosition(moveTo);
    this->board->SetPieceAt(moveFrom, nullptr);
}

void GameState::CapturePieceAt(const Position& attackedPosition) {
    this->board->SetPieceAt(attackedPosition, nullptr);
}

void GameState::ExecuteMove(const Move& move) {
    auto currentPlayer = this->CurrentPlayer();
    auto selectedPiece = currentPlayer->GetSelectedPiece(*this->board).value();

    bool didCapture = this->PerformCapture(move);
    bool didMove = this->PerformMove(move);
    bool didCastle = this->PerformCastling(move, selectedPiece);
    bool didPromote = this->PerformPromotion(move, selectedPiece);

    this->UpdateKingPosition(move.moveToDirection);

    GameState::CheckForPieceFirstMove(selectedPiece);
    currentPlayer->DeselectPiece();

    this->HandleMoveSounds(didCapture, didMove, didCastle, didPromote, move);
}

bool GameState::PerformCapture(const Move& move) {
    if (this->board->GetPieceAt(move.attackingDirection) != nullptr) {
        this->CapturePieceAt(move.attackingDirection);
        return true;
    }
    return false;
}

bool GameState::PerformMove(const Move& move) {
    this->MoveSelectedPieceTo(move.moveToDirection, move.moveFromDirection);
    return true;
}

bool GameState::PerformCastling(const Move& move, const std::shared_ptr<Piece>& selectedPiece) {
    if (selectedPiece->GetType() == PieceType::King) {
        if (std::abs(move.moveToDirection.x - move.moveFromDirection.x) == 2) {
            Position rookFrom;
            Position rookTo;

            if (move.moveToDirection.x > move.moveFromDirection.x) {
                rookFrom = {7, move.moveToDirection.y};
                rookTo = {move.moveToDirection.x - 1, move.moveToDirection.y};
            } else {
                rookFrom = {0, move.moveToDirection.y};
                rookTo = {move.moveToDirection.x + 1, move.moveToDirection.y};
            }

            MoveSelectedPieceTo(rookTo, rookFrom);
            auto rook = std::static_pointer_cast<Rook>(board->GetPieceAt(rookTo));
            rook->UpdateVisualPositionFromLogical();
            if (rook) {
                rook->SetHasMoved();
            }
            return true;
        }
        return false;
    }
    return false;
}

bool GameState::PerformPromotion(const Move& move, const std::shared_ptr<Piece>& selectedPiece) {
    if (selectedPiece->GetType() == PieceType::Pawn) {
        return CheckForPawnPromotion(selectedPiece, move);
    }
    return false;
}

void GameState::HandleMoveSounds(bool didCapture, bool didMove, bool didCastle, bool didPromote, const Move& lastMove) {
    this->ChangePlayerTurn();
    bool isOpponentInCheck = this->IsKingInCheck();
    bool isOpponentInCheckMate = this->IsCheckmate(lastMove);
    this->ChangePlayerTurn();

    if (didCastle) {
        this->audioManager.PlayCastleSound();
    } else if (didPromote) {
        this->audioManager.PlayPromotionSound();
    } else if (didCapture) {
        this->audioManager.PlayCaptureSound();
    } else if (isOpponentInCheckMate) {
        this->audioManager.PlayCheckmateSound();
    } else if (isOpponentInCheck) {
        this->audioManager.PlayCheckSound();
    } else if (didMove) {
        this->audioManager.PlayMoveSound();
    }
}


void GameState::UpdateKingPosition(const Position& moveTo) {
    if (this->board->GetPieceAt(moveTo)->GetType() == PieceType::King) {
        if (this->board->GetPieceAt(moveTo)->GetColor() == PlayerColor::White) {
            this->whiteKingPosition = moveTo;
        } else {
            this->blackKingPosition = moveTo;
        }
    }
}

std::optional<std::shared_ptr<Piece>> GameState::GetCurrentPlayerSelectedPiece() {
    auto selectedPiece = this->CurrentPlayer()->GetSelectedPiece(*this->GetBoard());
    if (selectedPiece.has_value()) {
        return selectedPiece.value();
    }
    return nullptr;
}

std::vector<Move> GameState::GetAvailableMovesCurrentPlayer() {
    auto availableMoves = this->CurrentPlayer()->GetAvailableMoves();
    auto piece = this->GetCurrentPlayerSelectedPiece();
    std::vector<Move> legalMoves;
    for (const auto& move : availableMoves) {
        if (this->IsLegalMove(move.moveToDirection, *piece)) {
            legalMoves.push_back(move);
        }
    }
    return legalMoves;
}

std::shared_ptr<Player> GameState::CurrentPlayer() {
    return this->GetPlayerTurn() == PlayerColor::White ? whitePlayer : blackPlayer;
}

void GameState::InitializeBoard() {
    this->board->Populate();
}

PlayerColor GameState::GetPlayerTurn() {
    return this->playerTurn;
}

bool GameState::CheckForPawnPromotion(const std::shared_ptr<Piece>& piece, const Move& move) {
    auto pawn = dynamic_cast<Pawn*>(piece.get());
    if (pawn->CanPromote(move)) {
        this->PromotePawn(move.moveToDirection, PieceType::Queen);
        return true;
    }
    return false;
}

void GameState::CheckForPieceFirstMove(const std::shared_ptr<Piece>& piece) {
    if (std::find(moveRestrictions.begin(), moveRestrictions.end(), piece->GetType()) != moveRestrictions.end()) {
        if (auto king = dynamic_cast<King*>(piece.get())) {
            king->SetHasMoved();
        } else if (auto rook = dynamic_cast<Rook*>(piece.get())) {
            rook->SetHasMoved();
        } else if (auto pawn = dynamic_cast<Pawn*>(piece.get())) {
            pawn->SetHasMoved();
        }
        return;
    }
}