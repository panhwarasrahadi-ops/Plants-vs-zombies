#include "Game.h"
#include "Constants.h"
#include "Sunflower.h"
#include "Peashooter.h"
#include "Wallnut.h"
#include "NormalZombie.h"
#include "ConeheadZombie.h"
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
      sun(STARTING_SUN),
      selectedCard(-1)
{
    window.setFramerateLimit(60);

    // load the font once, here in the constructor
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        std::cout << "Could not load font\n";

    // load every picture once; if a file is missing the game uses a shape instead
    hasZombieTex = zombieTex.loadFromFile("zombie.png");
    if (!hasZombieTex) std::cout << "zombie.png not found, using a shape\n";
    hasConeheadTex = coneheadTex.loadFromFile("conehead_zombie.png");
    if (!hasConeheadTex) std::cout << "conehead_zombie.png not found, using a shape\n";
    hasSunflowerTex = sunflowerTex.loadFromFile("sunflower.png");
    if (!hasSunflowerTex) std::cout << "sunflower.png not found, using a shape\n";
    hasPeashooterTex = peashooterTex.loadFromFile("peashooter.png");
    if (!hasPeashooterTex) std::cout << "peashooter.png not found, using a shape\n";
    hasWallnutTex = wallnutTex.loadFromFile("wallnut.png");
    if (!hasWallnutTex) std::cout << "wallnut.png not found, using a shape\n";
    hasSunTex = sunTex.loadFromFile("sun.png");
    if (!hasSunTex) std::cout << "sun.png not found, using a shape\n";
}

Game::~Game() {
    clearObjects();
}

void Game::clearObjects() {
    for (Plant* p : plants) delete p;
    for (Zombie* z : zombies) delete z;
    for (Projectile* pea : projectiles) delete pea;
    for (SunDrop* d : sunDrops) delete d;
    plants.clear();
    zombies.clear();
    projectiles.clear();
    sunDrops.clear();
}

void Game::startGame() {
    clearObjects();
    sun = STARTING_SUN;
    selectedCard = -1;
    spawnTimer = 5.f;
    zombiesSpawned = 0;
    zombiesKilled = 0;
    messageTimer = 0.f;
    state = State::Playing;
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

        // keyboard: Enter starts from the menu, R restarts after a win or loss
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Enter && state == State::Menu)
                startGame();
            if (key->code == sf::Keyboard::Key::R &&
                (state == State::Won || state == State::Lost))
                startGame();
        }

        if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
            float mx = static_cast<float>(click->position.x);
            float my = static_cast<float>(click->position.y);

            // start screen: only the Play button matters
            if (state == State::Menu) {
                if (click->button == sf::Mouse::Button::Left &&
                    mx >= PLAY_X && mx <= PLAY_X + PLAY_W &&
                    my >= PLAY_Y && my <= PLAY_Y + PLAY_H)
                    startGame();
                continue;
            }

            // win / lose screens ignore the mouse
            if (state != State::Playing)
                continue;

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
                // 1) did the player click a sun drop? collect it
                bool collected = false;
                for (SunDrop* d : sunDrops) {
                    if (!d->isDead() && d->getBounds().contains({mx, my})) {
                        sun += SUN_VALUE;
                        d->takeDamage(1);
                        collected = true;
                        break;
                    }
                }

                if (!collected) {
                    // 2) did the player click a plant card?
                    for (int i = 0; i < NUM_CARDS; i++) {
                        float cx = CARD_X + i * CARD_GAP;
                        if (mx >= cx && mx <= cx + CARD_W &&
                            my >= CARD_Y && my <= CARD_Y + CARD_H) {
                            selectedCard = i;
                        }
                    }
                    // 3) did the player click the lawn?
                    if (row >= 0)
                        placePlant(row, col);
                }
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
    std::uniform_int_distribution<int> coin(0, 2);

    int row = pickRow(rng);
    float startX = LAWN_X + COLS * CELL_W;

    // the first 3 zombies are normal; after that about 1 in 3 wears a cone
    bool cone = (zombiesSpawned >= 3 && coin(rng) == 0);
    if (cone)
        zombies.push_back(new ConeheadZombie(row, startX,
                                             hasConeheadTex ? &coneheadTex : nullptr));
    else
        zombies.push_back(new NormalZombie(row, startX,
                                           hasZombieTex ? &zombieTex : nullptr));
    zombiesSpawned++;
}

