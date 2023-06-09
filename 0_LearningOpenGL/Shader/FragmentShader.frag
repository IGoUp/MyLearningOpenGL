#version 330 core

out vec4 fragColor;
in vec3 ourColor;
uniform float ratio;

void main()
{
	fragColor = vec4(ourColor * ratio, 1.0);
}