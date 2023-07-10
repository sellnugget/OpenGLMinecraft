#version 330 core 
out vec4 FragColor;

in vec2 TexCoords;



uniform sampler2D screenTexture;

uniform vec2 TextureScale;
uniform vec2 TextureOffset;


void main()
{
	vec2 cord = TexCoords;
	cord.y = 1 - TexCoords.y;
	vec4 Col = texture2D(screenTexture, cord * TextureScale + TextureOffset);
	if(Col.a < 1){
		discard;
	}
	FragColor = Col;
}