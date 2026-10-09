#pragma once
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update(float dt);
    void draw();
    void drawLawn();
    void drawTopBar();

    sf::RenderWindow window;
    sf::Font font;
    int sun;            // current sun amount
    int selectedCard;   // -1 means no card selected
};