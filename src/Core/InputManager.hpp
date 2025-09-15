#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>

/**
 * InputManager handles all input events and input state.
 * Decouples input handling from the main Game class.
 */
class InputManager {
public:
    // Event callback types
    using QuitCallback = std::function<void()>;
    using KeyCallback = std::function<void(SDL_Keycode key, bool pressed)>;
    using MouseButtonCallback = std::function<void(int button, int x, int y, bool pressed)>;
    using MouseMotionCallback = std::function<void(int x, int y, int deltaX, int deltaY)>;

    InputManager();
    ~InputManager();

    /**
     * Process all pending SDL events
     */
    void processEvents();

    /**
     * Check if a specific key is currently pressed
     */
    bool isKeyPressed(SDL_Keycode key) const;

    /**
     * Get current mouse position
     */
    void getMousePosition(int& x, int& y) const;

    /**
     * Check if a mouse button is currently pressed
     */
    bool isMouseButtonPressed(int button) const;

    // Event callback registration
    void setQuitCallback(QuitCallback callback) { quitCallback_ = callback; }
    void setKeyCallback(KeyCallback callback) { keyCallback_ = callback; }
    void setMouseButtonCallback(MouseButtonCallback callback) { mouseButtonCallback_ = callback; }
    void setMouseMotionCallback(MouseMotionCallback callback) { mouseMotionCallback_ = callback; }

private:
    // Event callbacks
    QuitCallback quitCallback_;
    KeyCallback keyCallback_;
    MouseButtonCallback mouseButtonCallback_;
    MouseMotionCallback mouseMotionCallback_;

    // Input state
    std::unordered_map<SDL_Keycode, bool> keyStates_;
    std::unordered_map<int, bool> mouseButtonStates_;
    int mouseX_, mouseY_;
};

#endif // INPUT_MANAGER_HPP