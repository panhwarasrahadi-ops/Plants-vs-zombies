#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

// A pea shot by a Peashooter (full behaviour written in Week 3)
class Projectile : public GameObject {
protected:
    int row;
    int damage;
    float speed;    // pixels per second, moving right

public:
    Projectile(int row, float startX, float speed, int damage)
        : GameObject(startX, LAWN_Y + row * CELL_H + CELL_H / 2.f, 1),
          row(row), damage(damage), speed(speed) {}

    int getRow() const { return row; }
    int getDamage() const { return damage; }
    float getSpeed() const { return speed; }
};