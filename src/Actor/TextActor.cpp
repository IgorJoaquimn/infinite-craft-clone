#include "TextActor.hpp"
#include "../Core/TextRenderer/TextRenderer.hpp"
#include "../Game/Game.hpp"

TextActor::TextActor(class Game* game, const std::string& text)
    : Actor(game)
    , mText(text)
{
}

void TextActor::OnDraw(class TextRenderer* textRenderer)
{
    if (textRenderer)
    {
        Vector2 pos = GetPosition();
        textRenderer->RenderText(mText, pos.x, pos.y, 1.0f);
    }
}