void Game::update(float dt) {
    // nothing moves on the menu, win or lose screens
    if (state != State::Playing) return;

    // message timer
    if (messageTimer > 0.f) messageTimer -= dt;

    // waves: a new zombie every few seconds, faster in the second half
    if (zombiesSpawned < TOTAL_ZOMBIES) {
        spawnTimer -= dt;
        if (spawnTimer <= 0.f) {
            spawnZombie();
            spawnTimer = (zombiesSpawned > TOTAL_ZOMBIES / 2) ? SPAWN_INTERVAL * 0.6f
                                                              : SPAWN_INTERVAL;
        }
    }

    // plants: update, shoot, and make sun
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

        if (p->wantsToProduceSun())
            sunDrops.push_back(new SunDrop(p->getX() + 22.f, p->getY() + 20.f,
                                           hasSunTex ? &sunTex : nullptr));
    }

    // sun drops age and expire
    for (SunDrop* d : sunDrops) d->update(dt);

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

        // a zombie reached the house: the player loses
        if (z->getX() < LAWN_X - 30.f)
            state = State::Lost;
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

    // count the zombies that died this frame, then remove everything dead
    for (Zombie* z : zombies)
        if (z->isDead()) zombiesKilled++;

    removeDead(plants);
    removeDead(zombies);
    removeDead(projectiles);
    removeDead(sunDrops);

    // every zombie has appeared and none are left: the player wins
    if (state == State::Playing &&
        zombiesSpawned >= TOTAL_ZOMBIES && zombies.empty())
        state = State::Won;
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

    // how many zombies are still to be killed
    sf::Text left(font, "Zombies left: " + std::to_string(TOTAL_ZOMBIES - zombiesKilled), 22);
    left.setFillColor(sf::Color::White);
    left.setPosition({760.f, 45.f});
    window.draw(left);

    // short message, for example "Not enough sun!"
    if (messageTimer > 0.f) {
        sf::Text msg(font, message, 26);
        msg.setFillColor(sf::Color(255, 120, 120));
        msg.setPosition({520.f, 45.f});
        window.draw(msg);
    }
}

void Game::drawCenteredText(const std::string& text, unsigned int size,
                            sf::Color color, float y) {
    sf::Text t(font, text, size);
    t.setFillColor(color);

    // move the text's origin to its own centre, then place it in the middle of the window
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    t.setPosition({WINDOW_W / 2.f, y});
    window.draw(t);
}

void Game::drawMenu() {
    drawCenteredText("Plants vs Zombies", 72, sf::Color::White, 140.f);
    drawCenteredText("Place your plants and stop the zombies!", 28,
                     sf::Color(235, 235, 180), 230.f);

    // Play button, lighter when the mouse is over it
    sf::Vector2i m = sf::Mouse::getPosition(window);
    bool hover = m.x >= PLAY_X && m.x <= PLAY_X + PLAY_W &&
                 m.y >= PLAY_Y && m.y <= PLAY_Y + PLAY_H;

    sf::RectangleShape button({PLAY_W, PLAY_H});
    button.setPosition({PLAY_X, PLAY_Y});
    button.setFillColor(hover ? sf::Color(100, 210, 100) : sf::Color(60, 160, 60));
    button.setOutlineThickness(4.f);
    button.setOutlineColor(sf::Color(20, 90, 20));
    window.draw(button);
    drawCenteredText("PLAY", 38, sf::Color::White, PLAY_Y + PLAY_H / 2.f - 4.f);

    // short instructions
    drawCenteredText("Click a card, then click the lawn to plant it.  Right-click removes a plant.",
                     20, sf::Color(220, 220, 220), 510.f);
    drawCenteredText("Click the suns that sunflowers make to collect them.  Press R to restart.",
                     20, sf::Color(220, 220, 220), 540.f);
}

void Game::drawEndScreen(const std::string& title, sf::Color color) {
    // dark see-through layer over the whole game
    sf::RectangleShape overlay({static_cast<float>(WINDOW_W), static_cast<float>(WINDOW_H)});
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);

    drawCenteredText(title, 90, color, 260.f);
    drawCenteredText("Press R to play again", 32, sf::Color::White, 370.f);
}

void Game::draw() {
    window.clear(sf::Color(30, 110, 30));

    if (state == State::Menu) {
        drawMenu();
        window.display();
        return;
    }

    drawLawn();

    // everything on the lawn goes under the top bar
    for (Plant* p : plants) p->draw(window);
    for (Zombie* z : zombies) z->draw(window);
    for (Projectile* pea : projectiles) pea->draw(window);
    for (SunDrop* d : sunDrops) d->draw(window);

    drawTopBar();

    if (state == State::Won)
        drawEndScreen("YOU WIN!", sf::Color(255, 220, 60));
    else if (state == State::Lost)
        drawEndScreen("GAME OVER", sf::Color(255, 90, 90));

    window.display();
}