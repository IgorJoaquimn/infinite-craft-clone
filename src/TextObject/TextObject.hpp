#ifndef TEXT_OBJECT_HPP
#define TEXT_OBJECT_HPP

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class TextObject {
public:
    TextObject(float x, float y, const std::string& text, TTF_Font* font, SDL_Color color);
    ~TextObject();

    void render();

private:
    std::string text;
    float x, y;
    float width, height;

    GLuint textureID;
    GLuint VAO, VBO;

    void loadTexture(TTF_Font* font, SDL_Color color);
};

#endif // TEXT_OBJECT_HPP
