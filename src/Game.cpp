#include "Game.hpp"
#include "Shader.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

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

    void setupTriangle(GLuint& VAO, GLuint& VBO) {
        float vertices[] = {
            0.0f,  0.5f, 0.0f,
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void cleanup(GLuint VAO, GLuint VBO, GLuint shaderProgram, SDL_GLContext glContext, SDL_Window* window) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

Game::Game() {}
Game::~Game() {}

int Game::run() {
    if (!initSDL())
        return 1;

    setGLAttributes();

    SDL_Window* window = createWindow();
    if (!window) {
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glContext = createGLContext(window);
    if (!glContext) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!initGLEW()) {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Shader vertexShader("shaders/vertex.vert", GL_VERTEX_SHADER);
    Shader fragmentShader("shaders/fragment.frag", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);
    if (!shaderProgram) {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    GLuint VAO, VBO;
    setupTriangle(VAO, VBO);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    cleanup(VAO, VBO, shaderProgram, glContext, window);

    return 0;
}
