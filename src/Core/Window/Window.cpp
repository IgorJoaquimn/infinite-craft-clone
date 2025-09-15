#include "Window.hpp"
#include <iostream>
#include <GL/glew.h>

Window::Window() 
    : window_(nullptr)
    , glContext_(nullptr)
    , initialized_(false)
    , width_(0)
    , height_(0) {
}

Window::~Window() {
    cleanup();
}

bool Window::initialize(const std::string& title, int width, int height) {
    title_ = title;
    width_ = width;
    height_ = height;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    setGLAttributes();

    // Create window
    window_ = SDL_CreateWindow(
        title_.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width_, height_,
        SDL_WINDOW_OPENGL
    );

    if (!window_) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    initialized_ = true;
    return true;
}

bool Window::createGLContext() {
    if (!initialized_) {
        std::cerr << "Window not initialized before creating GL context!" << std::endl;
        return false;
    }

    glContext_ = SDL_GL_CreateContext(window_);
    if (!glContext_) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewStatus) << std::endl;
        return false;
    }

    return true;
}

void Window::swapBuffers() {
    if (window_) {
        SDL_GL_SwapWindow(window_);
    }
}

void Window::cleanup() {
    if (glContext_) {
        SDL_GL_DeleteContext(glContext_);
        glContext_ = nullptr;
    }

    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    if (initialized_) {
        SDL_Quit();
        initialized_ = false;
    }
}

void Window::setGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}