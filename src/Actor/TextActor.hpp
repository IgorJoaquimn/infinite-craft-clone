#pragma once
#include "../Actor/Actor.hpp"
#include <string>

class TextActor : public Actor
{
public:
    TextActor(class Game* game, const std::string& text);
    
    void SetText(const std::string& text) { mText = text; }
    const std::string& GetText() const { return mText; }
    
protected:
    void OnDraw(class TextRenderer* textRenderer) override;
    
private:
    std::string mText;
};