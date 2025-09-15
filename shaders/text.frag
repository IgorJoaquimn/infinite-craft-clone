#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D textTexture;

void main()
{
    vec4 textColor = texture(textTexture, TexCoords);
    
    // Show the text without any background processing
    color = textColor;
}
