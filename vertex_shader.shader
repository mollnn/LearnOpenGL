#version 330 core
layout (location = 0) in vec3 position; // position变量的属性位置值为0
uniform float rtime;
out vec4 vertexColor; // 为片段着色器指定一个颜色输出
out vec4 pos;

void main()
{
    gl_Position = vec4(position, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    pos = gl_Position;
    vertexColor = vec4(0.5f + position.x, 0.5f + position.y, rtime, 1.0f); // 把输出变量设置为暗红色
}