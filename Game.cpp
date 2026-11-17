#include "Game.h"
#include "Constants.h"
#include "Sunflower.h"
#include "LipstickShooter.h"
#include "Wallnut.h"
#include "NormalZombie.h"
#include "ConeheadZombie.h"
#include <optional>
#include <iostream>
#include <string>

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

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        std::cout << "Could not load font\n";

    // if a picture file is missing, objects draw a plain shape instead
    hasZombieTex = zombieTex.loadFromFile("zombie.png");
    if (!hasZombieTex) std::cout << "zombie.png not found, using a shape\n";
    hasConeheadTex = coneheadTex.loadFromFile("conehead_zombie.png");
    if (!hasConeheadTex) std::cout << "conehead_zombie.png not found, using a shape\n";
    hasSunflowerTex = sunflowerTex.loadFromFile("sunflower.png");
    if (!hasSunflowerTex) std::cout << "sunflower.png not found, using a shape\n";
    hasLipstickShooterTex = lipstickShooterTex.loadFromFile("lipstick_shooter.png");
    if (!hasLipstickShooterTex) std::cout << "lipstick_shooter.png not found, using a shape\n";
    hasWallnutTex = wallnutTex.loadFromFile("wallnut.png");
    if (!hasWallnutTex) std::cout << "wallnut.png not found, using a shape\n";
    hasSunTex = sunTex.loadFromFile("sun.png");
    if (!hasSunTex) std::cout << "sun.png not found, using a shape\n";
    hasLipstickTex = lipstickTex.loadFromFile("lipstick.png");
    if (!hasLipstickTex) std::cout << "lipstick.png not found, using a shape\n";
}

Game::~Game() {
    clearObjects();
}

void Game::clearObjects() {
    for (Plant* p : plants) delete p;
    for (Zombie* z : zombies) delete z;
    for (Projectile* shot : projectiles) delete shot;
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

            if (state == State::Menu) {
                if (click->button == sf::Mouse::Button::Left &&
                    mx >= PLAY_X && mx <= PLAY_X + PLAY_W &&
                    my >= PLAY_Y && my <= PLAY_Y + PLAY_H)
                    startGame();
                continue;
            }

            if (state != State::Playing)
                continue;

            // row and col stay -1 when the click is not on the lawn
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
                    for (int i = 0; i < NUM_CARDS; i++) {
                        float cx = CARD_X + i * CARD_GAP;
                        if (mx >= cx && mx <= cx + CARD_W &&
                            my >= CARD_Y && my <= CARD_Y + CARD_H) {
                            selectedCard = i;
                        }
                    }
                    if (row >= 0)
                        placePlant(row, col);
                }
            }
            else if (click->button == sf::Mouse::Button::Right) {
                if (row >= 0)
                    removePlant(row, col);
            }
        }
    }
}

void Game::placePlant(int row, int col) {
    if (selectedCard < 0) return;

    for (Plant* p : plants) {
        if (p->getRow() == row && p->getCol() == col)
            return;
    }

    int cost = CARD_COSTS[selectedCard];
    if (sun < cost) {
        message = "Not enough sun!";
        messageTimer = 2.f;
        return;
    }

    Plant* newPlant = nullptr;
    if (selectedCard == 0)
        newPlant = new Sunflower(row, col, hasSunflowerTex ? &sunflowerTex : nullptr);
    else if (selectedCard == 1)
        newPlant = new LipstickShooter(row, col, hasLipstickShooterTex ? &lipstickShooterTex : nullptr);
    else if (selectedCard == 2)
        newPlant = new Wallnut(row, col, hasWallnutTex ? &wallnutTex : nullptr);

    if (newPlant) {
        plants.push_back(newPlant);
        sun -= cost;
        selectedCard = -1;
    }
}

void Game::removePlant(int row, int col) {
    for (Plant* p : plants) {
        if (p->getRow() == row && p->getCol() == col)
            p->takeDamage(p->getHealth());
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
    if (state != State::Playing) return;

    if (messageTimer > 0.f) messageTimer -= dt;

    if (zombiesSpawned < TOTAL_ZOMBIES) {
        spawnTimer -= dt;
        if (spawnTimer <= 0.f) {
            spawnZombie();
            spawnTimer = (zombiesSpawned > TOTAL_ZOMBIES / 2) ? SPAWN_INTERVAL * 0.6f
                                                              : SPAWN_INTERVAL;
        }
    }

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
                                                 LIPSTICK_SPEED, LIPSTICK_DAMAGE,
                                                 hasLipstickTex ? &lipstickTex : nullptr));

        if (p->wantsToProduceSun())
            sunDrops.push_back(new SunDrop(p->getX() + 40.f, p->getY() - 50.f,
                                           hasSunTex ? &sunTex : nullptr,
                                           p->getY() + 5.f));
    }

    for (SunDrop* d : sunDrops) d->update(dt);

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

        if (z->getX() < LAWN_X - 30.f)
            state = State::Lost;
    }

    for (Projectile* shot : projectiles) {
        shot->update(dt);

        for (Zombie* z : zombies) {
            if (z->isDead()) continue;
            if (z->getRow() == shot->getRow() &&
                shot->getBounds().findIntersection(z->getBounds()).has_value()) {
                z->takeDamage(shot->getDamage());
                shot->takeDamage(1);
                break;
            }
        }

        if (shot->getX() > WINDOW_W)
            shot->takeDamage(1);
    }

    // dead objects are removed only here, after every loop above has finished
    for (Zombie* z : zombies)
        if (z->isDead()) zombiesKilled++;

    removeDead(plants);
    removeDead(zombies);
    removeDead(projectiles);
    removeDead(sunDrops);

    if (state == State::Playing &&
        zombiesSpawned >= TOTAL_ZOMBIES && zombies.empty())
        state = State::Won;
}

