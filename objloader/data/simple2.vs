#version 330

uniform mat4 uMVP;

in vec3 vertexPosition; 
in vec3 vertexNormal;
in vec2 vertexTexcoord;

out vec2 fragTexcoord;
out vec3 fragColor;

void main()
{
	gl_Position = uMVP * vec4(vertexPosition, 1.0);
	
	fragColor = vec3(mix(0.5,1.0, clamp(vertexNormal.z, 0.0, 1.0)));
	fragTexcoord = vertexTexcoord;
}