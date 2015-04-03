#version 330

in vec3 fragColor;
in vec2 fragTexcoord;

out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	outColor = vec4(fragColor * texture(uTexture, fragTexcoord).rgb, 1.0);
}