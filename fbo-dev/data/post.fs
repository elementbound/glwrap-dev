#version 330

in  vec2 fragTexcoord;
out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	vec4 col = vec4(0.0f);
	for(int oy = -3; oy <= 3; oy++)
		for(int ox = -3; ox <= 3; ox++)
			col += textureOffset(uTexture, fragTexcoord, ivec2(ox,oy));
	col /= 49.0;
	outColor = vec4(1.0 - col.rgb, col.a);
}