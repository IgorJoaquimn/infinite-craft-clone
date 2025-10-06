// ----------------------------------------------------------------
// Renderer implementation following the asteroids game architecture
// ----------------------------------------------------------------

#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer()
    : mWindowWidth(0)
    , mWindowHeight(0)
{
}

Renderer::~Renderer()
{
    Shutdown();
}

bool Renderer::Initialize(int windowWidth, int windowHeight)
{
    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return false;
    }

    // Set OpenGL attributes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Renderer::BeginFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndFrame()
{
    // Frame is complete
}

void Renderer::Shutdown()
{
    // Cleanup if needed
}