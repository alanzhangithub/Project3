#include <SFML/Graphics.hpp>
#include <string>
#include "Scene_Welcome.h"

using namespace sf;
using namespace std;

void setText(sf::Text & text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void Scene_Welcome::Init() {
    font.loadFromFile("font.ttf");

    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);
    setText(welcomeText, window->getSize().x / 2.0f, window->getSize().y / 2.0f - 150);

    inputPromptText.setFont(font);
    inputPromptText.setString("Enter your name:");
    inputPromptText.setCharacterSize(20);
    inputPromptText.setStyle(sf::Text::Bold);
    inputPromptText.setFillColor(sf::Color::White);
    setText(inputPromptText, window->getSize().x / 2.0f, window->getSize().y / 2.0f - 75);

    inputText.setFont(font);
    inputText.setString("");
    inputText.setCharacterSize(18);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Yellow);
    setText(inputText, window->getSize().x / 2.0f, window->getSize().y / 2.0f - 45);

    playerName = "";
}

void Scene_Welcome::Update() {
    if (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (std::isalpha(event.text.unicode) &&
                    playerName.length() < 10) {
                    char enteredChar = static_cast < char > (event.text.unicode);
                    playerName += enteredChar;
                    inputText.setString(playerName + "|");
                    setText(inputText, window->getSize().x / 2.0f, window->getSize().y / 2.0f - 45);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace && !playerName.empty()) {
                    playerName.pop_back();
                    inputText.setString(playerName + "|");
                    setText(inputText, window->getSize().x / 2.0f, window->getSize().y / 2.0f - 45);
                } else if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    playerName[0] = std::toupper(playerName[0]);
                    for (size_t i = 1; i < playerName.length(); ++i) {
                        playerName[i] = std::tolower(playerName[i]);
                    }
                    nextScene = "play";
                }
            }
        }

        window->clear(sf::Color::Blue);

        window->draw(welcomeText);
        window->draw(inputPromptText);
        window->draw(inputText);

        window->display();
    }
}

void Scene_Welcome::Destroy() {

}
