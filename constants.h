#pragma once
#include <SFML/Graphics/Color.hpp>

// window
const unsigned int WINDOW_W = 1000;
const unsigned int WINDOW_H = 650;

// lawn grid
const int ROWS = 5;
const int COLS = 9;
const float CELL_W = 90.f;
const float CELL_H = 90.f;
const float LAWN_X = 100.f;   // left edge of the lawn
const float LAWN_Y = 150.f;   // top edge of the lawn

// top bar
const float BAR_H = 120.f;

// plant cards in the top bar
const int NUM_CARDS = 3;
const float CARD_X = 150.f;   // x of the first card
const float CARD_Y = 20.f;
const float CARD_W = 90.f;
const float CARD_H = 80.f;
const float CARD_GAP = 110.f; // distance from one card to the next

// sun cost of each card: 0 = Sunflower, 1 = Peashooter, 2 = Wallnut
const int CARD_COSTS[NUM_CARDS] = {50, 100, 50};

// start screen Play button
const float PLAY_X = 390.f;
const float PLAY_Y = 380.f;
const float PLAY_W = 220.f;
const float PLAY_H = 70.f;

// gameplay numbers (easy to change in one place)
const int STARTING_SUN = 200;
const int SUN_VALUE = 25;            // sun gained by clicking one sun drop
const float SUN_INTERVAL = 6.f;      // seconds between a sunflower's sun drops
const float SUN_FIRST_DELAY = 2.f;   // seconds before a NEW sunflower makes its first sun
const float SUN_FALL_SPEED = 110.f;   // pixels per second a sun falls into the sunflower's cell
const float SUN_LIFETIME = 10.f;     // seconds a sun drop stays before it disappears

const float PEA_SPEED = 300.f;       // pixels per second
const int PEA_DAMAGE = 20;
const float SHOOT_COOLDOWN = 1.2f;   // seconds between peashooter shots

const int BITE_DAMAGE = 20;          // damage a zombie does to a plant per bite
const float BITE_INTERVAL = 1.0f;    // seconds between bites

const float ZOMBIE_SPEED = 25.f;       // pixels per second (lower = slower zombies)
const int TOTAL_ZOMBIES = 20;        // zombies in one game; kill them all to win
const float SPAWN_INTERVAL = 6.f;    // seconds between zombies (faster in the second half)

// colour theme (pink). Change these to re-theme the whole game.
const sf::Color COLOR_BACKGROUND(230, 120, 165);      // margin around the lawn
const sf::Color COLOR_LAWN_LIGHT(255, 214, 230);      // checkerboard, light cells
const sf::Color COLOR_LAWN_DARK(250, 178, 208);       // checkerboard, dark cells
const sf::Color COLOR_TOP_BAR(150, 40, 90);           // bar with sun and cards
const sf::Color COLOR_CARD(255, 238, 245);            // plant cards
const sf::Color COLOR_BUTTON(220, 70, 130);           // Play button
const sf::Color COLOR_BUTTON_HOVER(245, 120, 170);    // Play button under the mouse
const sf::Color COLOR_BUTTON_OUTLINE(130, 30, 80);
const sf::Color COLOR_SUBTITLE(255, 235, 245);        // text under the title