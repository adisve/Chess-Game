//
// Created by Adis Veletanlic on 2023-11-07.
//

#ifndef CHESSGAME_MOVEMANAGER_H
#define CHESSGAME_MOVEMANAGER_H

#include <vector>
#include "../Player/Move.h"

class MoveManager {
public:
    void ExecuteMove(const Move& move);

    void UndoMove();

    std::optional<Move> GetLastMove();

private:
    std::vector<Move> moveHistory;
};

#endif //CHESSGAME_MOVEMANAGER_H
