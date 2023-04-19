//
// Created by Alan on 4/11/2023.
//

#pragma once
#include "Tile.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>

#include <iomanip>
using namespace std;
using namespace sf;
using namespace std::chrono;


class TextureManager {
    int flagsavailable;
    int tilesrevealed;
    int columns, rows, mines;
    int tile_count;
    int minesLeft;



    std::map<std::string, sf::Texture> icontextures;
    vector<bool> tileids;


    Sprite debug_icon;

    Sprite smiley;
    Sprite leaderboard;
    Sprite pause;
    Sprite play;

    Sprite hundreds;
    Sprite tens;
    Sprite ones;
    sf::Sprite timerMinutesTens;
    sf::Sprite timerMinutesOnes;
    sf::Sprite timerSecondsTens;
    sf::Sprite timerSecondsOnes;
    bool game_lose{};
    bool game_over{};


public:
    int initialMinesLeft = mines;
    vector<Tile> tiles;
    TextureManager();
    void RandomTileIds();
    void MakeTiles();
    void setBombStates();
    void setIcons();
    void update(RenderWindow &window);
    void debug();
    void draw(RenderWindow &window);
    void refreshCounter();
    void refreshSmiley();
    void load_board_config();
    int width{};
    int height{};
    bool rightMouseButtonPressed;
    bool leftButtonClicked;
    sf::Clock gameClock;
    int currentTime;

    void updateTimer();
};


TextureManager::TextureManager()
{
    leftButtonClicked = false;
    rightMouseButtonPressed = false;
    load_board_config();
    flagsavailable = mines;
    minesLeft = mines;



    load_board_config();
    std::map<std::string, std::string> TEXTURE_FILES = {
            {"debug", "images/debug.png"},
            {"leaderboard", "images/leaderboard.png"},
            {"play", "images/play.png"},
            {"pause", "images/pause.png"},
            {"digits", "images/digits.png"},
            {"happy", "images/face_happy.png"},
            {"lose", "images/face_lose.png"},
            {"win", "images/face_win.png"},
            {"flag", "images/flag.png"},
            {"bomb", "images/mine.png"},
            {"one", "images/number_1.png"},
            {"two", "images/number_2.png"},
            {"three", "images/number_3.png"},
            {"four", "images/number_4.png"},
            {"five", "images/number_5.png"},
            {"six", "images/number_6.png"},
            {"seven", "images/number_7.png"},
            {"eight", "images/number_8.png"},
            {"hidden", "images/tile_hidden.png"},
            {"revealed", "images/tile_revealed.png"}
    };

    for (const auto& [name, file] : TEXTURE_FILES) {
        sf::Texture texture;
        icontextures.emplace(name, texture);
        icontextures[name].loadFromFile(file);
    }

    debug_icon.setTexture(icontextures["debug"]);
    debug_icon.setPosition((columns * 32) - 304, 32 * (rows + 0.5f));
    smiley.setTexture(icontextures["happy"]);
    smiley.setPosition(((columns / 2.0) * 32) - 32, 32 * (rows + 0.5f));
    pause.setTexture(icontextures["pause"]);
    pause.setPosition((columns * 32) - 240, 32 * (rows + 0.5f));
    leaderboard.setTexture(icontextures["leaderboard"]);
    leaderboard.setPosition((columns * 32) - 176, 32 * (rows + 0.5f));

    hundreds.setTexture(icontextures["digits"]);
    hundreds.setTextureRect(sf::IntRect(0, 0, 21, 32));
    hundreds.setPosition(33, 32 * ((rows) + 0.5f) + 16);
    tens.setTexture(icontextures["digits"]);
    tens.setTextureRect(sf::IntRect(21 * 5, 0, 21, 32));
    tens.setPosition(54, 32 * ((rows) + 0.5f) + 16);
    ones.setTexture(icontextures["digits"]);
    ones.setTextureRect(sf::IntRect(0, 0, 21, 32));
    ones.setPosition(75, 32 * ((rows) + 0.5f) + 16);

    timerMinutesTens.setTexture(icontextures["digits"]);
    timerMinutesTens.setPosition((columns * 32) - 97, 32 * (rows + 0.5f) + 16);


    timerMinutesOnes.setTexture(icontextures["digits"]);
    timerMinutesOnes.setPosition((columns * 32) - 97 + 21, 32 * (rows + 0.5f) + 16);

    timerSecondsTens.setTexture(icontextures["digits"]);
    timerSecondsTens.setPosition((columns * 32) - 54, 32 * (rows + 0.5f) + 16);

    timerSecondsOnes.setTexture(icontextures["digits"]);
    timerSecondsOnes.setPosition((columns * 32) - 54 + 21, 32 * (rows + 0.5f) + 16);



    MakeTiles();
    RandomTileIds();
    refreshCounter();

}

