#include "Game.hpp"
#include "Component/TextComponent.hpp"
#include <iostream>

// === GAME CLASS IMPLEMENTATION ===

Game::Game() 
    : running_(false)
    , initialized_(false) {
}

Game::~Game() {
    unload();
}

int Game::run() {
    if (!load()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    if (!loadContent()) {
        std::cerr << "Failed to load game content!" << std::endl;
        return 1;
    }

    gameLoop();
    
    return 0;
}

// === ACTOR MANAGEMENT ===

void Game::addActor(std::unique_ptr<Actor> actor) {
    actors_.push_back(std::move(actor));
}

void Game::removeActor(Actor* actor) {
    actors_.erase(
        std::remove_if(actors_.begin(), actors_.end(),
            [actor](const std::unique_ptr<Actor>& a) {
                return a.get() == actor;
            }),
        actors_.end()
    );
}

const std::vector<std::unique_ptr<Actor>>& Game::getActors() const {
    return actors_;
}

void Game::clearActors() {
    actors_.clear();
}

// === LIFECYCLE FUNCTIONS ===

bool Game::load() {
    // Initialize window
    if (!window_.initialize(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return false;
    }

    // Create OpenGL context
    if (!window_.createGLContext()) {
        return false;
    }

    // Initialize renderer
    if (!renderer_.initialize(WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return false;
    }

    // Initialize resource manager
    if (!resourceManager_.initialize()) {
        return false;
    }

    // Setup input callbacks
    setupInput();

    initialized_ = true;
    return true;
}

bool Game::loadContent() {
    if (!initialized_) {
        std::cerr << "Game not initialized before loading content!" << std::endl;
        return false;
    }

    // Load shaders
    GLuint textShaderProgram = resourceManager_.loadShaderProgram(
        "text", "shaders/text.vert", "shaders/text.frag");
    
    if (textShaderProgram == 0) {
        return false;
    }

    // Load fonts
    auto font = resourceManager_.loadFont(
        "main", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 28);
    
    if (!font) {
        return false;
    }

    // Create initial game objects
    return createGameObjects();
}

void Game::gameLoop() {
    running_ = true;
    lastTime_ = std::chrono::high_resolution_clock::now();

    while (running_) {
        float deltaTime = calculateDeltaTime();
        
        // Handle input
        inputManager_.processEvents();
        
        // Update game logic
        update(deltaTime);
        
        // Render
        render();

        // Present frame
        window_.swapBuffers();
    }
}

void Game::update(float deltaTime) {
    updateActors(deltaTime);
}

void Game::render() {
    renderer_.beginFrame();
    renderer_.renderActors(actors_);
    renderer_.endFrame();
}

void Game::unload() {
    // Clean up actors first
    clearActors();

    // Clean up subsystems (in reverse order of initialization)
    resourceManager_.cleanup();
    renderer_.cleanup();
    window_.cleanup();

    initialized_ = false;
}

// === HELPER FUNCTIONS ===

void Game::setupInput() {
    // Setup input callbacks using lambdas
    inputManager_.setQuitCallback([this]() {
        onQuit();
    });

    inputManager_.setKeyCallback([this](SDL_Keycode key, bool pressed) {
        onKeyEvent(key, pressed);
    });

    inputManager_.setMouseButtonCallback([this](int button, int x, int y, bool pressed) {
        onMouseButton(button, x, y, pressed);
    });
}

bool Game::createGameObjects() {
    // Get resources
    GLuint textShaderProgram = resourceManager_.getShaderProgram("text");
    auto font = resourceManager_.getFont("main");

    if (textShaderProgram == 0 || !font) {
        std::cerr << "Required resources not loaded!" << std::endl;
        return false;
    }

    // Create initial game objects
    glm::vec3 textColor(1.0f, 0.0f, 0.0f); // Red text

    // Create an actor with a text component
    auto textActor = std::make_unique<Actor>(0.0f, 250.0f);
    auto textComponent = textActor->addComponent<TextComponent>("Game Text", font, textColor);
    textComponent->setShaderProgram(textShaderProgram);
    
    addActor(std::move(textActor));
    
    std::cout << "TextComponent created successfully" << std::endl;
    return true;
}

void Game::updateActors(float deltaTime) {
    // Remove inactive actors
    actors_.erase(
        std::remove_if(actors_.begin(), actors_.end(),
            [](const std::unique_ptr<Actor>& actor) {
                return !actor->isActive();
            }),
        actors_.end()
    );

    // Update all active actors - this implements the Update Method pattern
    for (auto& actor : actors_) {
        if (actor && actor->isActive()) {
            actor->update(deltaTime);
        }
    }
}

float Game::calculateDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime_).count();
    lastTime_ = currentTime;
    
    // Cap delta time to prevent large jumps (e.g., when debugging)
    const float maxDeltaTime = 1.0f / 60.0f; // 60 FPS
    return std::min(deltaTime, maxDeltaTime);
}

// === EVENT HANDLERS ===

void Game::onQuit() {
    running_ = false;
}

void Game::onKeyEvent(SDL_Keycode key, bool pressed) {
    if (pressed) {
        switch (key) {
            case SDLK_ESCAPE:
                running_ = false;
                break;
            case SDLK_SPACE:
                std::cout << "Space key pressed!" << std::endl;
                break;
            default:
                break;
        }
    }
}

void Game::onMouseButton(int button, int x, int y, bool pressed) {
    if (pressed) {
        std::cout << "Mouse button " << button << " clicked at (" << x << ", " << y << ")" << std::endl;
    }
}


