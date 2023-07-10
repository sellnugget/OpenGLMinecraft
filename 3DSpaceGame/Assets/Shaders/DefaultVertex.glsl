#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcord;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 projection;
uniform vec3 CurrentColor;
uniform vec3 aColor;

uniform vec3 CameraPosition;



out vec3 ourColor;
out vec3 Normal;
out vec4 Pos;
out vec3 FragPos;
out vec2 TexCord;

//Global Modifiable vars in all shaders

vec3 PositionOffset;



mat4 BuildTranslation(vec3 delta)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(delta, 1.0));
}



void main()
{
 
	mat4 pos = BuildTranslation(PositionOffset);
	gl_Position = projection * View * pos * Model * vec4(aPos, 1.0);
	ourColor = CurrentColor;
	Normal = (vec4(normal,1.0)).xyz;
	Pos = Model * vec4(aPos, 1.0);
	FragPos = vec3(Model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(Model))) * normal;
	TexCord = texcord;
}

