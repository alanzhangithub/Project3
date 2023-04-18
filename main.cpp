#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include "Tile.h"
#include "Board.h"
#include "TextureManager.h"
#include "WelcomeWindow.h"
#include "LeaderboardWindow.h"

using namespace sf;
using namespace std;

enum GameState {
    WELCOME_SCREEN,
    PLAYING,
    LEADERBOARD,
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 576), "Minesweeper");

    GameState currentState = WELCOME_SCREEN;

    // Initialize the TextureManager
    TextureManager textureManager;
    // Load textures using textureManager

    // Display the Welcome window
    WelcomeWindow welcomeWindow;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle user input for the current state
            switch (currentState) {
                case WELCOME_SCREEN:
                    welcomeWindow.handleEvent(event);
                    break;
                case PLAYING:
                    // Handle PLAYING state events here
                    break;
                case LEADERBOARD:
                    // Handle LEADERBOARD state events here
                    break;
            }
        }

        window.clear();

        switch (currentState) {
            case WELCOME_SCREEN:
                welcomeWindow.draw(window);
                break;
            case PLAYING:
                // Create the Board using the TextureManager
                Board board(textureManager);

                // Run the game loop
                while (!game_over) {
                    // Process user input

                    // Update the board state

                    // Draw the board
                    board.draw(window);
                }

                currentState = LEADERBOARD;
                break;
            case LEADERBOARD:
                // Display the Leaderboard window
                LeaderboardWindow leaderboardWindow;
                leaderboardWindow.draw(window);
                break;
        }

        window.display();
    }

    return 0;
}