void TextureManager::load_board_config() {
    std::ifstream config_file("board_config.cfg");
    if (config_file.is_open()) {
        config_file >> columns >> rows >> mines;

        width = columns * 32;
        height = (rows * 32) + 100;
        tile_count = columns * rows;

        config_file.close();
    } else {
        cout << "Unable to open board_config.cfg" << std::endl;
    }
}



void TextureManager::RandomTileIds()
{
    tileids.clear();
    vector<int> bits;
    int randomnum;
    bool getfree = false;
    minesLeft = mines;

    for (int c = 0; c < mines; c++)
    {
        while (!getfree)
        {
            randomnum = rand() % tile_count;
            if (find(bits.begin(), bits.end(), randomnum) == bits.end())
            {
                bits.push_back(randomnum);
                getfree = true;
            }
        }
        getfree = false;
    }

    auto iter = bits.begin();
    for (int d = 0; d < tile_count; d++)
    {
        iter = find(bits.begin(), bits.end(), d);
        if (iter == bits.end())
        {
            tileids.push_back(false);
        }
        else
        {
            tileids.push_back(true);
        }
    }

    setBombStates();
    setIcons();
    flagsavailable = mines; // initialize flagsavailable with the number of mines
    refreshCounter();
    tilesrevealed = 0;

    game_lose = false;
    game_over = false;
}







//instantiates the array of tiles, which will remain static over all games played during this instance
void TextureManager::MakeTiles()
{
    float x = 0; float y = 0;
    for (int c = 0; c < rows; c++)
    {
        for (int d = 0; d < columns; d++)
        {
            Tile t = Tile(icontextures, x, y);
            tiles.push_back(t);
            x += 32.f;
        }
        x = 0;
        y += 32.f;
    }
}

//calls setbombstate on each tile in tiles
//for use ONLY when the game is reset, otherwise you'll just make a big mess
void TextureManager::setBombStates()
{
    for (int c = 0; c < tile_count; c++)
    {
        tiles[c].setBombState(tileids[c]);
    }
}

