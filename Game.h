#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include "Plant.h"
#include "Zombie.h"
#include "Projectile.h"
#include "SunDrop.h"

class Game {
public:
    Game();
    ~Game();                               // frees every object
    Game(const Game&) = delete;            // a Game must not be copied
    Game& operator=(const Game&) = delete; // (it owns raw pointers)

    void run();

private:
    // which screen the game is on
    enum class State { Menu, Playing, Won, Lost };

    void handleEvents();
    void update(float dt);
    void draw();
    void drawLawn();
    void drawTopBar();
    void drawMenu();
    void drawEndScreen(const std::string& title, sf::Color color);
    void drawCenteredText(const std::string& text, unsigned int size,
                          sf::Color color, float y);

    void startGame();       // resets everything and starts a fresh game
    void clearObjects();    // deletes all plants, zombies, peas and suns
    void placePlant(int row, int col);
    void removePlant(int row, int col);
    void spawnZombie();

    sf::RenderWindow window;
    sf::Font font;
    int sun;            // current sun amount
    int selectedCard;   // -1 means no card selected
    State state = State::Menu;

    // pictures, loaded once in the constructor and shared by all objects
    sf::Texture zombieTex, coneheadTex, sunflowerTex, peashooterTex, wallnutTex, sunTex;
    bool hasZombieTex = false;
    bool hasConeheadTex = false;
    bool hasSunflowerTex = false;
    bool hasPeashooterTex = false;
    bool hasWallnutTex = false;
    bool hasSunTex = false;

    std::vector<Plant*> plants;          // Plant* so different plant types fit in one list
    std::vector<Zombie*> zombies;        // Zombie* so Normal and Conehead fit in one list
    std::vector<Projectile*> projectiles;
    std::vector<SunDrop*> sunDrops;

    float spawnTimer = 5.f;              // seconds until the next zombie appears
    int zombiesSpawned = 0;
    int zombiesKilled = 0;
    std::mt19937 rng{std::random_device{}()};

    std::string message;                 // short message shown in the top bar
    float messageTimer = 0.f;
};