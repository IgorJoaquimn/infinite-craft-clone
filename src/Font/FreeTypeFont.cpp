#include "FreeTypeFont.hpp"
#include <iostream>

// Static member definitions
FT_Library FreeTypeFont::ft_;
bool FreeTypeFont::libraryInitialized_ = false;

FreeTypeFont::FreeTypeFont() 
    : face_(nullptr), loaded_(false), fontSize_(0) {
}

FreeTypeFont::~FreeTypeFont() {
    if (loaded_) {
        // Clean up character textures
        for (auto& pair : characters_) {
            glDeleteTextures(1, &pair.second.textureID);
        }
        characters_.clear();
        
        // Clean up face
        FT_Done_Face(face_);
        loaded_ = false;
    }
}

bool FreeTypeFont::initializeLibrary() {
    if (libraryInitialized_) {
        return true;
    }

    if (FT_Init_FreeType(&ft_)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    libraryInitialized_ = true;
    std::cout << "FreeType library initialized successfully" << std::endl;
    return true;
}

void FreeTypeFont::cleanupLibrary() {
    if (libraryInitialized_) {
        FT_Done_FreeType(ft_);
        libraryInitialized_ = false;
        std::cout << "FreeType library cleaned up" << std::endl;
    }
}

bool FreeTypeFont::loadFont(const std::string& fontPath, int fontSize) {
    if (!libraryInitialized_) {
        std::cerr << "ERROR::FREETYPE: Library not initialized" << std::endl;
        return false;
    }

    if (FT_New_Face(ft_, fontPath.c_str(), 0, &face_)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font from " << fontPath << std::endl;
        return false;
    }

    // Set pixel size
    FT_Set_Pixel_Sizes(face_, 0, fontSize);
    fontSize_ = fontSize;

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load characters
    loadCharacters();

    // Restore default alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    loaded_ = true;
    std::cout << "Font loaded successfully: " << fontPath << " (size: " << fontSize << ")" << std::endl;
    return true;
}

void FreeTypeFont::loadCharacters() {
    // Load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face_, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character '" << c << "'" << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face_->glyph->bitmap.width,
            face_->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face_->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character = {
            texture,
            glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows),
            glm::ivec2(face_->glyph->bitmap_left, face_->glyph->bitmap_top),
            static_cast<GLuint>(face_->glyph->advance.x)
        };
        characters_[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

const Character& FreeTypeFont::getCharacter(char c) const {
    auto it = characters_.find(c);
    if (it != characters_.end()) {
        return it->second;
    }
    
    // Return space character as fallback
    static Character fallback = {};
    auto spaceIt = characters_.find(' ');
    if (spaceIt != characters_.end()) {
        return spaceIt->second;
    }
    return fallback;
}

glm::ivec2 FreeTypeFont::getTextSize(const std::string& text) const {
    if (!loaded_ || text.empty()) {
        return glm::ivec2(0, 0);
    }

    int width = 0;
    int height = 0;

    for (char c : text) {
        const Character& ch = getCharacter(c);
        width += (ch.advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
        height = std::max(height, ch.size.y);
    }

    return glm::ivec2(width, height);
}