#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform vec2 Position;
uniform vec2 Scale;

void main()
{
    gl_Position = vec4((aPos.x * Scale.x) + Position.x, (aPos.y * Scale.y) + Position.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  