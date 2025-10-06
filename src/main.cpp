// ----------------------------------------------------------------
// Main entry point following the asteroids game architecture
// ----------------------------------------------------------------

#include "Game/Game.hpp"

int main(int argc, char** argv)
{
    Game game;
    bool success = game.Initialize();
    if (success)
    {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}
