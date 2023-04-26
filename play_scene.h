#include <SFML/Graphics.hpp>

#include <string>

#include <fstream>
#include <cctype>
#include "TextureManager.h"

float play_scene(const std::string &playerName, bool &playerWon) {
    TextureManager game_init;
    game_init.load_board_config();
    sf::RenderWindow window(sf::VideoMode(game_init.width, game_init.height), "Minesweeper", sf::Style::Close);
    sf::Event event{};

    bool loop_exit = false;
    sf::Clock clock;
    while (!loop_exit) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                loop_exit = true;
            }
        }
        sf::Time elapsed = clock.restart();
        game_init.updateTimer();
        window.clear(sf::Color::White);
        game_init.update(window);
        window.display();
        playerWon = true;
    }
    return clock.getElapsedTime().asSeconds(); // Add this line to return the elapsed time

}

std::string format_time(float time) {
    int minutes = static_cast<int>(time) / 60;
    int seconds = static_cast<int>(time) % 60;
    int milliseconds = static_cast<int>((time - static_cast<int>(time)) * 100);

    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(2) << minutes << ":"
           << std::setfill('0') << std::setw(2) << seconds << "."
           << std::setfill('0') << std::setw(2) << milliseconds;

    return stream.str();
}
