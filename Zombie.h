#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

class Zombie : public GameObject {
protected:
    int row;
    float speed;
    bool eating = false;
    float biteTimer = 0.f;

public:
    Zombie(int row, float startX, float speed, int health,
           const sf::Texture* texture = nullptr)
        : GameObject(startX, LAWN_Y + row * CELL_H, health, texture),
          row(row), speed(speed) {}

    int getRow() const { return row; }
    float getSpeed() const { return speed; }

    void setEating(bool e) {
        eating = e;
        if (!e) biteTimer = 0.f;
    }
    bool isEating() const { return eating; }

    bool readyToBite(float dt) {
        biteTimer += dt;
        if (biteTimer >= BITE_INTERVAL) {
            biteTimer -= BITE_INTERVAL;
            return true;
        }
        return false;
    }
};
