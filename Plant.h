#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

class Plant : public GameObject {
protected:
    int row, col;
    int cost;

public:
    Plant(int row, int col, int cost, int health, const sf::Texture* texture = nullptr)
        : GameObject(LAWN_X + col * CELL_W, LAWN_Y + row * CELL_H, health, texture),
          row(row), col(col), cost(cost) {}

    // Game asks every plant these two questions each frame; the default answer is no
    virtual bool wantsToShoot(bool /*zombieInRow*/) { return false; }
    virtual bool wantsToProduceSun() { return false; }

    int getRow() const { return row; }
    int getCol() const { return col; }
    int getCost() const { return cost; }
};
