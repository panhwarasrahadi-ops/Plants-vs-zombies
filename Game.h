#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include "Plant.h"
#include "Zombie.h"
#include "Projectile.h"

class Game {
public:
    Game();
    ~Game();                               // frees all plants, zombies and peas
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
    void removePlant(int row, int col);
    void spawnZombie();

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

    std::vector<Plant*> plants;          // Plant* so different plant types fit in one list
    std::vector<Zombie*> zombies;
    std::vector<Projectile*> projectiles;

    float spawnTimer = 3.f;              // seconds until the next zombie appears
    std::mt19937 rng{std::random_device{}()};

    std::string message;                 // short message shown in the top bar
    float messageTimer = 0.f;
};