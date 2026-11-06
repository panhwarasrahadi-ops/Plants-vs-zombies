#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

// A pea shot by a Peashooter. It flies right until it hits a zombie or leaves the screen.
class Projectile : public GameObject {
protected:
    int row;
    int damage;
    float speed;    // pixels per second, moving right

public:
    Projectile(int row, float startX, float speed, int damage)
        : GameObject(startX, LAWN_Y + row * CELL_H + 27.f, 1),
          row(row), damage(damage), speed(speed) {}

    void update(float dt) override {
        x += speed * dt;
    }

    void draw(sf::RenderWindow& window) override {
        sf::CircleShape pea(9.f);
        pea.setPosition({x, y});
        pea.setFillColor(sf::Color(90, 210, 70));
        pea.setOutlineThickness(2.f);
        pea.setOutlineColor(sf::Color(20, 100, 20));
        window.draw(pea);
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x, y}, {18.f, 18.f});
    }

    int getRow() const { return row; }
    int getDamage() const { return damage; }
};