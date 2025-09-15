#include "ResourceManager.hpp"
#include "Shader/Shader.hpp"
#include <iostream>

ResourceManager::ResourceManager() 
    : initialized_(false) {
}

ResourceManager::~ResourceManager() {
    cleanup();
}

bool ResourceManager::initialize() {
    if (!FreeTypeFont::initializeLibrary()) {
        std::cerr << "FreeType could not initialize!" << std::endl;
        return false;
    }

    initialized_ = true;
    return true;
}

GLuint ResourceManager::loadShaderProgram(const std::string& name, 
                                        const std::string& vertexPath, 
                                        const std::string& fragmentPath) {
    if (!initialized_) {
        std::cerr << "ResourceManager not initialized!" << std::endl;
        return 0;
    }

    try {
        Shader vertexShader(vertexPath, GL_VERTEX_SHADER);
        Shader fragmentShader(fragmentPath, GL_FRAGMENT_SHADER);
        
        GLuint program = createShaderProgram(vertexShader.getID(), fragmentShader.getID());
        
        if (program != 0) {
            shaderPrograms_[name] = program;
            std::cout << "Loaded shader program: " << name << std::endl;
        }
        
        return program;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shader program " << name << ": " << e.what() << std::endl;
        return 0;
    }
}

GLuint ResourceManager::getShaderProgram(const std::string& name) const {
    auto it = shaderPrograms_.find(name);
    return it != shaderPrograms_.end() ? it->second : 0;
}

std::shared_ptr<FreeTypeFont> ResourceManager::loadFont(const std::string& name, const std::string& path, int size) {
    if (!initialized_) {
        std::cerr << "ResourceManager not initialized!" << std::endl;
        return nullptr;
    }

    auto font = std::make_shared<FreeTypeFont>();
    if (!font->loadFont(path, size)) {
        std::cerr << "Failed to load font " << name << " from " << path << std::endl;
        return nullptr;
    }

    fonts_[name] = font;
    std::cout << "Loaded font: " << name << std::endl;
    return font;
}

std::shared_ptr<FreeTypeFont> ResourceManager::getFont(const std::string& name) const {
    auto it = fonts_.find(name);
    return it != fonts_.end() ? it->second : nullptr;
}

void ResourceManager::cleanup() {
    // Clean up shader programs
    for (auto& pair : shaderPrograms_) {
        glDeleteProgram(pair.second);
    }
    shaderPrograms_.clear();

    // Clean up fonts
    fonts_.clear();

    if (initialized_) {
        FreeTypeFont::cleanupLibrary();
        initialized_ = false;
    }
}

GLuint ResourceManager::createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    return program;
}