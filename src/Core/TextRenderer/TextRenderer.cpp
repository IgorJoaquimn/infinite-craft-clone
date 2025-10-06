#include "TextRenderer.hpp"
#include <iostream>

TextRenderer::TextRenderer() {
}

TextRenderer::~TextRenderer() {
}

bool TextRenderer::Initialize() {
    font = std::make_unique<SimpleFont>();
    
    // Try system font first for better ASCII character support
    if (!font->LoadFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24)) {
        if (!font->LoadFont("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 24)) {
            if (!font->LoadFont("assets/NotoColorEmoji-Regular.ttf", 24)) {
                std::cout << "ERROR: Could not load any font!" << std::endl;
                return false;
            }
        }
    }
    
    return true;
}

void TextRenderer::RenderText(const std::string& text, float x, float y, float scale) {
    if (font) {
        font->RenderText(text, x, y, scale);
    }
}