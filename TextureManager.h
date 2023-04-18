#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class TextureManager
{
public:
    // Get the instance of the singleton class
    static TextureManager& getInstance();

    // Load a texture from a file and store it with a given identifier
    bool loadTexture(const std::string& identifier, const std::string& filename);

    // Get a reference to the texture with the given identifier
    sf::Texture& getTexture(const std::string& identifier);

    // Remove a texture with the given identifier
    void removeTexture(const std::string& identifier);

// Constructor
TextureManager() {}

private:

    // Copy constructor (deleted to prevent multiple instances)
    TextureManager(const TextureManager&) = delete;

    // Assignment operator (deleted to prevent multiple instances)
    TextureManager& operator=(const TextureManager&) = delete;

    // Map to store textures with their identifier
    std::map<std::string, sf::Texture> textures;
};

#endif // TEXTURE_MANAGER_H
