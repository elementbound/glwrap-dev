#version 330

in  vec2 fragTexcoord;
out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	vec4 col = texture(uTexture, fragTexcoord);
	outColor = vec4(1.0 - col.rgb, col.a);
}