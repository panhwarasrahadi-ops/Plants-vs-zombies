#pragma once
#include <SFML/Graphics.hpp> // graphics
class Game { // makes the class named game
    public: // this can be used outside class
    Game () ; // constructor runs when u make an object , so it is used for the setup
    void run(); //main fuction runs which is called in main.cpp
    private :  // only can be used within the function
    void handleEvents(); // will keep track of clicks
    void update(float dt); // will change things in each frame , dt=time since the last frame in seconds
    void draw(); //paints the screen shows what we want player to see , sooo shows the results after updates
     sf::RenderWindow window; // works as application window , render means draw , window means box so the box u can draw on



};