#include "TextComponent.hpp"
#include "Actor/Actor.hpp"
#include <iostream>

TextComponent::TextComponent(const std::string& text, TTF_Font* font, SDL_Color color)
    : text_(text), color_(color), width_(0), height_(0), textureID_(0), VAO_(0), VBO_(0) {
    loadTexture(font, color);
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
    if (shaderProgram_ == 0 || textureID_ == 0) return;

    glUseProgram(shaderProgram_);
    
    // Set projection matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, "projection"), 1, GL_FALSE, &projection[0][0]);
    
    // Tell the shader to use texture unit 0
    glUniform1i(glGetUniformLocation(shaderProgram_, "textTexture"), 0);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    // Update vertex data with current actor position
    setupVertexData(actor.getX(), actor.getY());

    // Render
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextComponent::setText(const std::string& text, TTF_Font* font, SDL_Color color) {
    text_ = text;
    color_ = color;
    
    // Cleanup old texture
    if (textureID_ != 0) {
        glDeleteTextures(1, &textureID_);
        textureID_ = 0;
    }
    
    // Create new texture
    loadTexture(font, color);
}

void TextComponent::setColor(SDL_Color color, TTF_Font* font) {
    if (color.r != color_.r || color.g != color_.g || color.b != color_.b || color.a != color_.a) {
        setText(text_, font, color);
    }
}

void TextComponent::initializeGraphics() {
    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
}

void TextComponent::cleanupGraphics() {
    if (textureID_ != 0) {
        glDeleteTextures(1, &textureID_);
        textureID_ = 0;
    }
    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
        VAO_ = 0;
    }
    if (VBO_ != 0) {
        glDeleteBuffers(1, &VBO_);
        VBO_ = 0;
    }
}

void TextComponent::loadTexture(TTF_Font* font, SDL_Color color) {
    if (!font) {
        std::cerr << "Font is null in TextComponent::loadTexture" << std::endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text_.c_str(), color);
    if (!surface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Convert to a consistent RGBA format
    SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);
    
    if (!rgbaSurface) {
        std::cerr << "Unable to convert surface to RGBA format! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    width_ = rgbaSurface->w;
    height_ = rgbaSurface->h;

    // Create OpenGL texture
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    // Tell OpenGL how SDL_Surface stores its pixels
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaSurface->w, rgbaSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurface->pixels);
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in texture creation: " << error << std::endl;
    }
    
    // Restore default alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free the converted SDL surface
    SDL_FreeSurface(rgbaSurface);
}

void TextComponent::setupVertexData(float x, float y) {
    // Create VAO and VBO for the quad with current position
    float vertices[] = {
        // pos      // tex (flipped vertically to fix upside-down text)
        x,           y,            0.0f, 1.0f,  // bottom-left
        x,           y + height_,  0.0f, 0.0f,  // top-left  
        x + width_,  y + height_,  1.0f, 0.0f,  // top-right

        x,           y,            0.0f, 1.0f,  // bottom-left
        x + width_,  y + height_,  1.0f, 0.0f,  // top-right
        x + width_,  y,            1.0f, 1.0f   // bottom-right
    };

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Vertex attribute (position + texture coordinates)
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}