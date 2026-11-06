#include "Game.h"
#include "Constants.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "Wallnut.h"
#include "NormalZombie.h"
#include <optional>
#include <iostream>
#include <string>

// deletes every dead object in a list and removes it from the list
template <typename T>
static void removeDead(std::vector<T*>& list) {
    for (size_t i = 0; i < list.size(); ) {
        if (list[i]->isDead()) {
            delete list[i];
            list.erase(list.begin() + i);
        } else {
            i++;
        }
    }
}

Game::Game()
    : window(sf::VideoMode({WINDOW_W, WINDOW_H}), "Plants vs Zombies"),
      sun(150),
      selectedCard(-1)
{
    window.setFramerateLimit(60);

    // load the font once, here in the constructor
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        std::cout << "Could not load font\n";

    // load every picture once; if a file is missing the game uses a shape instead
    hasZombieTex = zombieTex.loadFromFile("zombie.png");
    if (!hasZombieTex) std::cout << "zombie.png not found, using a shape\n";
    hasSunflowerTex = sunflowerTex.loadFromFile("sunflower.png");
    if (!hasSunflowerTex) std::cout << "sunflower.png not found, using a shape\n";
    hasPeashooterTex = peashooterTex.loadFromFile("peashooter.png");
    if (!hasPeashooterTex) std::cout << "peashooter.png not found, using a shape\n";
    hasWallnutTex = wallnutTex.loadFromFile("wallnut.png");
    if (!hasWallnutTex) std::cout << "wallnut.png not found, using a shape\n";
}

Game::~Game() {
    for (Plant* p : plants) delete p;
    for (Zombie* z : zombies) delete z;
    for (Projectile* pea : projectiles) delete pea;
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

        if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
            float mx = static_cast<float>(click->position.x);
            float my = static_cast<float>(click->position.y);

            // turn the click into a lawn row and column (-1 means "not on the lawn")
            int row = -1, col = -1;
            if (mx >= LAWN_X && my >= LAWN_Y) {
                int c = static_cast<int>((mx - LAWN_X) / CELL_W);
                int r = static_cast<int>((my - LAWN_Y) / CELL_H);
                if (r < ROWS && c < COLS) {
                    row = r;
                    col = c;
                }
            }

            if (click->button == sf::Mouse::Button::Left) {
                // 1) did the player click a plant card?
                for (int i = 0; i < NUM_CARDS; i++) {
                    float cx = CARD_X + i * CARD_GAP;
                    if (mx >= cx && mx <= cx + CARD_W &&
                        my >= CARD_Y && my <= CARD_Y + CARD_H) {
                        selectedCard = i;
                    }
                }
                // 2) did the player click the lawn?
                if (row >= 0)
                    placePlant(row, col);
            }
            else if (click->button == sf::Mouse::Button::Right) {
                // right click removes the plant in that cell
                if (row >= 0)
                    removePlant(row, col);
            }
        }
    }
}

void Game::placePlant(int row, int col) {
    // 1) a card must be selected
    if (selectedCard < 0) return;

    // 2) the cell must be empty
    for (Plant* p : plants) {
        if (p->getRow() == row && p->getCol() == col)
            return;
    }

    // 3) there must be enough sun
    int cost = CARD_COSTS[selectedCard];
    if (sun < cost) {
        message = "Not enough sun!";
        messageTimer = 2.f;
        return;
    }

    // 4) make the plant that matches the selected card
    Plant* newPlant = nullptr;
    if (selectedCard == 0)
        newPlant = new Sunflower(row, col, hasSunflowerTex ? &sunflowerTex : nullptr);
    else if (selectedCard == 1)
        newPlant = new Peashooter(row, col, hasPeashooterTex ? &peashooterTex : nullptr);
    else if (selectedCard == 2)
        newPlant = new Wallnut(row, col, hasWallnutTex ? &wallnutTex : nullptr);

    if (newPlant) {
        plants.push_back(newPlant);
        sun -= cost;
        selectedCard = -1;   // one click = one plant
    }
}

void Game::removePlant(int row, int col) {
    for (Plant* p : plants) {
        if (p->getRow() == row && p->getCol() == col)
            p->takeDamage(p->getHealth());   // it is now dead, update() removes it
    }
}

void Game::spawnZombie() {
    std::uniform_int_distribution<int> pickRow(0, ROWS - 1);
    int row = pickRow(rng);
    zombies.push_back(new NormalZombie(row, LAWN_X + COLS * CELL_W,
                                       hasZombieTex ? &zombieTex : nullptr));
}

