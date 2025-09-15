#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <memory>

// Forward declaration
class Shader;

/**
 * ResourceManager handles loading and management of game resources.
 * Decouples resource management from the main Game class.
 */
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    /**
     * Initialize the resource manager (SDL_ttf, etc.)
     */
    bool initialize();

    /**
     * Load a shader program from vertex and fragment shader files
     */
    GLuint loadShaderProgram(const std::string& name, 
                           const std::string& vertexPath, 
                           const std::string& fragmentPath);

    /**
     * Get a shader program by name
     */
    GLuint getShaderProgram(const std::string& name) const;

    /**
     * Load a font
     */
    TTF_Font* loadFont(const std::string& name, const std::string& path, int size);

    /**
     * Get a font by name
     */
    TTF_Font* getFont(const std::string& name) const;

    /**
     * Cleanup all resources
     */
    void cleanup();

private:
    /**
     * Create shader program from compiled shaders
     */
    GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);

    std::unordered_map<std::string, GLuint> shaderPrograms_;
    std::unordered_map<std::string, TTF_Font*> fonts_;
    bool initialized_;
};

#endif // RESOURCE_MANAGER_HPP