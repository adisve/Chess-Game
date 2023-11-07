//
// Created by Adis Veletanlic on 2023-11-07.
//

#include "MoveManager.h"
#include <iostream>

void MoveManager::ExecuteMove(const Move &move) {
    std::cout << "Executing move: (" << move.moveToDirection.x << ", " << move.moveToDirection.y << ")" << std::endl;
    std::cout << "Attacking at (" << move.attackingDirection.x << ", " << move.attackingDirection.y << ")" << std::endl;
    this->moveHistory.push_back(move);
}

void MoveManager::UndoMove() {
    if (!this->moveHistory.empty()) {
        this->moveHistory.pop_back();
    }
}

std::optional<Move> MoveManager::GetLastMove() {
    if (!this->moveHistory.empty()) {
        return moveHistory.back();
    }
    return std::nullopt;
}
