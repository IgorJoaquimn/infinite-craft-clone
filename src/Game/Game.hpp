#ifndef GAME_HPP
#define GAME_HPP

#include "Actor/Actor.hpp"
#include "Core/Window/Window.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/InputManager/InputManager.hpp"
#include "Core/ResourceManager/ResourceManager.hpp"
#include <vector>
#include <memory>
#include <chrono>

/**
 * Game class following the Update Method pattern from Game Programming Patterns.
 * Now properly decoupled using separate classes for different concerns.
 */
class Game {
public:
    Game();
    ~Game();
    
    /**
     * Main entry point - runs the complete game loop
     */
    int run();

    /**
     * Add an actor to the game world.
     */
    void addActor(std::unique_ptr<Actor> actor);

    /**
     * Remove an actor from the game world.
     */
    void removeActor(Actor* actor);

    /**
     * Get all actors in the game world.
     */
    const std::vector<std::unique_ptr<Actor>>& getActors() const;

    /**
     * Clear all actors from the game world.
     */
    void clearActors();

private:
    // === LIFECYCLE FUNCTIONS ===
    /**
     * Initialize all subsystems
     */
    bool load();

    /**
     * Load game content (shaders, fonts, initial objects)
     */
    bool loadContent();

    /**
     * Main game loop
     */
    void gameLoop();

    /**
     * Update game logic - implements the Update Method pattern
     */
    void update(float deltaTime);

    /**
     * Render all game objects
     */
    void render();

    /**
     * Cleanup all resources
     */
    void unload();

    // === HELPER FUNCTIONS ===
    /**
     * Setup input callbacks
     */
    void setupInput();

    /**
     * Create initial game objects
     */
    bool createGameObjects();

    /**
     * Update all actors
     */
    void updateActors(float deltaTime);

    /**
     * Calculate delta time
     */
    float calculateDeltaTime();

    // === EVENT HANDLERS ===
    void onQuit();
    void onKeyEvent(SDL_Keycode key, bool pressed);
    void onMouseButton(int button, int x, int y, bool pressed);

    // === MEMBER VARIABLES ===
    // Core subsystems (decoupled from Game)
    Window window_;
    Renderer renderer_;
    InputManager inputManager_;
    ResourceManager resourceManager_;
    
    // Game state
    bool running_;
    bool initialized_;
    
    // Timing
    std::chrono::high_resolution_clock::time_point lastTime_;
    
    // Game objects
    std::vector<std::unique_ptr<Actor>> actors_;
    
    // Constants
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr char WINDOW_TITLE[] = "Infinite Craft Clone";
};

#endif // GAME_HPP
