#pragma once
#include "../../Font/SimpleFont.hpp"
#include <memory>

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    
    bool Initialize();
    void RenderText(const std::string& text, float x, float y, float scale = 1.0f);
    
private:
    std::unique_ptr<SimpleFont> font;
};