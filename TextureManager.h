#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"

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
#include <unordered_set>
#include "LeaderboardWindow.h"
using namespace std;
using namespace sf;
using namespace std::chrono;

class TextureManager {
    int flags_available, tiles_revealed{}, columns{}, rows{}, mines{}, tile_count{}, minesLeft, configMines{};
    std::map<std::string, sf::Texture> icon_textures;
    vector<bool> tile_id;
    Sprite debug_icon, smiley, leaderboard, pause, play, hundreds, tens, ones;
    Sprite timerMinutesTens, timerMinutesOnes, timerSecondsTens, timerSecondsOnes;
    bool game_lose{}, game_over{};
    bool playerWon;
public:
    sf::Time elapsedTime;
    bool isPaused = false;
    int initialMinesLeft = mines;
    vector<Tile> tiles;
    TextureManager();
    void initialize_mines();
    void MakeTiles();
    void setBombStates();
    void setIcons();
    void update(RenderWindow &window);
    void debug();
    void draw(RenderWindow &window);
    void refreshCounter();
    void refreshSmiley();
    void load_board_config();
    int width{}, height{};
    bool rightMouseButtonPressed;
    bool leftButtonClicked;
    Clock gameClock;
    int currentTime{};
    void updateTimer();
    void resetTimer();
    void pauseGame();
};

TextureManager::TextureManager()
{
    elapsedTime = sf::Time::Zero;
    leftButtonClicked = false;
    rightMouseButtonPressed = false;
    load_board_config();
    flags_available = configMines;
    minesLeft = configMines;
    initialMinesLeft = configMines;
    playerWon = false;

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
        icon_textures.emplace(name, texture);
        icon_textures[name].loadFromFile(file);
    }

    debug_icon.setTexture(icon_textures["debug"]);
    debug_icon.setPosition((columns * 32) - 304, 32 * (rows + 0.5f));
    smiley.setTexture(icon_textures["happy"]);
    smiley.setPosition(((columns / 2.0) * 32) - 32, 32 * (rows + 0.5f));
    pause.setTexture(icon_textures["pause"]);
    pause.setPosition((columns * 32) - 240, 32 * (rows + 0.5f));
    leaderboard.setTexture(icon_textures["leaderboard"]);
    leaderboard.setPosition((columns * 32) - 176, 32 * (rows + 0.5f));
    hundreds.setTexture(icon_textures["digits"]);
    hundreds.setTextureRect(sf::IntRect(0, 0, 21, 32));
    hundreds.setPosition(33, 32 * ((rows) + 0.5f) + 16);
    tens.setTexture(icon_textures["digits"]);
    tens.setTextureRect(sf::IntRect(21 * 5, 0, 21, 32));
    tens.setPosition(54, 32 * ((rows) + 0.5f) + 16);
    ones.setTexture(icon_textures["digits"]);
    ones.setTextureRect(sf::IntRect(0, 0, 21, 32));
    ones.setPosition(75, 32 * ((rows) + 0.5f) + 16);
    timerMinutesTens.setTexture(icon_textures["digits"]);
    timerMinutesTens.setPosition((columns * 32) - 97, 32 * (rows + 0.5f) + 16);
    timerMinutesOnes.setTexture(icon_textures["digits"]);
    timerMinutesOnes.setPosition((columns * 32) - 97 + 21, 32 * (rows + 0.5f) + 16);
    timerSecondsTens.setTexture(icon_textures["digits"]);
    timerSecondsTens.setPosition((columns * 32) - 54, 32 * (rows + 0.5f) + 16);
    timerSecondsOnes.setTexture(icon_textures["digits"]);
    timerSecondsOnes.setPosition((columns * 32) - 54 + 21, 32 * (rows + 0.5f) + 16);
    MakeTiles();
    initialize_mines();
    refreshCounter();

}

