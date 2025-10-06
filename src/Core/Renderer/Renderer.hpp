// ----------------------------------------------------------------
// Renderer class following the asteroids game architecture
// ----------------------------------------------------------------

#pragma once
#include <GL/glew.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Initialize(int windowWidth, int windowHeight);
    void BeginFrame();
    void EndFrame();
    void Shutdown();

private:
    int mWindowWidth;
    int mWindowHeight;
};