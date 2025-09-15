#include "TextObject.hpp"
#include <iostream>

TextObject::TextObject(float x, float y, const std::string& text, std::shared_ptr<FreeTypeFont> font, glm::vec3 color)
    : x_(x), y_(y), text_(text), font_(font), color_(color), VAO_(0), VBO_(0) {
    
    // Initialize VAO and VBO for text rendering
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

TextObject::~TextObject() {
    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
    }
    if (VBO_ != 0) {
        glDeleteBuffers(1, &VBO_);
    }
}

void TextObject::render() {
    if (font_ && font_->isLoaded()) {
        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        renderText();
        
        glDisable(GL_BLEND);
    }
}

void TextObject::renderText() {
    if (!font_ || !font_->isLoaded()) return;

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_);

    float currentX = x_;

    // Iterate through all characters
    for (char c : text_) {
        const Character& ch = font_->getCharacter(c);

        float xpos = currentX + ch.bearing.x;
        float ypos = y_ - (ch.size.y - ch.bearing.y);

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