void TextureManager::load_board_config() {
    std::ifstream config_file("board_config.cfg");
    if (config_file.is_open()) {
        config_file >> columns >> rows >> mines;
        cout << "Columns: " << columns << ", Rows: " << rows << ", Mines: " << mines << endl;
        configMines = mines;


        width = columns * 32;
        height = (rows * 32) + 100;
        tile_count = columns * rows;

        config_file.close();
    } else {
        cout << "Unable to open board_config.cfg" << std::endl;
    }
}



void TextureManager::initialize_mines() {
    gameClock.restart();
    currentTime = 0;
    tile_id.clear();
    unordered_set<int> mine_positions;
    minesLeft = configMines;

    // Place mines in random positions
    while (mine_positions.size() < mines) {
        int random_position = rand() % tile_count;
        mine_positions.insert(random_position);
    }

    for (int i = 0; i < tile_count; ++i) {
        tile_id.push_back(mine_positions.count(i) > 0);
    }

    setBombStates();
    setIcons();
    flags_available = mines; // initialize flags_available with the number of mines
    refreshCounter();
    tiles_revealed = 0;

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
            Tile t = Tile(icon_textures, x, y);
            tiles.push_back(t);
            x += 32.f;
        }
        x = 0;
        y += 32.f;
    }
}


void TextureManager::setBombStates()
{
    for (int c = 0; c < tile_count; c++)
    {
        tiles[c].setBombState(tile_id[c]);
    }
}

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
        tiles[c].updateIcon(icon_textures);
    }
}