//assigns icons to each tile based on isbomb and nearby bombs
void TextureManager::setIcons()
{
    for (int c = 0; c < tile_count; c++)
    {
        tiles[c].neighbors.clear();
        for (int d = 0; d < 8; d++)
        {
            tiles[c].neighbors.push_back(nullptr);
        }
        if (c == 0) //tile is top left corner
        {
            tiles[c].neighbors[4] = &(tiles[c + 1]);
            tiles[c].neighbors[6] = &(tiles[c + columns]);
            tiles[c].neighbors[7] = &(tiles[c + columns + 1]);
        }
        else if (c == columns - 1) // tile is top right corner
        {
            tiles[c].neighbors[3] = &(tiles[c - 1]);
            tiles[c].neighbors[5] = &(tiles[c + columns - 1]);
            tiles[c].neighbors[6] = &(tiles[c + columns]);
        }
        else if (c == tile_count - columns) // tile is bottom left corner
        {
            tiles[c].neighbors[4] = &(tiles[c + 1]);
            tiles[c].neighbors[1] = &(tiles[c - columns]);
            tiles[c].neighbors[2] = &(tiles[c - columns + 1]);
        }
        else if (c == tile_count - 1) // tile is bottom right corner
        {
            tiles[c].neighbors[3] = &(tiles[c - 1]);
            tiles[c].neighbors[1] = &(tiles[c - columns]);
            tiles[c].neighbors[0] = &(tiles[c - columns - 1]);
        }
        else if (c < columns) // tile is on the top
        {
            tiles[c].neighbors[3] = &(tiles[c - 1]);
            tiles[c].neighbors[4] = &(tiles[c + 1]);
            tiles[c].neighbors[5] = &(tiles[c + columns - 1]);
            tiles[c].neighbors[6] = &(tiles[c + columns]);
            tiles[c].neighbors[7] = &(tiles[c + columns + 1]);
        }
        else if (c >= tile_count - columns) // tile is on the bottom
        {
            tiles[c].neighbors[3] = &(tiles[c - 1]);
            tiles[c].neighbors[4] = &(tiles[c + 1]);
            tiles[c].neighbors[2] = &(tiles[c - columns + 1]);
            tiles[c].neighbors[1] = &(tiles[c - columns]);
            tiles[c].neighbors[0] = &(tiles[c - columns - 1]);
        }
        else if ((c % columns) == 0) // tile is on the left edge
        {
            tiles[c].neighbors[4] = &(tiles[c + 1]);
            tiles[c].neighbors[6] = &(tiles[c + columns]);
            tiles[c].neighbors[7] = &(tiles[c + columns + 1]);
            tiles[c].neighbors[1] = &(tiles[c - columns]);
            tiles[c].neighbors[2] = &(tiles[c - columns + 1]);
        }
        else if (((c + 1) % columns) == 0) // tile is on the right edge
        {
            tiles[c].neighbors[3] = &(tiles[c - 1]);
            tiles[c].neighbors[0] = &(tiles[c - columns - 1]);
            tiles[c].neighbors[1] = &(tiles[c - columns]);
            tiles[c].neighbors[5] = &(tiles[c + columns - 1]);
            tiles[c].neighbors[6] = &(tiles[c + columns]);
        }
        else // tile is in the middle
        {
            tiles[c].neighbors[3] = &(tiles[c - 1]);
            tiles[c].neighbors[4] = &(tiles[c + 1]);
            tiles[c].neighbors[5] = &(tiles[c + columns - 1]);
            tiles[c].neighbors[6] = &(tiles[c + columns]);
            tiles[c].neighbors[7] = &(tiles[c + columns + 1]);
            tiles[c].neighbors[2] = &(tiles[c - columns + 1]);
            tiles[c].neighbors[1] = &(tiles[c - columns]);
            tiles[c].neighbors[0] = &(tiles[c - columns - 1]);
        }
        tiles[c].updateIcon(icontextures);
    }
}



void TextureManager::update(RenderWindow &window)
{
    Vector2i position = Mouse::getPosition(window);

    int debugButtonX = (columns * 32) - 304;
    int debugButtonY = 32 * (rows + 0.5f);
    int happyFaceX = ((columns / 2.0) * 32) - 32;
    int happyFaceY = 32 * (rows + 0.5f);
    int pausePlayButtonX = (columns * 32) - 240;
    int pausePlayButtonY = 32 * (rows + 0.5f);
    int leaderboardButtonX = (columns * 32) - 176;
    int leaderboardButtonY = 32 * (rows + 0.5f);

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        if (!leftButtonClicked)
        {
            leftButtonClicked = true;

            if (position.y >= 0 && position.y < 512 && !game_over)
            {
                int x = (position.x / 32);
                int y = (position.y / 32);

                int pos = (y * columns) + x;
                int count = 0;
                if (tiles[pos].reveal(count))
                {
                    game_lose = true;
                    game_over = true;
                    int dummycount = 0;
                    for (int c = 0; c < tile_count; c++)
                    {
                        if (tiles[c].isflagged && tiles[c].isbomb)
                            tiles[c].toggleFlag();
                        if (tiles[c].isbomb)
                            tiles[c].reveal(dummycount);
                    }
                }
                else tilesrevealed += count;
            }
            else if (position.y >= 512 && position.y < 576)
            {
                // Happy face button
                if (position.x >= happyFaceX && position.x < happyFaceX + 64 &&
                    position.y >= happyFaceY && position.y < happyFaceY + 64)
                {
                    RandomTileIds();
                }
                    // Debug button
                else if (position.x >= debugButtonX && position.x < debugButtonX + 64 &&
                         position.y >= debugButtonY && position.y < debugButtonY + 64 &&
                         !game_over)
                {
                    debug();
                }
                    // Pause/Play button
                else if (position.x >= pausePlayButtonX && position.x < pausePlayButtonX + 64 &&
                         position.y >= pausePlayButtonY && position.y < pausePlayButtonY + 64)
                {
                    // Implement play/pause functionality
                }
                    // Leaderboard button
                else if (position.x >= leaderboardButtonX && position.x < leaderboardButtonX + 64 &&
                         position.y >= leaderboardButtonY && position.y < leaderboardButtonY + 64)
                {
                    // Implement leaderboard functionality
                }
            }
        }
    }
    else
    {
        leftButtonClicked = false;
    }


    if (Mouse::isButtonPressed(Mouse::Right)) {
        if (!rightMouseButtonPressed && !game_over) {
            rightMouseButtonPressed = true;

            Vector2i position = Mouse::getPosition(window);
            if (position.x >= 0 && position.x <= 800) {
                if (position.y >= 0 && position.y <= 512) {
                    int x = (position.x / 32);
                    int y = (position.y / 32);

                    int pos = (y * columns) + x;
                    refreshCounter();
                    if (!tiles[pos].isflagged && !tiles[pos].isrevealed) {
                        tiles[pos].toggleFlag();
                        flagsavailable--;
                        minesLeft--;
                        cout << "minesLeft after placing flag: " << minesLeft << endl;

                    } else if (!tiles[pos].isrevealed && tiles[pos].isflagged) { // Added the minesLeft != -1 check
                        tiles[pos].toggleFlag();
                        flagsavailable++;
                        minesLeft++;
                        cout << "minesLeft after removing flag: " << minesLeft << endl;
                    }
                }
            }
        }
    } else {
        rightMouseButtonPressed = false;
    }

    if (tilesrevealed == (tile_count - mines) && !game_lose) {
        game_over = true;
        for (int e = 0; e < tile_count; e++) {
            if (tiles[e].isbomb && !tiles[e].isflagged) {
                tiles[e].toggleFlag();
            }
        }
    }

    refreshCounter();
    refreshSmiley();
    draw(window);
}

