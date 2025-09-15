#ifndef TEXT_OBJECT_HPP
#define TEXT_OBJECT_HPP

#include "Font/FreeTypeFont.hpp"
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class TextObject {
public:
    TextObject(float x, float y, const std::string& text, std::shared_ptr<FreeTypeFont> font, glm::vec3 color);
    ~TextObject();

    void render();

private:
    std::string text_;
    float x_, y_;
    std::shared_ptr<FreeTypeFont> font_;
    glm::vec3 color_;

    GLuint VAO_, VBO_;

    void renderText();
};

#endif // TEXT_OBJECT_HPP
