#pragma once
#include "Zombie.h"

class NormalZombie : public Zombie {
    static constexpr int MAX_HEALTH = 100;

public:
    // startX is where it appears on the screen (usually the right edge of the lawn)
    NormalZombie(int row, float startX, const sf::Texture* texture = nullptr)
        : Zombie(row, startX, 30.f, MAX_HEALTH, texture) {}

    // walks left: distance = speed * time
    void update(float dt) override {
        x -= speed * dt;
    }

    void draw(sf::RenderWindow& window) override {
        // picture if we have one, otherwise a grey-green rectangle
        if (!drawSprite(window, x, y + 5.f, CELL_W - 5.f, CELL_H - 5.f)) {
            sf::RectangleShape body({50.f, 70.f});
            body.setPosition({x + 15.f, y + 10.f});
            body.setFillColor(sf::Color(110, 140, 110));
            window.draw(body);
        }
        drawHealthBar(window, MAX_HEALTH, 15.f, 2.f, 50.f);
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x + 15.f, y + 10.f}, {50.f, 70.f});
    }
};