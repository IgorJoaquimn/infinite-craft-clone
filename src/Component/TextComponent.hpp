#ifndef TEXT_COMPONENT_HPP
#define TEXT_COMPONENT_HPP

#include "Component/GraphicsComponent.hpp"
#include "Font/FreeTypeFont.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>

/**
 * TextComponent handles text rendering for an Actor.
 * This follows the Component pattern by encapsulating text rendering behavior.
 */
class TextComponent : public GraphicsComponent {
public:
    TextComponent(const std::string& text, std::shared_ptr<FreeTypeFont> font, glm::vec3 color);
    virtual ~TextComponent();

    /**
     * Update method - for text, this might handle text animations or changes.
     */
    void update(Actor& actor, float deltaTime) override;

    /**
     * Render the text component.
     */
    void render(const Actor& actor, const glm::mat4& projection) override;

    /**
     * Change the text content.
     */
    void setText(const std::string& text, std::shared_ptr<FreeTypeFont> font, glm::vec3 color);

    /**
     * Get the current text content.
     */
    const std::string& getText() const { return text_; }

    /**
     * Get text dimensions.
     */
    float getWidth() const { return width_; }
    float getHeight() const { return height_; }

    /**
     * Set text color.
     */
    void setColor(glm::vec3 color);

protected:
    void initializeGraphics() override;
    void cleanupGraphics() override;

private:
    void renderText(const std::string& text, float x, float y);

    std::string text_;
    std::shared_ptr<FreeTypeFont> font_;
    glm::vec3 color_;
    float width_, height_;
    
    GLuint VAO_, VBO_;
};

#endif // TEXT_COMPONENT_HPP