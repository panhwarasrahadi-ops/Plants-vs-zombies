#pragma once
#include "Plant.h"

class Sunflower : public Plant {
    static constexpr int MAX_HEALTH = 100;
    float sunTimer = SUN_FIRST_DELAY;

public:
    Sunflower(int row, int col, const sf::Texture* texture = nullptr)
        : Plant(row, col, 50, MAX_HEALTH, texture) {}

    void update(float dt) override {
        if (sunTimer > 0.f) sunTimer -= dt;
    }

    bool wantsToProduceSun() override {
        if (sunTimer <= 0.f) {
            sunTimer = SUN_INTERVAL;
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) override {
        if (!drawSprite(window, x + 5.f, y + 8.f, CELL_W - 10.f, CELL_H - 10.f)) {
            sf::CircleShape body(35.f);
            body.setPosition({x + 10.f, y + 10.f});
            body.setFillColor(sf::Color(255, 220, 0));
            window.draw(body);
        }
        drawHealthBar(window, MAX_HEALTH, 10.f, 2.f, CELL_W - 20.f);
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x + 10.f, y + 10.f}, {CELL_W - 20.f, CELL_H - 20.f});
    }
};
