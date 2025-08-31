#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath, GLenum type) {
    std::string code = loadShaderSource(filepath);
    compile(code, type);
}

Shader::~Shader() {
    glDeleteShader(ID);
}

GLuint Shader::getID() const {
    return ID;
}

std::string Shader::loadShaderSource(const std::string& filepath) const {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::compile(const std::string& source, GLenum type) {
    ID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(ID, 1, &src, nullptr);
    glCompileShader(ID);

    GLint success;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
    }
}
