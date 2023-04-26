#ifndef LEADERBOARDWINDOW_H
#define LEADERBOARDWINDOW_H

#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

class LeaderboardWindow {
public:
    LeaderboardWindow(bool playerWon, const std::string& playerName, const std::string& playerTime) {
        sf::Context context;
        window.create(sf::VideoMode(800 / 2, (600 / 2) + 50), "Leaderboard", sf::Style::Close);
        if (!font.loadFromFile("font.ttf")) {
            std::cout << "Error loading font file" << std::endl;
            // You can choose to exit the application or handle the error in another way
        }
        else {
            cout << "loaded successfully";
        }

        // Leaderboard Title
        leaderboardTitle.setFont(font);
        leaderboardTitle.setString("LEADERBOARD");
        leaderboardTitle.setCharacterSize(20);
        leaderboardTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
        leaderboardTitle.setFillColor(sf::Color::White);
        setText(leaderboardTitle, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 120);

        // Reading and displaying leaderboard content
        std::ifstream leaderboardFile("leaderboard.txt");
        std::vector<std::pair<std::string, std::string>> leaderboardData;
        std::string line;
        while (std::getline(leaderboardFile, line)) {
            std::istringstream iss(line);
            std::string time, name;
            std::getline(iss, time, ',');
            std::getline(iss, name);
            leaderboardData.emplace_back(time, name);
        }
        leaderboardFile.close();

        if (playerWon) {
            std::cout << "Player won, updating leaderboard..." << std::endl;
            leaderboardData.emplace_back(playerTime, playerName);
            std::sort(leaderboardData.begin(), leaderboardData.end());
            if (leaderboardData.size() > 5) {
                leaderboardData.resize(5);
            }

            std::ofstream updatedLeaderboardFile("leaderboard.txt");
            for (const auto& entry : leaderboardData) {
                updatedLeaderboardFile << entry.first << ',' << entry.second << '\n';
            }
            updatedLeaderboardFile.close();
        }

        std::string leaderboardContent;
        for (size_t i = 0; i < leaderboardData.size(); ++i) {
            leaderboardContent += std::to_string(i + 1) + ".\t" + leaderboardData[i].first + "\t" + leaderboardData[i].second;
            if (playerWon && leaderboardData[i].second == playerName && leaderboardData[i].first == playerTime) {
                leaderboardContent += " *";
            }
            leaderboardContent += "\n\n";
        }

        leaderboardText.setFont(font);
        leaderboardText.setString(leaderboardContent);
        leaderboardText.setCharacterSize(18);
        leaderboardText.setStyle(sf::Text::Bold);
        leaderboardText.setFillColor(sf::Color::White);
        setText(leaderboardText, window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20);
    }

    ~LeaderboardWindow() {
    }

    void run() {
        sf::Event event{};
        bool loop_exit = false;
        while (!loop_exit) {
            // Process events while the leaderboard window is open
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    loop_exit = true;
                    window.close();
                }
            }

            window.clear(sf::Color::Blue);
            window.draw(leaderboardTitle);
            window.draw(leaderboardText);
            window.display();
        }
    }

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text leaderboardTitle;
    sf::Text leaderboardText;
};

#endif // LEADERBOARDWINDOW_H

