#ifndef GRAPHICS_COMPONENT_HPP
#define GRAPHICS_COMPONENT_HPP

#include "Component/Component/Component.hpp"
#include "Shader/Shader.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

/**
 * GraphicsComponent handles rendering for an Actor.
 * This follows the Component pattern by encapsulating graphics-related behavior.
 */
class GraphicsComponent : public Component {
public:
    GraphicsComponent() : shaderProgram_(0) {}
    
    virtual ~GraphicsComponent() {
        cleanup();
    }

    /**
     * Update method called once per frame.
     * For graphics components, this might update animations or visual effects.
     */
    void update(Actor& actor, float deltaTime) override {
        // Graphics components often don't need frame-by-frame updates
        // unless dealing with animations or effects
        // This method can be overridden by derived classes for specific behavior
    }

    /**
     * Render the component. This should be called during the render phase,
     * not during the update phase.
     */
    virtual void render(const Actor& actor, const glm::mat4& projection) {
        // Base implementation does nothing
        // Derived classes should implement specific rendering logic
    }

    /**
     * Set the shader program for this graphics component.
     */
    void setShaderProgram(GLuint program) {
        shaderProgram_ = program;
    }

    /**
     * Get the current shader program.
     */
    GLuint getShaderProgram() const {
        return shaderProgram_;
    }

    /**
     * Initialize the graphics component.
     */
    void initialize() override {
        // Setup any OpenGL resources here
        initializeGraphics();
    }

    /**
     * Cleanup graphics resources.
     */
    void cleanup() override {
        cleanupGraphics();
    }

protected:
    /**
     * Initialize graphics-specific resources.
     * Override in derived classes for specific setup.
     */
    virtual void initializeGraphics() {}

    /**
     * Cleanup graphics-specific resources.
     * Override in derived classes for specific cleanup.
     */
    virtual void cleanupGraphics() {}

    GLuint shaderProgram_;
};

#endif // GRAPHICS_COMPONENT_HPP