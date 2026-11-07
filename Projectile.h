#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

// A lipstick shot by a Peashooter. It flies right until it hits a zombie or leaves the screen.
class Projectile : public GameObject {
protected:
    int row;
    int damage;
    float speed;    // pixels per second, moving right

public:
    Projectile(int row, float startX, float speed, int damage,
               const sf::Texture* texture = nullptr)
        : GameObject(startX, LAWN_Y + row * CELL_H + 27.f, 1, texture),
          row(row), damage(damage), speed(speed) {}

    void update(float dt) override {
        x += speed * dt;
    }

    void draw(sf::RenderWindow& window) override {
        // picture if we have one, otherwise a green circle
        if (!drawSprite(window, x, y - 6.f, 40.f, 30.f)) {
            sf::CircleShape pea(9.f);
            pea.setPosition({x, y});
            pea.setFillColor(sf::Color(90, 210, 70));
            pea.setOutlineThickness(2.f);
            pea.setOutlineColor(sf::Color(20, 100, 20));
            window.draw(pea);
        }
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x, y}, {36.f, 18.f});
    }

    int getRow() const { return row; }
    int getDamage() const { return damage; }
};