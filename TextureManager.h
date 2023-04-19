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
    int numbombs{};
    int tilesrevealed{};
    int columns{}, rows{}, mines{};
    int mine_count{}, tile_count{};


    map<string, Texture> icontextures;
    vector<bool> tileids;


    Sprite debug_icon;

    Sprite smiley;
    Sprite leaderboard;
    Sprite pause;
    Sprite play;

    Sprite hundreds;
    Sprite tens;
    Sprite ones;

    bool game_lose{};
    bool game_over{};


public:
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
};
TextureManager::TextureManager()
{
    load_board_config();
    flagsavailable = mine_count;

    const std::vector<std::tuple<std::string, std::string>> TEXTURE_FILES = {
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
        Texture texture;
        texture.loadFromFile(file);
        icontextures.emplace(name, texture);
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


    MakeTiles();
    RandomTileIds();
}

void TextureManager::load_board_config() {
    std::ifstream config_file("board_config.cfg");
    if (config_file.is_open()) {
        config_file >> columns >> rows >> mines;

        width = columns * 32;
        height = (rows * 32) + 100;
        mine_count = mines;
        tile_count = columns * rows;
        std::cout << "Width: " << width << ", Height: " << height << ", Mine Count: " << mine_count << ", Tile Count: " << tile_count << std::endl;

        config_file.close();
    } else {
        std::cerr << "Unable to open board_config.cfg" << std::endl;
    }
}



void TextureManager::RandomTileIds()
{
    tileids.clear();
    vector<int> bits;
    int randomnum;
    bool getfree = false;

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
    flagsavailable = mines;
    refreshCounter();
    numbombs = mines;
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



//checks for left and right clicks
void TextureManager::update(RenderWindow &window)
{

    if (Mouse::isButtonPressed(Mouse::Left))
    {

        Vector2i position = Mouse::getPosition(window);
        if (position.y >= 0 && position.y < 512 && !game_over)
        {
            if (position.x >= 0 && position.x <= 800)
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
        }
        else if (position.y >= 512 && position.y < 572)
        {
            if (position.x >= 368 && position.x < 432)
            {
                RandomTileIds();
            }
            if (position.x >= 496 && position.x < 560 && !game_over)
            {
                debug();
            }


        }
    }

    if (Mouse::isButtonPressed(Mouse::Right)&& !game_over)
    {
        Vector2i position = Mouse::getPosition(window);
        if (position.x >= 0 && position.x <= 800)
        {
            if (position.y >= 0 && position.y <= 512)
            {
                int x = (position.x / 32);
                int y = (position.y / 32);

                int pos = (y * columns) + x;
                if (!tiles[pos].isflagged && flagsavailable > 0 && !tiles[pos].isrevealed)
                {
                    tiles[pos].toggleFlag();
                    flagsavailable--;
                }
                else if (!tiles[pos].isrevealed && tiles[pos].isflagged)
                {
                    tiles[pos].toggleFlag();
                    flagsavailable++;
                }
                refreshCounter();
            }
        }
    }

    if (tilesrevealed == (tile_count - numbombs) && !game_lose)
    {
        game_over = true;
        for (int e = 0; e < tile_count; e++)
        {
            if (tiles[e].isbomb && !tiles[e].isflagged)
            {
                tiles[e].toggleFlag();
            }
        }
    }

    refreshSmiley();

    draw(window);

    }

//calls toggleDebug on each tile
void TextureManager::debug()
{
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
}

void TextureManager::refreshCounter()
{
    int copy = flagsavailable;
    int onesplace = copy % 10;
    copy /= 10;
    int tensplace = copy % 10;

    tens.setTextureRect(Rect<int>(21 * tensplace, 0, 21, 32));
    ones.setTextureRect(Rect<int>(21 * onesplace, 0, 21, 32));
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

