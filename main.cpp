#include <SFML/Graphics.hpp>

#include <string>

#include <fstream>

#include <cctype>

#include "TextureManager.h"

using namespace sf;
using namespace std;
int currentState = 1;

void setText(sf::Text & text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void welcome_scene() {
    sf::Context context;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper", sf::Style::Close);
    sf::Font font;
    font.loadFromFile("font.ttf");
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
    sf::Event event;
    bool loop_exit = false;
    while (!loop_exit) {
        window.clear(sf::Color::Blue);
        window.draw(welcomeText);
        window.draw(inputPromptText);
        window.draw(inputText);
        window.display();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (std::isalpha(event.text.unicode) &&
                    playerName.length() < 10) {
                    char enteredChar = static_cast < char > (event.text.unicode);
                    playerName += enteredChar;
                    inputText.setString(playerName + "|");
                    setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace && !playerName.empty()) {
                    playerName.pop_back();
                    inputText.setString(playerName + "|");
                    setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);
                }
                else if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    playerName[0] = std::toupper(playerName[0]);
                    for (size_t i = 1; i < playerName.length(); ++i) {
                        playerName[i] = std::tolower(playerName[i]);
                    }
                    //currentState = 2;
                    loop_exit = true;
                    window.close();
                }
            }
        }
    }


};


void play_scene() {
    TextureManager game_init;
    game_init.load_board_config();
    sf::RenderWindow window(sf::VideoMode(game_init.width, game_init.height), "Minesweeper", sf::Style::Close);
    sf::Event event;
    bool loop_exit = false;
    sf::Clock clock;
    while (!loop_exit) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                loop_exit = true;
            }
        }
        // Calculate the elapsed time since the last loop iteration
        sf::Time elapsed = clock.restart();
        // Update the timer
        game_init.updateTimer();
        window.clear(sf::Color::White);
        game_init.update(window);
        window.display();
    }
}







int main() {
    switch (currentState) {
        case 1:
            welcome_scene();
            play_scene();
            break;
        case 2:
            play_scene();
            break;
        default:
            break;
    }

    return 0;
}
