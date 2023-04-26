#include "welcome_scene.h"
#include "play_scene.h"
#include "LeaderboardWindow.h"

int main() {
    std::string playerName;
    bool playerWon = false;
    welcome_scene(playerName);

    float gameTime = play_scene(playerName, playerWon);

    if (playerWon) {
        LeaderboardWindow(playerWon, playerName, format_time(gameTime));
    }

    return 0;
}


