#include <SFML/Graphics.hpp>

#include <string>

#include <fstream>

#include <cctype>

#include "TextureManager.h"

using namespace sf;
using namespace std;

enum GameState {
    WELCOME_SCREEN,
    PLAYING,
    LEADERBOARD,
};

void setText(sf::Text & text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper", sf::Style::Close);

    TextureManager initGame;
    initGame.load_board_config();


    cout << initGame.width << "and" << initGame.height;



    GameState currentState = WELCOME_SCREEN;

    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        return -1;
    }

    // Welcome Text
    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);
    setText(welcomeText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 150);

    // Input Prompt Text
    sf::Text inputPromptText;
    inputPromptText.setFont(font);
    inputPromptText.setString("Enter your name:");
    inputPromptText.setCharacterSize(20);
    inputPromptText.setStyle(sf::Text::Bold);
    inputPromptText.setFillColor(sf::Color::White);
    setText(inputPromptText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 75);

    // Input Text
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setString("");
    inputText.setCharacterSize(18);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Yellow);
    setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);

    std::string playerName;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (currentState == WELCOME_SCREEN) {
                if (event.type == sf::Event::TextEntered) {
                    if (std::isalpha(event.text.unicode) &&
                        playerName.length() < 10) {
                        char enteredChar = static_cast < char > (event.text.unicode);
                        playerName += enteredChar;
                        inputText.setString(playerName + "|");
                        setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);
                    }
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::BackSpace && !playerName.empty()) {
                        playerName.pop_back();
                        inputText.setString(playerName + "|");
                        setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);
                    } else if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                        playerName[0] = std::toupper(playerName[0]);
                        for (size_t i = 1; i < playerName.length(); ++i) {
                            playerName[i] = std::tolower(playerName[i]);
                        }
                        currentState = PLAYING;
                    }
                }
            };

            if (currentState == PLAYING) {
                int width, height;
                std::ifstream config("board_config.cfg");
                window.setSize(sf::Vector2u(initGame.width,initGame.height));
                config.close();
                initGame.update(window);
                // Update the game textures
                // handle events for the gameplay screen


                // handle events for the gameplay screen
            } else if (currentState == LEADERBOARD) {
                // handle events for the leaderboard screen
            }
        }

        window.clear(sf::Color::Blue);

        switch (currentState) {
            case WELCOME_SCREEN:
                // draw the welcome screen
                window.draw(welcomeText);
                window.draw(inputPromptText);
                window.draw(inputText);
                break;
            case PLAYING:
                window.clear(sf::Color::White);
                initGame.update(window);
                // draw the gameplay screen
                break;
            case LEADERBOARD:
                // draw the leaderboard screen
                break;
        }

        window.display();
    }

    return 0;
}