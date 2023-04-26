#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "TextureManager.h"
using namespace sf;
using namespace std;

class Tile {
    Sprite background;
    Sprite foreground;
    Sprite flagicon;
    Sprite tile;
    bool isdebug;
    void setIconTexture(map<string, Texture> &textures);

public:
    Sprite icon;
    Sprite revealed, hidden;
    vector<Tile*> neighbors;
    int numbombs;
    bool isbomb;
    bool isflagged;
    bool isrevealed;

    Tile(map<string, Texture> &textures, float x_coordinate, float y_coordinate);
    void setBombState(bool bombstate);
    void updateIcon(map<string, Texture> &textures);

    bool toggleFlag();
    void toggleDebug();
    bool reveal(int &counter);
    void draw(RenderWindow &w);
    void setTexture(const sf::Texture& texture);

    void hideTile();

    void revealTile();
};

Tile::Tile(map<string, Texture> &textures, float x_coordinate, float y_coordinate) {
    background.setPosition(x_coordinate, y_coordinate);
    foreground.setPosition(x_coordinate, y_coordinate);
    flagicon.setPosition(x_coordinate, y_coordinate);
    icon.setPosition(x_coordinate, y_coordinate);

    background.setTexture(textures["revealed"]);
    foreground.setTexture(textures["hidden"]);
    flagicon.setTexture(textures["flag"]);
    revealed.setTexture(textures["revealed"]);
    hidden.setTexture(textures["hidden"]);



    flagicon.setColor(Color(255, 255, 255, 0));
    isbomb = false;
    isrevealed = false;
    isflagged = false;
    isdebug = false;

}

void Tile::setBombState(bool bombstate) {
    isbomb = bombstate;
}

void Tile::updateIcon(map<string, Texture> &textures) {
    numbombs = 0;
    for (Tile* neighbor : neighbors) {
        if (neighbor != nullptr && neighbor->isbomb) {
            numbombs++;
        }
    }
    setIconTexture(textures);
    icon.setColor(Color(255, 255, 255, 0));
    flagicon.setColor(Color(255, 255, 255, 0));
    foreground.setColor(Color(255, 255, 255, 255));
    isrevealed = false;
    isflagged = false;
    isdebug = false;
}

void Tile::setIconTexture(map<string, Texture> &textures) {
    if (isbomb) {
        icon.setTexture(textures["bomb"]);
    } else {
        static const string numberTextures[] = {"revealed", "one", "two", "three", "four", "five", "six", "seven", "eight"};
        icon.setTexture(textures[numberTextures[numbombs]]);
    }
}

bool Tile::toggleFlag() {
    if (!isrevealed) {
        isflagged = !isflagged;
        flagicon.setColor(isflagged ? Color(255, 255, 255, 255) : Color(255, 255, 255, 0));
    }
    return isflagged;
}

void Tile::toggleDebug() {
    if (isbomb && !isrevealed) {
        isdebug = !isdebug;
        icon.setColor(isdebug ? Color(255, 255, 255, 255) : Color(255, 255, 255, 0));
    }
}

bool Tile::reveal(int &counter) {
    if (!isrevealed && !isflagged) {
        isrevealed = true;
        counter++;
        foreground.setColor(Color(255, 255, 255, 0));
        icon.setColor(Color(255, 255, 255, 255));
        if (numbombs == 0 && !isbomb) {
            for (Tile* neighbor : neighbors) {
                if (neighbor != nullptr && !(neighbor->isbomb)) {
                    neighbor->reveal(counter);
                }
            }
        }
    }
    return isbomb;
}

void Tile::draw(RenderWindow &w) {
    w.draw(background);
    w.draw(foreground);
    w.draw(icon);
    w.draw(flagicon);
}


void Tile::hideTile() {
    foreground.setTexture(*hidden.getTexture());
}

void Tile::revealTile() {
    foreground.setTexture(*revealed.getTexture());
}






