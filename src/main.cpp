#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "shader.hpp"
using namespace std;

// settings
const unsigned int SCR_WIDTH = 512;
const unsigned int SCR_HEIGHT = 512;

void render();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void ReadShaderSource(std::string filename, GLchar *p_GLchar)
{
	std::ifstream t;
	int length;
	t.open(filename);							   // open input file
	t.seekg(0, std::ios::end);					   // go to the end
	length = t.tellg();							   // report location (this is the length)
	t.seekg(0, std::ios::beg);					   // go back to the beginning
	t.read(static_cast<char *>(p_GLchar), length); // read the whole file into the buffer
	t.close();									   // close file handle

	cout << "read succ!" << endl;
	cout << p_GLchar << endl;
	cout << endl;
}

int main()
{
	// 窗口初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 渲染准备工作
	// 准备着色器

	Shader shader("vertex_shader.shader", "fragment_shader.shader");

	// 准备顶点数据

	GLfloat vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f	  // 顶部
	};

	GLuint indices[] = {
		0, 1, 2};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// 绑定顶点数组对象
	glBindVertexArray(VAO);
	// 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 设定并启用顶点属性指针
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// 解绑VAO（不是EBO！）
	glBindVertexArray(0);

	// 程序循环
	while (!glfwWindowShouldClose(window))
	{
		// 渲染过程开始

		shader.Use();
		// GLfloat timeValue = fmod(glfwGetTime(), 2.0f);
		// if (timeValue > 1.0f)
		// timeValue = 2.0f - timeValue;
		// GLint vertexColorLocation = glGetUniformLocation(shader.Program, "rtime");
		// glUniform1f(vertexColorLocation, timeValue);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// 渲染过程结束
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}