#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

// Base class for NormalZombie and ConeheadZombie
class Zombie : public GameObject {
protected:
    int row;
    float speed;    // pixels per second, moving left

public:
    Zombie(int row, float startX, float speed, int health,
           const sf::Texture* texture = nullptr)
        : GameObject(startX, LAWN_Y + row * CELL_H, health, texture),
          row(row), speed(speed) {}

    int getRow() const { return row; }
    float getSpeed() const { return speed; }
};