void TextureManager::update(RenderWindow &window)
{
    Vector2i position = Mouse::getPosition(window);

    // Assuming happyFaceSprite is an instance of sf::Sprite
    sf::IntRect rect = smiley.getTextureRect();
    int happyFaceWidth = rect.width;
    int happyFaceHeight = rect.height;

// Calculate the position of the happy face button



    sf::IntRect debugRect = debug_icon.getTextureRect();
    int debugWidth = debugRect.width;
    int debugHeight = debugRect.height;

    sf::IntRect pausePlayRect = pause.getTextureRect();
    int pausePlayWidth = pausePlayRect.width;
    int pausePlayHeight = pausePlayRect.height;

    sf::IntRect leaderboardRect = leaderboard.getTextureRect();
    int leaderboardWidth = leaderboardRect.width;
    int leaderboardHeight = leaderboardRect.height;

// Calculate the positions of the buttons
    float happyFaceX = ((static_cast<float>(columns) / 2.0) * 32) - 32;
    float happyFaceY = 32 * (static_cast<float>(rows) + 0.5f);

    float debugX = (static_cast<float>(columns) * 32) - 304;
    float debugY = 32 * (static_cast<float>(rows) + 0.5f);

    float pausePlayX = (static_cast<float>(columns) * 32) - 240;
    float pausePlayY = 32 * (static_cast<float>(rows) + 0.5f);

    float leaderboardX = (static_cast<float>(columns) * 32) - 176;
    float leaderboardY = 32 * (static_cast<float>(rows) + 0.5f);

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        if (!leftButtonClicked)
        {
            leftButtonClicked = true;

            if (position.x >= happyFaceX && position.x <= (happyFaceX + happyFaceWidth) &&
                position.y >= happyFaceY && position.y <= (happyFaceY + happyFaceHeight))
            {
                cout << "happy face button clicked" << endl;
                resetTimer();
                initialize_mines();
            }
            else if (position.x >= debugX && position.x <= (debugX + debugWidth) &&
                     position.y >= debugY && position.y <= (debugY + debugHeight))
            {
                debug();
                cout << "debug button clicked" << endl;
            }
            else if (position.y >= pausePlayY && position.x >= pausePlayX && position.x <= pausePlayX + pausePlayWidth && position.y <= pausePlayY + pausePlayHeight)
            {
                // Only allow pausing/resuming if the game is not over
                if (!game_over && !game_lose) {
                    pauseGame();
                    pause.setTexture(icon_textures[isPaused ? "play" : "pause"]);
                }
            }
            else if (position.x >= leaderboardX && position.x <= (leaderboardX + leaderboardWidth) &&
                     position.y >= leaderboardY && position.y <= (leaderboardY + leaderboardHeight))
            {
                cout << "leaderboard button clicked" << endl;
                LeaderboardWindow leaderboardWindow(false, "", "");
                leaderboardWindow.run();
            }

                // Only process tile clicks if the game is not paused
            else if (!isPaused && position.y >= 0 && position.y < height - 100 && !game_over)
            {
                cout << "grid clicked" << endl;
                int x = (position.x / 32);
                int y = (position.y / 32);

                int pos = y * columns + x;
                int count = 0;
                if (tiles[pos].reveal(count)) {
                    game_lose = true;
                    game_over = true;
                    int dummycount = 0;
                    for (int c = 0; c < tile_count; c++) {
                        if (tiles[c].isflagged && tiles[c].isbomb)
                            tiles[c].toggleFlag();
                        if (tiles[c].isbomb)
                            tiles[c].reveal(dummycount);
                    }
                    resetTimer(); // Reset the timer when the game is over or lost
                }
                else tiles_revealed += count;
                if (tiles_revealed + mines == tile_count) {
                    game_over = true;
                    playerWon = true;
                    game_lose = false; // Set playerWon to true to indicate the player has won
                    // Perform any additional actions required when the player wins, e.g., displaying a win message or stopping the timer
                }

            }
        }
    }
    else
    {
        leftButtonClicked = false;
    }


    if (Mouse::isButtonPressed(Mouse::Right)) {
        if (!rightMouseButtonPressed && !game_over && !isPaused) { // Only process right-clicks if the game is not paused
            rightMouseButtonPressed = true;

            if (position.y >= 0 && position.y < height - 100 && !game_over){
                int x = (position.x / 32);
                int y = (position.y / 32);

                int pos = (y * columns) + x;
                refreshCounter();
                if (!tiles[pos].isflagged && !tiles[pos].isrevealed) {
                    tiles[pos].toggleFlag();
                    flags_available--;
                    minesLeft--;

                } else if (!tiles[pos].isrevealed && tiles[pos].isflagged) { // Added the minesLeft != -1 check
                    tiles[pos].toggleFlag();
                    flags_available++;
                    minesLeft++;
                }

            }
        }
    } else {
        rightMouseButtonPressed = false;
    }

    if (tiles_revealed == (tile_count - mines) && !game_lose) {
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

void TextureManager::debug()
{
    cout << "Before setting minesLeft: " << minesLeft << endl;
    minesLeft = initialMinesLeft;
    cout << "After setting minesLeft: " << minesLeft << endl;

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
        {
            smiley.setTexture(icon_textures["lose"]);
        }
        else
        {
            smiley.setTexture(icon_textures["win"]);
            playerWon = true; // Update playerWon when the player wins
        }
    }
    else
    {
        smiley.setTexture(icon_textures["happy"]);
    }
}


void TextureManager::resetTimer() {
    elapsedTime = sf::Time::Zero;
    gameClock.restart();
    currentTime = 0;
}
void TextureManager::pauseGame() {
    if (!isPaused) {
        isPaused = true;
        elapsedTime += gameClock.getElapsedTime();
        // Iterate over each tile
        for (int i = 0; i < tiles.size(); i++) {
            if (!tiles[i].isrevealed) {
                tiles[i].revealTile();
            }
            tiles[i].icon.setColor(Color(255, 255, 255, 0));
        }
    } else {
        isPaused = false;
        gameClock.restart();
        // Revert change texture here
        for (int i = 0; i < tiles.size(); i++) {
            if (!tiles[i].isrevealed) {
                tiles[i].hideTile();
            }
            if (tiles[i].isrevealed && !tiles[i].isbomb) {
                tiles[i].icon.setColor(Color(255, 255, 255, 255));
            }
        }
    }
}







void TextureManager::updateTimer() {
    if (!isPaused && !(game_over || game_lose)) {
        // Get elapsed time in seconds
        currentTime = static_cast<int>((gameClock.getElapsedTime() + elapsedTime).asSeconds());
    }

    // ... the rest of the function remains unchanged


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


#pragma clang diagnostic pop