#version 330

in  vec2 fragTexcoord;
out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	vec4 col = vec4(0.0f);
	for(int oy = -5; oy <= 5; oy++)
		for(int ox = -5; ox <= 5; ox++)
			col += textureOffset(uTexture, fragTexcoord, ivec2(ox,oy));
	col /= 121.0;
	outColor = col; //vec4(1.0 - col.rgb, col.a);
}