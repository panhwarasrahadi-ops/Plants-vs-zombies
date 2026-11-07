#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "GameObject.h"
#include "Constants.h"

// A little sun made by a sunflower. It falls straight down into the sunflower's own cell,
// then bobs gently. Click it to collect SUN_VALUE sun.
// It disappears SUN_LIFETIME seconds after landing.
class SunDrop : public GameObject {
    float age = 0.f;
    float targetY;      // where it stops falling

public:
    // targetY < 0 means "do not fall, stay where you are"
    SunDrop(float x, float y, const sf::Texture* texture = nullptr, float targetY = -1.f)
        : GameObject(x, y, 1, texture),
          targetY(targetY < 0.f ? y : targetY) {}

    void update(float dt) override {
        if (y < targetY) {
            y += SUN_FALL_SPEED * dt;
            if (y > targetY) y = targetY;
            return;             // still falling, so it is not ageing yet
        }
        age += dt;
        if (age > SUN_LIFETIME)
            takeDamage(1);      // "dies", so Game removes it
    }

    void draw(sf::RenderWindow& window) override {
        // bob up and down only after landing
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