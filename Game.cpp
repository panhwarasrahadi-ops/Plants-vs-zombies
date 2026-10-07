#include "Game.h"
#include "Constants.h"
#include <optional>

Game::Game()
    : window(sf::VideoMode({WINDOW_W, WINDOW_H}), "Plants vs Zombies")
{
    window.setFramerateLimit(60);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        draw();
    }
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update(float dt) {
    // empty for now
}

void Game::draw() {
    window.clear(sf::Color(30, 110, 30));
    window.display();
}