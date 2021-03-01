#version 330 core
in vec4 ourColor;
out vec4 color;

void main()
{
    color = vec4(ourColor.r,ourColor.g,ourColor.b,ourColor.a);
}