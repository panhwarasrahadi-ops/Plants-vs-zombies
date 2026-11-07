#pragma once
#include "Zombie.h"

// Same as NormalZombie but wears a cone, so it takes many more hits
class ConeheadZombie : public Zombie {
    static constexpr int MAX_HEALTH = 200;

public:
    ConeheadZombie(int row, float startX, const sf::Texture* texture = nullptr)
        : Zombie(row, startX, ZOMBIE_SPEED, MAX_HEALTH, texture) {}

    // walks left, but stops while it is eating
    void update(float dt) override {
        if (!eating)
            x -= speed * dt;
    }

    void draw(sf::RenderWindow& window) override {
        // picture if we have one, otherwise an orange rectangle
        if (!drawSprite(window, x, y + 5.f, CELL_W - 5.f, CELL_H - 5.f)) {
            sf::RectangleShape body({50.f, 70.f});
            body.setPosition({x + 15.f, y + 10.f});
            body.setFillColor(sf::Color(210, 130, 60));
            window.draw(body);
        }
        drawHealthBar(window, MAX_HEALTH, 15.f, 2.f, 50.f);
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x + 15.f, y + 10.f}, {50.f, 70.f});
    }
};