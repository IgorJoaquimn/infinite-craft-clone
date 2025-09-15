#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

// Forward declarations
class Actor;
class GraphicsComponent;

/**
 * Renderer class handles all OpenGL rendering operations.
 * Decouples rendering logic from the main Game class.
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    /**
     * Initialize the renderer
     */
    bool initialize(int windowWidth, int windowHeight);

    /**
     * Begin a new frame (clear buffers, set up state)
     */
    void beginFrame();

    /**
     * End the current frame
     */
    void endFrame();

    /**
     * Render all actors with graphics components
     */
    void renderActors(const std::vector<std::unique_ptr<Actor>>& actors);

    /**
     * Get the projection matrix for 2D rendering
     */
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix_; }

    /**
     * Set clear color
     */
    void setClearColor(float r, float g, float b, float a = 1.0f);

    /**
     * Cleanup renderer resources
     */
    void cleanup();

private:
    /**
     * Setup OpenGL state for 2D rendering
     */
    void setupRenderState();

    glm::mat4 projectionMatrix_;
    float clearColor_[4];
    bool initialized_;
    int windowWidth_;
    int windowHeight_;
};

#endif // RENDERER_HPP