void Game::drawLawn() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            sf::RectangleShape cell({CELL_W, CELL_H});
            cell.setPosition({LAWN_X + c * CELL_W, LAWN_Y + r * CELL_H});

            if ((r + c) % 2 == 0)
                cell.setFillColor(COLOR_LAWN_LIGHT);
            else
                cell.setFillColor(COLOR_LAWN_DARK);

            window.draw(cell);
        }
    }
}

void Game::drawTopBar() {
    sf::RectangleShape bar({static_cast<float>(WINDOW_W), BAR_H});
    bar.setFillColor(COLOR_TOP_BAR);
    window.draw(bar);

    sf::Text sunText(font, "Sun: " + std::to_string(sun), 28);
    sunText.setFillColor(sf::Color::White);
    sunText.setPosition({20.f, 40.f});
    window.draw(sunText);

    const char* names[NUM_CARDS] = {"Sunflower", "Lipstick\nShooter", "Wallnut"};

    for (int i = 0; i < NUM_CARDS; i++) {
        float cx = CARD_X + i * CARD_GAP;

        sf::RectangleShape card({CARD_W, CARD_H});
        card.setPosition({cx, CARD_Y});
        card.setFillColor(COLOR_CARD);

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

    sf::Text left(font, "Zombies left: " + std::to_string(TOTAL_ZOMBIES - zombiesKilled), 22);
    left.setFillColor(sf::Color::White);
    left.setPosition({760.f, 45.f});
    window.draw(left);

    if (messageTimer > 0.f) {
        sf::Text msg(font, message, 26);
        msg.setFillColor(sf::Color(255, 235, 130));
        msg.setPosition({520.f, 45.f});
        window.draw(msg);
    }
}

void Game::drawCenteredText(const std::string& text, unsigned int size,
                            sf::Color color, float y) {
    sf::Text t(font, text, size);
    t.setFillColor(color);

    // centre the text horizontally in the window
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    t.setPosition({WINDOW_W / 2.f, y});
    window.draw(t);
}

void Game::drawMenu() {
    drawCenteredText("Plants vs Zombies", 72, sf::Color::White, 140.f);
    drawCenteredText("Place your plants and stop the zombies!", 28,
                     COLOR_SUBTITLE, 230.f);

    sf::Vector2i m = sf::Mouse::getPosition(window);
    bool hover = m.x >= PLAY_X && m.x <= PLAY_X + PLAY_W &&
                 m.y >= PLAY_Y && m.y <= PLAY_Y + PLAY_H;

    sf::RectangleShape button({PLAY_W, PLAY_H});
    button.setPosition({PLAY_X, PLAY_Y});
    button.setFillColor(hover ? COLOR_BUTTON_HOVER : COLOR_BUTTON);
    button.setOutlineThickness(4.f);
    button.setOutlineColor(COLOR_BUTTON_OUTLINE);
    window.draw(button);
    drawCenteredText("PLAY", 38, sf::Color::White, PLAY_Y + PLAY_H / 2.f - 4.f);

    drawCenteredText("Click a card, then click the lawn to plant it.  Right-click removes a plant.",
                     20, sf::Color::White, 510.f);
    drawCenteredText("Click the suns that sunflowers make to collect them.  Press R to restart.",
                     20, sf::Color::White, 540.f);
}

void Game::drawEndScreen(const std::string& title, sf::Color color) {
    sf::RectangleShape overlay({static_cast<float>(WINDOW_W), static_cast<float>(WINDOW_H)});
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);

    drawCenteredText(title, 90, color, 260.f);
    drawCenteredText("Press R to play again", 32, sf::Color::White, 370.f);
}

void Game::draw() {
    window.clear(COLOR_BACKGROUND);

    if (state == State::Menu) {
        drawMenu();
        window.display();
        return;
    }

    drawLawn();

    for (Plant* p : plants) p->draw(window);
    for (Zombie* z : zombies) z->draw(window);
    for (Projectile* shot : projectiles) shot->draw(window);
    for (SunDrop* d : sunDrops) d->draw(window);

    drawTopBar();

    if (state == State::Won)
        drawEndScreen("YOU WIN!", sf::Color(255, 220, 60));
    else if (state == State::Lost)
        drawEndScreen("GAME OVER", sf::Color(255, 90, 90));

    window.display();
}
