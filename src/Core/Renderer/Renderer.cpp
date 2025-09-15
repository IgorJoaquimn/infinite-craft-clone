#include "Renderer.hpp"
#include "Actor/Actor.hpp"
#include "Component/GraphicsComponent/GraphicsComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer() 
    : initialized_(false)
    , windowWidth_(0)
    , windowHeight_(0) {
    
    // Default clear color (dark gray)
    clearColor_[0] = 0.1f;
    clearColor_[1] = 0.1f;
    clearColor_[2] = 0.1f;
    clearColor_[3] = 1.0f;
}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::initialize(int windowWidth, int windowHeight) {
    windowWidth_ = windowWidth;
    windowHeight_ = windowHeight;

    // Create projection matrix for 2D rendering
    projectionMatrix_ = glm::ortho(0.0f, static_cast<float>(windowWidth_), 
                                  0.0f, static_cast<float>(windowHeight_));

    setupRenderState();
    
    initialized_ = true;
    return true;
}

void Renderer::beginFrame() {
    if (!initialized_) return;

    // Clear the screen
    glClearColor(clearColor_[0], clearColor_[1], clearColor_[2], clearColor_[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::endFrame() {
    // Nothing specific needed here for now
    // Could add post-processing effects, debug overlays, etc.
}

void Renderer::renderActors(const std::vector<std::unique_ptr<Actor>>& actors) {
    if (!initialized_) return;

    // Enable blending for transparent rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render all actors with graphics components
    for (const auto& actor : actors) {
        if (actor && actor->isActive()) {
            // Get all graphics components and render them
            auto graphicsComponents = actor->getComponents<GraphicsComponent>();
            for (auto* graphics : graphicsComponents) {
                if (graphics && graphics->isActive()) {
                    graphics->render(*actor, projectionMatrix_);
                }
            }
        }
    }

    glDisable(GL_BLEND);
}

void Renderer::setClearColor(float r, float g, float b, float a) {
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
    clearColor_[3] = a;
}

void Renderer::cleanup() {
    // Any OpenGL cleanup would go here
    initialized_ = false;
}

void Renderer::setupRenderState() {
    // Enable depth testing (if needed for 3D in the future)
    // glEnable(GL_DEPTH_TEST);
    
    // Set viewport
    glViewport(0, 0, windowWidth_, windowHeight_);
    
    // Other OpenGL state setup can go here
}