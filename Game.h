#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Plant.h"
#include "Zombie.h"

class Game {
public:
    Game();
    ~Game();                               // frees all plants and zombies
    Game(const Game&) = delete;            // a Game must not be copied
    Game& operator=(const Game&) = delete; // (it owns raw pointers)

    void run();

private:
    void handleEvents();
    void update(float dt);
    void draw();
    void drawLawn();
    void drawTopBar();
    void placePlant(int row, int col);

    sf::RenderWindow window;
    sf::Font font;
    int sun;            // current sun amount
    int selectedCard;   // -1 means no card selected

    // pictures, loaded once in the constructor and shared by all objects
    sf::Texture zombieTex, sunflowerTex, peashooterTex, wallnutTex;
    bool hasZombieTex = false;
    bool hasSunflowerTex = false;
    bool hasPeashooterTex = false;
    bool hasWallnutTex = false;

    std::vector<Plant*> plants;     // Plant* so Sunflower, Peashooter and Wallnut fit in one list
    std::vector<Zombie*> zombies;
};