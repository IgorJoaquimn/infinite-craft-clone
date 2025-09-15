#ifndef FREETYPE_FONT_HPP
#define FREETYPE_FONT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

/**
 * Character structure to hold glyph information
 */
struct Character {
    GLuint textureID;   // ID handle of the glyph texture
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing; // Offset from baseline to left/top of glyph
    GLuint advance;     // Offset to advance to next glyph
};

/**
 * FreeTypeFont class handles FreeType font loading and glyph rendering
 */
class FreeTypeFont {
public:
    FreeTypeFont();
    ~FreeTypeFont();

    /**
     * Initialize FreeType library
     */
    static bool initializeLibrary();

    /**
     * Cleanup FreeType library
     */
    static void cleanupLibrary();

    /**
     * Load font from file with specified size
     */
    bool loadFont(const std::string& fontPath, int fontSize);

    /**
     * Get character glyph information
     */
    const Character& getCharacter(char c) const;

    /**
     * Check if font is loaded
     */
    bool isLoaded() const { return loaded_; }

    /**
     * Get font size
     */
    int getFontSize() const { return fontSize_; }

    /**
     * Calculate text dimensions
     */
    glm::ivec2 getTextSize(const std::string& text) const;

private:
    /**
     * Load character glyphs for ASCII range
     */
    void loadCharacters();

    static FT_Library ft_;
    static bool libraryInitialized_;
    
    FT_Face face_;
    bool loaded_;
    int fontSize_;
    std::unordered_map<char, Character> characters_;
};

#endif // FREETYPE_FONT_HPP