void Game::update(float dt) {
    // message timer
    if (messageTimer > 0.f) messageTimer -= dt;

    // new zombie every few seconds
    spawnTimer -= dt;
    if (spawnTimer <= 0.f) {
        spawnZombie();
        spawnTimer = SPAWN_INTERVAL;
    }

    // plants: update, and let peashooters shoot when a zombie is in their row
    for (Plant* p : plants) {
        p->update(dt);

        bool zombieInRow = false;
        for (Zombie* z : zombies) {
            if (z->getRow() == p->getRow() &&
                z->getX() > p->getX() && z->getX() < WINDOW_W) {
                zombieInRow = true;
                break;
            }
        }
        if (p->wantsToShoot(zombieInRow))
            projectiles.push_back(new Projectile(p->getRow(), p->getX() + 65.f,
                                                 PEA_SPEED, PEA_DAMAGE));
    }

    // zombies: stop and eat a plant that is in front of them, otherwise walk
    for (Zombie* z : zombies) {
        Plant* target = nullptr;
        for (Plant* p : plants) {
            if (p->getRow() == z->getRow() && !p->isDead() &&
                z->getBounds().findIntersection(p->getBounds()).has_value()) {
                target = p;
                break;
            }
        }

        z->setEating(target != nullptr);
        z->update(dt);

        if (target && z->readyToBite(dt))
            target->takeDamage(BITE_DAMAGE);

        // walked off the left side of the screen
        if (z->getX() < -CELL_W)
            z->takeDamage(z->getHealth());
    }

    // peas: fly right and hurt the first zombie they touch
    for (Projectile* pea : projectiles) {
        pea->update(dt);

        for (Zombie* z : zombies) {
            if (z->isDead()) continue;
            if (z->getRow() == pea->getRow() &&
                pea->getBounds().findIntersection(z->getBounds()).has_value()) {
                z->takeDamage(pea->getDamage());
                pea->takeDamage(1);   // the pea is used up
                break;
            }
        }

        if (pea->getX() > WINDOW_W)
            pea->takeDamage(1);       // flew off the screen
    }

    // remove everything that died this frame
    removeDead(plants);
    removeDead(zombies);
    removeDead(projectiles);
}

void Game::drawLawn() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            sf::RectangleShape cell({CELL_W, CELL_H});
            cell.setPosition({LAWN_X + c * CELL_W, LAWN_Y + r * CELL_H});

            // two shades of green, like a checkerboard
            if ((r + c) % 2 == 0)
                cell.setFillColor(sf::Color(110, 200, 80));
            else
                cell.setFillColor(sf::Color(80, 170, 60));

            window.draw(cell);
        }
    }
}

void Game::drawTopBar() {
    // brown bar across the top
    sf::RectangleShape bar({static_cast<float>(WINDOW_W), BAR_H});
    bar.setFillColor(sf::Color(90, 60, 30));
    window.draw(bar);

    // sun counter
    sf::Text sunText(font, "Sun: " + std::to_string(sun), 28);
    sunText.setFillColor(sf::Color::White);
    sunText.setPosition({20.f, 40.f});
    window.draw(sunText);

    // the three plant cards
    const char* names[NUM_CARDS] = {"Sunflower", "Peashooter", "Wallnut"};

    for (int i = 0; i < NUM_CARDS; i++) {
        float cx = CARD_X + i * CARD_GAP;

        sf::RectangleShape card({CARD_W, CARD_H});
        card.setPosition({cx, CARD_Y});
        card.setFillColor(sf::Color(230, 215, 150));

        // red outline on the selected card
        if (i == selectedCard) {
            card.setOutlineThickness(4.f);
            card.setOutlineColor(sf::Color::Red);
        }
        window.draw(card);

        sf::Text label(font, std::string(names[i]) + "\n" + std::to_string(CARD_COSTS[i]), 15);
        label.setFillColor(sf::Color::Black);
        label.setPosition({cx + 6.f, CARD_Y + 10.f});
        window.draw(label);
    }

    // short message, for example "Not enough sun!"
    if (messageTimer > 0.f) {
        sf::Text msg(font, message, 26);
        msg.setFillColor(sf::Color(255, 120, 120));
        msg.setPosition({520.f, 45.f});
        window.draw(msg);
    }
}

void Game::draw() {
    window.clear(sf::Color(30, 110, 30));
    drawLawn();

    // plants, zombies and peas go on top of the lawn but under the top bar
    for (Plant* p : plants) p->draw(window);
    for (Zombie* z : zombies) z->draw(window);
    for (Projectile* pea : projectiles) pea->draw(window);

    drawTopBar();
    window.display();
}