#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <cctype>


void setText(sf::Text & text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}


void welcome_scene(std::string& playerName) {
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
    setText(welcomeText, static_cast<float>(window.getSize().x) / 2.0f, static_cast<float>(window.getSize().y) / 2.0f - 150);

    // Input Prompt Text
    sf::Text inputPromptText;
    inputPromptText.setFont(font);
    inputPromptText.setString("Enter your name:");
    inputPromptText.setCharacterSize(20);
    inputPromptText.setStyle(sf::Text::Bold);
    inputPromptText.setFillColor(sf::Color::White);
    setText(inputPromptText,  static_cast<float>(window.getSize().x) / 2.0f,  static_cast<float>(window.getSize().y) / 2.0f - 75);

    // Input Text
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setString("");
    inputText.setCharacterSize(18);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Yellow);
    setText(inputText,  static_cast<float>(window.getSize().x) / 2.0f,  static_cast<float>(window.getSize().y) / 2.0f - 45);

    sf::Event event{};
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
                if (std::isalpha(static_cast<int>(event.text.unicode)) && playerName.length() < 10) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    playerName += enteredChar;
                    inputText.setString(playerName + "|");
                    setText(inputText, static_cast<float>(window.getSize().x) / 2.0f, static_cast<float>(window.getSize().y) / 2.0f - 45.0f);
                }
            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace && !playerName.empty()) {
                    playerName.pop_back();
                    inputText.setString(playerName + "|");
                    setText(inputText,  static_cast<float>(window.getSize().x) / 2.0f,  static_cast<float>(window.getSize().y) / 2.0f - 45);
                }
                else if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    playerName[0] = static_cast<char>(std::toupper(playerName[0]));
                    for (size_t i = 1; i < playerName.length(); ++i) {
                        playerName[i] = static_cast<char>(std::tolower(playerName[i]));
                    }
                    loop_exit = true;
                    window.close();
                }
            }
        }
    }
}



