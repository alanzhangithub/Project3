#pragma once
#include <SFML/Graphics.hpp>

class Scene_Welcome {
public:
    Scene_Welcome(sf::RenderWindow& window, sf::Font& font);

    bool isReady() const;
    void update();
    void draw();

    const std::string& getPlayerName() const;

private:
    sf::RenderWindow& m_window;
    sf::Font& m_font;
    sf::Text m_welcomeText;
    sf::Text m_inputPromptText;
    sf::Text m_inputText;
    std::string m_playerName;
    bool m_ready;
};
