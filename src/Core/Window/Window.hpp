#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <string>

/**
 * Window class handles SDL window creation and management.
 * Decouples window management from the main Game class.
 */
class Window {
public:
    Window();
    ~Window();

    /**
     * Initialize SDL and create the window
     */
    bool initialize(const std::string& title, int width, int height);

    /**
     * Create OpenGL context for the window
     */
    bool createGLContext();

    /**
     * Swap the window buffers (present the frame)
     */
    void swapBuffers();

    /**
     * Cleanup window and SDL resources
     */
    void cleanup();

    /**
     * Check if window is initialized
     */
    bool isInitialized() const { return initialized_; }

    /**
     * Get window dimensions
     */
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    /**
     * Get SDL window pointer (for advanced operations)
     */
    SDL_Window* getSDLWindow() const { return window_; }

    /**
     * Get OpenGL context (for advanced operations)
     */
    SDL_GLContext getGLContext() const { return glContext_; }

private:
    /**
     * Set OpenGL context attributes
     */
    void setGLAttributes();

    SDL_Window* window_;
    SDL_GLContext glContext_;
    bool initialized_;
    int width_;
    int height_;
    std::string title_;
};

#endif // WINDOW_HPP