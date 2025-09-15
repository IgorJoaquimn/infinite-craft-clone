#include "InputManager.hpp"

InputManager::InputManager() 
    : mouseX_(0)
    , mouseY_(0) {
}

InputManager::~InputManager() {
}

void InputManager::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                if (quitCallback_) {
                    quitCallback_();
                }
                break;

            case SDL_KEYDOWN:
                keyStates_[event.key.keysym.sym] = true;
                if (keyCallback_) {
                    keyCallback_(event.key.keysym.sym, true);
                }
                break;

            case SDL_KEYUP:
                keyStates_[event.key.keysym.sym] = false;
                if (keyCallback_) {
                    keyCallback_(event.key.keysym.sym, false);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouseButtonStates_[event.button.button] = true;
                if (mouseButtonCallback_) {
                    mouseButtonCallback_(event.button.button, event.button.x, event.button.y, true);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                mouseButtonStates_[event.button.button] = false;
                if (mouseButtonCallback_) {
                    mouseButtonCallback_(event.button.button, event.button.x, event.button.y, false);
                }
                break;

            case SDL_MOUSEMOTION:
                {
                    int deltaX = event.motion.x - mouseX_;
                    int deltaY = event.motion.y - mouseY_;
                    mouseX_ = event.motion.x;
                    mouseY_ = event.motion.y;
                    
                    if (mouseMotionCallback_) {
                        mouseMotionCallback_(mouseX_, mouseY_, deltaX, deltaY);
                    }
                }
                break;

            default:
                break;
        }
    }
}

bool InputManager::isKeyPressed(SDL_Keycode key) const {
    auto it = keyStates_.find(key);
    return it != keyStates_.end() && it->second;
}

void InputManager::getMousePosition(int& x, int& y) const {
    x = mouseX_;
    y = mouseY_;
}

bool InputManager::isMouseButtonPressed(int button) const {
    auto it = mouseButtonStates_.find(button);
    return it != mouseButtonStates_.end() && it->second;
}