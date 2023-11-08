//
// Created by Adis Veletanlic on 2023-11-06.
//

#ifndef CHESSGAME_MOVE_H
#define CHESSGAME_MOVE_H
#include <SFML/System/Vector2.hpp>

struct Move {
    sf::Vector2i moveFromDirection;
    sf::Vector2i moveToDirection;
    sf::Vector2i attackingDirection;

    Move(const sf::Vector2i& moveToDirection, const sf::Vector2i& attackingDirection, const sf::Vector2i& moveFromDirection)
            : moveFromDirection(moveFromDirection), moveToDirection(moveToDirection), attackingDirection(attackingDirection) {}
};



#endif //CHESSGAME_MOVE_H
