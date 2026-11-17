#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

class Projectile : public GameObject {
protected:
    int row;
    int damage;
    float speed;

public:
    Projectile(int row, float startX, float speed, int damage,
               const sf::Texture* texture = nullptr)
        : GameObject(startX, LAWN_Y + row * CELL_H + 27.f, 1, texture),
          row(row), damage(damage), speed(speed) {}

    void update(float dt) override {
        x += speed * dt;
    }

    void draw(sf::RenderWindow& window) override {
        if (!drawSprite(window, x, y - 6.f, 40.f, 30.f)) {
            sf::CircleShape dot(9.f);
            dot.setPosition({x, y});
            dot.setFillColor(sf::Color(90, 210, 70));
            dot.setOutlineThickness(2.f);
            dot.setOutlineColor(sf::Color(20, 100, 20));
            window.draw(dot);
        }
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x, y}, {36.f, 18.f});
    }

    int getRow() const { return row; }
    int getDamage() const { return damage; }
};
