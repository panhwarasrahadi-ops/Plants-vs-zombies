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
    ~Game();
    Game(const Game&) = delete;  // Game owns raw pointers, so it must not be copied
    Game& operator=(const Game&) = delete;

    void run();

private:
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

    void startGame();
    void clearObjects();
    void placePlant(int row, int col);
    void removePlant(int row, int col);
    void spawnZombie();

    sf::RenderWindow window;
    sf::Font font;
    int sun;
    int selectedCard;  // -1 = no card selected
    State state = State::Menu;

    sf::Texture zombieTex, coneheadTex, sunflowerTex, lipstickShooterTex, wallnutTex, sunTex,
                lipstickTex;
    bool hasZombieTex = false;
    bool hasConeheadTex = false;
    bool hasSunflowerTex = false;
    bool hasLipstickShooterTex = false;
    bool hasWallnutTex = false;
    bool hasSunTex = false;
    bool hasLipstickTex = false;

    std::vector<Plant*> plants;  // base-class pointers, so one list holds every plant type
    std::vector<Zombie*> zombies;
    std::vector<Projectile*> projectiles;
    std::vector<SunDrop*> sunDrops;

    float spawnTimer = 5.f;
    int zombiesSpawned = 0;
    int zombiesKilled = 0;
    std::mt19937 rng{std::random_device{}()};

    std::string message;
    float messageTimer = 0.f;
};
