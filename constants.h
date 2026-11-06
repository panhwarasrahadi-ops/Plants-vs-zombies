#pragma once

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
const int STARTING_SUN = 150;
const int SUN_VALUE = 25;            // sun gained by clicking one sun drop
const float SUN_INTERVAL = 10.f;     // seconds between a sunflower's sun drops
const float SUN_LIFETIME = 12.f;     // seconds a sun drop stays before it disappears

const float PEA_SPEED = 300.f;       // pixels per second
const int PEA_DAMAGE = 20;
const float SHOOT_COOLDOWN = 1.5f;   // seconds between peashooter shots

const int BITE_DAMAGE = 20;          // damage a zombie does to a plant per bite
const float BITE_INTERVAL = 1.0f;    // seconds between bites

const int TOTAL_ZOMBIES = 12;        // zombies in one game; kill them all to win
const float SPAWN_INTERVAL = 7.f;    // seconds between zombies (faster in the second half)