#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

// Base class for Sunflower, Peashooter and Wallnut
class Plant : public GameObject {
protected:
    int row, col;
    int cost;

public:
    Plant(int row, int col, int cost, int health, const sf::Texture* texture = nullptr)
        : GameObject(LAWN_X + col * CELL_W, LAWN_Y + row * CELL_H, health, texture),
          row(row), col(col), cost(cost) {}

    int getRow() const { return row; }
    int getCol() const { return col; }
    int getCost() const { return cost; }
};