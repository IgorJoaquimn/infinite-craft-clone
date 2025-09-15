#ifndef TEXT_COMPONENT_HPP
#define TEXT_COMPONENT_HPP

#include "Component/GraphicsComponent.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

/**
 * TextComponent handles text rendering for an Actor.
 * This follows the Component pattern by encapsulating text rendering behavior.
 */
class TextComponent : public GraphicsComponent {
public:
    TextComponent(const std::string& text, TTF_Font* font, SDL_Color color);
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
    void setText(const std::string& text, TTF_Font* font, SDL_Color color);

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
     * Set text color (requires re-rendering the texture).
     */
    void setColor(SDL_Color color, TTF_Font* font);

protected:
    void initializeGraphics() override;
    void cleanupGraphics() override;

private:
    void loadTexture(TTF_Font* font, SDL_Color color);
    void setupVertexData(float x, float y);

    std::string text_;
    SDL_Color color_;
    float width_, height_;
    
    GLuint textureID_;
    GLuint VAO_, VBO_;
};

#endif // TEXT_COMPONENT_HPP