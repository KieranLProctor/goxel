#version 330 core
in vec3 vColour;
out vec4 FragColour;

void main()
{
    FragColour = vec4(vColour, 1.0);
}