#version 330

in  vec2 fragTexcoord;
out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	outColor = texture(uTexture, fragTexcoord);
}