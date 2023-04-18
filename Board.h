#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Tile.h"
#include "TextureManager.h"

class Board {
public:
    Board(unsigned int width, unsigned int height, TextureManager& textureManager);

    void draw(sf::RenderWindow& window);
    void update(float deltaTime);
    void restart();

private:
    std::vector<std::vector<Tile>> tiles;
    unsigned int width;
    unsigned int height;
    TextureManager& textureManager;

    void initializeTiles();
};

#endif // BOARD_H
