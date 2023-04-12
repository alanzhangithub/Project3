#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Tile.h"
#include "Functions.h"
using namespace sf;
using namespace std;

// Add an enumeration for game states
enum GameState {
    WELCOME_SCREEN,
    PLAYING,
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 576), "Minesweeper");

    Controller gamemaster;

    // Create a variable for the current game state and set it to the welcome screen
    GameState currentState = WELCOME_SCREEN;

    // Create a welcome screen text object
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        return -1; // Failed to load font
    }
    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("Welcome to Minesweeper!\nPress Enter to start.");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::Black);
    welcomeText.setPosition(200, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle key presses
            if (event.type == sf::Event::KeyPressed) {
                if (currentState == WELCOME_SCREEN && event.key.code == sf::Keyboard::Enter) {
                    currentState = PLAYING;
                }
            }
        }

        window.clear(Color(255, 255, 255, 255));

        // Use a switch-case statement to handle different game states
        switch (currentState) {
            case WELCOME_SCREEN:
                window.draw(welcomeText);
                break;
            case PLAYING:
                gamemaster.update(window);
                break;
        }

        window.display();
    }

    return 0;
}
