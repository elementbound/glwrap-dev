#version 330

in  vec3 fragTexcoord;
out vec4 outColor;

uniform samplerCube uTexture;

void main()
{
	//outColor = vec4((1.0+normalize(fragTexcoord))/2.0, 1.0);
	outColor = texture(uTexture, fragTexcoord);
}