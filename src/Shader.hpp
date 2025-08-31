#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

class Shader {
public:
    Shader(const std::string& filepath, GLenum type);
    ~Shader();

    GLuint getID() const;

private:
    GLuint ID;
    std::string loadShaderSource(const std::string& filepath) const;
    void compile(const std::string& source, GLenum type);
};

#endif // SHADER_HPP
