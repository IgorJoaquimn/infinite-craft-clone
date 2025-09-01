#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D textTexture;

void main()
{
    vec4 textColor = texture(textTexture, TexCoords);
    
    // Create a white background for the text area
    // Blend text color with white background based on text alpha
    vec3 backgroundColor = vec3(0, 0, 0); // White background
    vec3 finalColor = mix(backgroundColor, textColor.rgb, textColor.a);
    
    color = vec4(finalColor, 1.0);
}
