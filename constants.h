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
const float LAWN_X = 100.f;
const float LAWN_Y = 150.f;

// top bar and plant cards
const float BAR_H = 120.f;
const int NUM_CARDS = 3;
const float CARD_X = 150.f;
const float CARD_Y = 20.f;
const float CARD_W = 90.f;
const float CARD_H = 80.f;
const float CARD_GAP = 110.f;

// sun cost per card: Sunflower, LipstickShooter, Wallnut
const int CARD_COSTS[NUM_CARDS] = {50, 100, 50};

// play button on the start screen
const float PLAY_X = 390.f;
const float PLAY_Y = 380.f;
const float PLAY_W = 220.f;
const float PLAY_H = 70.f;

// gameplay (times in seconds, speeds in pixels per second)
const int STARTING_SUN = 200;
const int SUN_VALUE = 25;
const float SUN_INTERVAL = 6.f;
const float SUN_FIRST_DELAY = 2.f;
const float SUN_FALL_SPEED = 110.f;
const float SUN_LIFETIME = 10.f;

const float LIPSTICK_SPEED = 300.f;
const int LIPSTICK_DAMAGE = 20;
const float SHOOT_COOLDOWN = 1.2f;

const int BITE_DAMAGE = 20;
const float BITE_INTERVAL = 1.0f;

const float ZOMBIE_SPEED = 25.f;
const int TOTAL_ZOMBIES = 20;
const float SPAWN_INTERVAL = 6.f;

// colour theme
const sf::Color COLOR_BACKGROUND(230, 120, 165);
const sf::Color COLOR_LAWN_LIGHT(255, 214, 230);
const sf::Color COLOR_LAWN_DARK(250, 178, 208);
const sf::Color COLOR_TOP_BAR(150, 40, 90);
const sf::Color COLOR_CARD(255, 238, 245);
const sf::Color COLOR_BUTTON(220, 70, 130);
const sf::Color COLOR_BUTTON_HOVER(245, 120, 170);
const sf::Color COLOR_BUTTON_OUTLINE(130, 30, 80);
const sf::Color COLOR_SUBTITLE(255, 235, 245);
