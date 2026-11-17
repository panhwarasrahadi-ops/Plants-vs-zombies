#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class GameObject {
protected:
    float x, y;
    int health;
    const sf::Texture* texture;  // nullptr = draw a shape instead

    void drawHealthBar(sf::RenderWindow& window, int maxHealth,
                       float offsetX, float offsetY, float width) const {
        float ratio = (health > 0) ? static_cast<float>(health) / maxHealth : 0.f;

        sf::RectangleShape back({width, 6.f});
        back.setPosition({x + offsetX, y + offsetY});
        back.setFillColor(sf::Color(150, 0, 0));
        window.draw(back);

        sf::RectangleShape front({width * ratio, 6.f});
        front.setPosition({x + offsetX, y + offsetY});
        front.setFillColor(sf::Color(0, 200, 0));
        window.draw(front);
    }

    bool drawSprite(sf::RenderWindow& window,
                    float boxX, float boxY, float boxW, float boxH) const {
        if (texture == nullptr) return false;

        sf::Vector2u size = texture->getSize();
        if (size.x == 0 || size.y == 0) return false;

        float scale = std::min(boxW / static_cast<float>(size.x),
                               boxH / static_cast<float>(size.y));
        float w = size.x * scale;
        float h = size.y * scale;

        sf::Sprite sprite(*texture);
        sprite.setScale({scale, scale});
        sprite.setPosition({boxX + (boxW - w) / 2.f, boxY + boxH - h});
        window.draw(sprite);
        return true;
    }

public:
    GameObject(float x, float y, int health, const sf::Texture* texture = nullptr)
        : x(x), y(y), health(health), texture(texture) {}

    virtual ~GameObject() {}

    // every child class must implement these
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;

    void takeDamage(int amount) { health -= amount; }
    bool isDead() const { return health <= 0; }

    float getX() const { return x; }
    float getY() const { return y; }
    int getHealth() const { return health; }
};
