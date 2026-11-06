#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Constants.h"

// A little sun made by a sunflower. Click it to collect SUN_VALUE sun.
// It disappears by itself after SUN_LIFETIME seconds.
class SunDrop : public GameObject {
    float age = 0.f;

public:
    SunDrop(float x, float y, const sf::Texture* texture = nullptr)
        : GameObject(x, y, 1, texture) {}

    void update(float dt) override {
        age += dt;
        if (age > SUN_LIFETIME)
            takeDamage(1);   // "dies", so Game removes it
    }

    void draw(sf::RenderWindow& window) override {
        if (!drawSprite(window, x, y, 45.f, 45.f)) {
            sf::CircleShape sunShape(22.f);
            sunShape.setPosition({x, y});
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