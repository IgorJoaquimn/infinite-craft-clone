#include "TextObject.hpp"
#include <iostream>

TextObject::TextObject(float x, float y, const std::string& text, TTF_Font* font, SDL_Color color)
    : x(x), y(y), text(text), textureID(0), VAO(0), VBO(0) {
    loadTexture(font, color);
}

TextObject::~TextObject() {
    glDeleteTextures(1, &textureID);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TextObject::loadTexture(TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
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

    this->width = rgbaSurface->w;
    this->height = rgbaSurface->h;

    // Create OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Tell OpenGL how SDL_Surface stores its pixels
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaSurface->w, rgbaSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurface->pixels);
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in texture creation: " << error << std::endl;
    }
    
    // ADDED: Restore default alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free the converted SDL surface
    SDL_FreeSurface(rgbaSurface);

    // Create VAO and VBO for the quad
    float vertices[] = {
        // pos      // tex (flipped vertically to fix upside-down text)
        x,         y,          0.0f, 1.0f,  // bottom-left
        x,         y + height, 0.0f, 0.0f,  // top-left  
        x + width, y + height, 1.0f, 0.0f,  // top-right

        x,         y,          0.0f, 1.0f,  // bottom-left
        x + width, y + height, 1.0f, 0.0f,  // top-right
        x + width, y,          1.0f, 1.0f   // bottom-right
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextObject::render() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
