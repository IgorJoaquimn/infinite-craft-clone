#include "Game.hpp"
#include "Shader/Shader.hpp"
#include "TextObject/TextObject.hpp" // Make sure to include your TextObject
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Include SDL_ttf
#include <GL/glew.h>
#include <iostream>

// Include GLM for matrix transformations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr char WINDOW_TITLE[] = "Infinite Craft Clone";

    bool initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }
        return true;
    }

    void setGLAttributes() {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }

    SDL_Window* createWindow() {
        SDL_Window* window = SDL_CreateWindow(
            WINDOW_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL
        );
        if (!window) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        return window;
    }

    SDL_GLContext createGLContext(SDL_Window* window) {
        SDL_GLContext context = SDL_GL_CreateContext(window);
        if (!context) {
            std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        return context;
    }

    bool initGLEW() {
        glewExperimental = GL_TRUE;
        GLenum glewStatus = glewInit();
        if (glewStatus != GLEW_OK) {
            std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewStatus) << std::endl;
            return false;
        }
        return true;
    }

    GLuint createShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader.getID());
        glAttachShader(program, fragmentShader.getID());
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

    // Simplified cleanup function without triangle variables
    void cleanup(SDL_GLContext glContext, SDL_Window* window) {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
}

Game::Game() {}
Game::~Game() {}

int Game::run() {
    if (!initSDL()) return 1;

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    setGLAttributes();
    SDL_Window* window = createWindow();
    SDL_GLContext glContext = createGLContext(window);
    if (!initGLEW()) { /* ... error handling ... */ }

    // --- Shaders ---
    // We only need the text shaders now
    Shader textVertexShader("shaders/text.vert", GL_VERTEX_SHADER);
    Shader textFragmentShader("shaders/text.frag", GL_FRAGMENT_SHADER);
    GLuint textShaderProgram = createShaderProgram(textVertexShader, textFragmentShader);
    if (!textShaderProgram) { /* ... error handling ... */ return 1; }

    // --- Game Objects ---
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 28);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }
    SDL_Color textColor = { 255, 0, 0, 255 }; // Red text for debugging
    TextObject myText(300.0f, 250.0f, "This should work!", font, textColor);
    
    std::cout << "TextObject created successfully" << std::endl;

    // --- Projection Matrix for 2D Rendering ---
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));

    // --- Main Loop ---
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // --- Draw the text object ---
        // Disable blending since we handle background in shader
        glDisable(GL_BLEND);

        glUseProgram(textShaderProgram);

        // ADDED: Tell the shader to use texture unit 0 for the 'textTexture' sampler
        glUniform1i(glGetUniformLocation(textShaderProgram, "textTexture"), 0);

        // Set the projection matrix uniform
        glUniformMatrix4fv(glGetUniformLocation(textShaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
        
        myText.render(); // Render the text object

        SDL_GL_SwapWindow(window);
    }

    // --- Cleanup ---
    TTF_CloseFont(font);
    glDeleteProgram(textShaderProgram); // Clean up the text shader program
    cleanup(glContext, window);

    return 0;
}
