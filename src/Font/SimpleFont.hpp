#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <vector>

struct Character {
    GLuint textureID;   // ID handle of the glyph texture
    int width, height;  // Size of glyph
    int bearingX, bearingY; // Offset from baseline to left/top of glyph
    int advance;        // Offset to advance to next glyph
};

class SimpleFont {
public:
    SimpleFont();
    ~SimpleFont();
    
    bool LoadFont(const std::string& fontPath, int fontSize);
    void RenderText(const std::string& text, float x, float y, float scale = 1.0f);
    
private:
    std::unordered_map<char, Character> characters;
    std::unordered_map<uint32_t, Character> unicodeCharacters; // For emoji and Unicode
    GLuint VAO, VBO;
    GLuint shaderProgram;
    
    bool CreateShaders();
    void LoadCharacters();
    void LoadUnicodeCharacter(uint32_t codepoint);
    std::vector<uint32_t> UTF8ToCodepoints(const std::string& utf8);
    
    // FreeType face for loading additional characters
    void* face; // FT_Face but as void* to avoid including freetype in header
};