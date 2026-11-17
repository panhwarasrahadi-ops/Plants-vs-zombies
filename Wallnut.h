#pragma once
#include "Plant.h"

class Wallnut : public Plant {
    static constexpr int MAX_HEALTH = 400;

public:
    Wallnut(int row, int col, const sf::Texture* texture = nullptr)
        : Plant(row, col, 50, MAX_HEALTH, texture) {}

    void update(float /*dt*/) override {}

    void draw(sf::RenderWindow& window) override {
        if (!drawSprite(window, x + 5.f, y + 8.f, CELL_W - 10.f, CELL_H - 10.f)) {
            sf::RectangleShape body({CELL_W - 30.f, CELL_H - 20.f});
            body.setPosition({x + 15.f, y + 10.f});
            body.setFillColor(sf::Color(150, 100, 50));
            window.draw(body);
        }
        drawHealthBar(window, MAX_HEALTH, 10.f, 2.f, CELL_W - 20.f);
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x + 15.f, y + 10.f}, {CELL_W - 30.f, CELL_H - 20.f});
    }
};
