#version 330

uniform mat4 uMVP;

in vec2 vertexPosition; 
in vec2 vertexTexcoord;

out vec2 fragTexcoord;

void main()
{
	gl_Position = uMVP * vec4(vertexPosition, 0.0, 1.0);
	fragTexcoord = vertexTexcoord;
}