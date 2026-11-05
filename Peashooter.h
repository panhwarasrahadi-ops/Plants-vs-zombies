#pragma once
#include "Plant.h"

class Peashooter : public Plant {
    static constexpr int MAX_HEALTH = 100;

public:
    Peashooter(int row, int col, const sf::Texture* texture = nullptr)
        : Plant(row, col, 100, MAX_HEALTH, texture) {}

    // Week 3: this is where it will shoot when a zombie is in its row
    void update(float /*dt*/) override {}

    void draw(sf::RenderWindow& window) override {
        if (!drawSprite(window, x + 5.f, y + 8.f, CELL_W - 10.f, CELL_H - 10.f)) {
            sf::CircleShape body(35.f);
            body.setPosition({x + 10.f, y + 10.f});
            body.setFillColor(sf::Color(40, 170, 60));
            window.draw(body);

            // the "gun" sticking out to the right
            sf::RectangleShape barrel({25.f, 14.f});
            barrel.setPosition({x + 60.f, y + 38.f});
            barrel.setFillColor(sf::Color(20, 110, 40));
            window.draw(barrel);
        }
        drawHealthBar(window, MAX_HEALTH, 10.f, 2.f, CELL_W - 20.f);
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x + 10.f, y + 10.f}, {CELL_W - 20.f, CELL_H - 20.f});
    }
};