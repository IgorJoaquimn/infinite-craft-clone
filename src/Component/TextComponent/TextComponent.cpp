#include "TextComponent.hpp"
#include "Actor/Actor.hpp"
#include <iostream>

TextComponent::TextComponent(const std::string& text, std::shared_ptr<FreeTypeFont> font, glm::vec3 color)
    : text_(text), font_(font), color_(color), width_(0), height_(0), VAO_(0), VBO_(0) {
    if (font_) {
        glm::ivec2 textSize = font_->getTextSize(text_);
        width_ = textSize.x;
        height_ = textSize.y;
    }
}

TextComponent::~TextComponent() {
    cleanupGraphics();
}

void TextComponent::update(Actor& actor, float deltaTime) {
    // Text components typically don't need frame-by-frame updates
    // unless dealing with animations or dynamic text changes
    // This can be extended for text effects like typewriter effect, etc.
}

void TextComponent::render(const Actor& actor, const glm::mat4& projection) {
    if (shaderProgram_ == 0 || !font_ || !font_->isLoaded()) return;

    glUseProgram(shaderProgram_);
    
    // Set projection matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, "projection"), 1, GL_FALSE, &projection[0][0]);
    
    // Set text color
    glUniform3f(glGetUniformLocation(shaderProgram_, "textColor"), color_.r, color_.g, color_.b);
    
    // Tell the shader to use texture unit 0
    glUniform1i(glGetUniformLocation(shaderProgram_, "text"), 0);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render text
    renderText(text_, actor.getX(), actor.getY());

    glDisable(GL_BLEND);
}

void TextComponent::setText(const std::string& text, std::shared_ptr<FreeTypeFont> font, glm::vec3 color) {
    text_ = text;
    font_ = font;
    color_ = color;
    
    if (font_) {
        glm::ivec2 textSize = font_->getTextSize(text_);
        width_ = textSize.x;
        height_ = textSize.y;
    }
}

void TextComponent::setColor(glm::vec3 color) {
    color_ = color;
}

void TextComponent::initializeGraphics() {
    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextComponent::cleanupGraphics() {
    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
        VAO_ = 0;
    }
    if (VBO_ != 0) {
        glDeleteBuffers(1, &VBO_);
        VBO_ = 0;
    }
}

void TextComponent::renderText(const std::string& text, float x, float y) {
    if (!font_ || !font_->isLoaded()) return;

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_);

    float currentX = x;

    // Iterate through all characters
    for (char c : text) {
        const Character& ch = font_->getCharacter(c);

        float xpos = currentX + ch.bearing.x;
        float ypos = y - (ch.size.y - ch.bearing.y);

        float w = ch.size.x;
        float h = ch.size.y;

        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        currentX += (ch.advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}