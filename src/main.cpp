#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

// Shaders
const GLchar *vertexShaderSource = "#version 330 core\n"
								   "layout (location = 0) in vec3 position;\n"
								   "void main()\n"
								   "{\n"
								   "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
								   "}\0";
const GLchar *fragmentShaderSource1 = "#version 330 core\n"
									  "out vec4 color;\n"
									  "void main()\n"
									  "{\n"
									  "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									  "}\n\0";
const GLchar *fragmentShaderSource2 = "#version 330 core\n"
									  "out vec4 color;\n"
									  "void main()\n"
									  "{\n"
									  "color = vec4(0.5f, 1.0f, 0.2f, 1.0f);\n"
									  "}\n\0";

// settings
const unsigned int SCR_WIDTH = 512;
const unsigned int SCR_HEIGHT = 512;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

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
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader[2];
	GLuint shaderProgram[2];

	shaderProgram[0] = glCreateProgram();
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader[0]);
	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);

	shaderProgram[1] = glCreateProgram();
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader[1]);
	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);

	// 准备顶点数据

	GLfloat vertices1[] = {
		-0.5, -0.5, 0,
		-0.5, 0.5, 0,
		0, 0, 0};

	GLfloat vertices2[] = {
		0, 0, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0};

	GLuint indices[] = {
		0, 1, 2};

	GLuint EBO;
	glGenBuffers(1, &EBO);

	// 处理第一个

	GLuint VBO1;
	glGenBuffers(1, &VBO1);
	GLuint VAO1;
	glGenVertexArrays(1, &VAO1);

	// 绑定顶点数组对象
	glBindVertexArray(VAO1);
	// 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	// 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 设定并启用顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// 解绑VAO（不是EBO！）
	glBindVertexArray(0);

	// 处理第二个

	GLuint VBO2;
	glGenBuffers(1, &VBO2);
	GLuint VAO2;
	glGenVertexArrays(1, &VAO2);

	// 绑定顶点数组对象
	glBindVertexArray(VAO2);
	// 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	// 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 设定并启用顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// 解绑VAO（不是EBO！）
	glBindVertexArray(0);

	// 程序循环
	while (!glfwWindowShouldClose(window))
	{
		// 渲染过程开始
		// 绘制第一个
		glUseProgram(shaderProgram[0]);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// 绘制第二个
		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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