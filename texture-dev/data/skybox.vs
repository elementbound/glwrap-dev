#version 330

uniform mat4 uMVP;

in vec3 vertexPosition; 
in vec3 vertexNormal;
in vec2 vertexTexcoord;

out vec3 fragTexcoord;

void main()
{
	gl_Position = uMVP * vec4(vertexPosition, 1.0);
	fragTexcoord = -vertexNormal;
}