//calls toggleDebug on each tile
void TextureManager::debug()
{
    minesLeft = initialMinesLeft;
    for (int c = 0; c < tile_count; c++)
    {
        tiles[c].toggleDebug();
    }
}

//calls draw on each tile in tiles
void TextureManager::draw(RenderWindow &window)
{
    for (int c = 0; c < tile_count; c++)
        tiles[c].draw(window);

    window.draw(smiley);
    window.draw(debug_icon);
    window.draw(pause);

    window.draw(leaderboard);

    window.draw(hundreds);
    window.draw(tens);
    window.draw(ones);
    window.draw(timerMinutesTens);
    window.draw(timerMinutesOnes);
    window.draw(timerSecondsTens);
    window.draw(timerSecondsOnes);

}

void TextureManager::refreshCounter() {
    int displayMinesLeft = abs(minesLeft); // Use a temporary variable to store the absolute value of minesLeft

    int hundredsDigit = displayMinesLeft / 100;
    int tensDigit = (displayMinesLeft / 10) % 10;
    int onesDigit = displayMinesLeft % 10;

    if (minesLeft < 0) {
        // Display the flag counter as negative
        // Update the counter sprites with a minus sign
        hundreds.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
    } else {
        // Display the flag counter as positive
        hundreds.setTextureRect(sf::IntRect(21 * hundredsDigit, 0, 21, 32));
    }

    // Update the tens and ones digits for both positive and negative numbers
    tens.setTextureRect(sf::IntRect(21 * tensDigit, 0, 21, 32));
    ones.setTextureRect(sf::IntRect(21 * onesDigit, 0, 21, 32));
}






void TextureManager::refreshSmiley()
{
    if (game_over)
    {
        if (game_lose)
            smiley.setTexture(icontextures["lose"]);
        else
            smiley.setTexture(icontextures["win"]);
    }
    else smiley.setTexture(icontextures["happy"]);
}

void TextureManager::updateTimer() {
    // Get elapsed time in seconds
    currentTime = static_cast<int>(gameClock.getElapsedTime().asSeconds());

    // Calculate the digits to display
    int minutes = currentTime / 60;
    int seconds = currentTime % 60;

    // Calculate the digits for the timer
    int minutesTensDigit = minutes / 10;
    int minutesOnesDigit = minutes % 10;
    int secondsTensDigit = seconds / 10;
    int secondsOnesDigit = seconds % 10;

    // Update the timer sprites with the appropriate digits
    timerMinutesTens.setTextureRect(sf::IntRect(21 * minutesTensDigit, 0, 21, 32));
    timerMinutesOnes.setTextureRect(sf::IntRect(21 * minutesOnesDigit, 0, 21, 32));
    timerSecondsTens.setTextureRect(sf::IntRect(21 * secondsTensDigit, 0, 21, 32));
    timerSecondsOnes.setTextureRect(sf::IntRect(21 * secondsOnesDigit, 0, 21, 32));
}
