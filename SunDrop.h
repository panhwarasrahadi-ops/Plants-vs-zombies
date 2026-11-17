#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "GameObject.h"
#include "Constants.h"

class SunDrop : public GameObject {
    float age = 0.f;
    float targetY;

public:
    // targetY < 0 means: do not fall
    SunDrop(float x, float y, const sf::Texture* texture = nullptr, float targetY = -1.f)
        : GameObject(x, y, 1, texture),
          targetY(targetY < 0.f ? y : targetY) {}

    void update(float dt) override {
        if (y < targetY) {
            y += SUN_FALL_SPEED * dt;
            if (y > targetY) y = targetY;
            return;
        }
        age += dt;
        if (age > SUN_LIFETIME)
            takeDamage(1);
    }

    void draw(sf::RenderWindow& window) override {
        bool landed = (y >= targetY);
        float bob = landed ? 4.f * std::sin(age * 4.f) : 0.f;

        if (!drawSprite(window, x, y + bob, 45.f, 45.f)) {
            sf::CircleShape sunShape(22.f);
            sunShape.setPosition({x, y + bob});
            sunShape.setFillColor(sf::Color(255, 210, 40));
            sunShape.setOutlineThickness(3.f);
            sunShape.setOutlineColor(sf::Color(240, 140, 20));
            window.draw(sunShape);
        }
    }

    sf::FloatRect getBounds() const override {
        return sf::FloatRect({x, y}, {45.f, 45.f});
    }
};
