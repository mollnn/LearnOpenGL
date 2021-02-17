#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Shaders
const GLchar *vertexShaderSource = "#version 330 core\n"
								   "layout (location = 0) in vec3 position;\n"
								   "void main()\n"
								   "{\n"
								   "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
								   "}\0";
const GLchar *fragmentShaderSource = "#version 330 core\n"
									 "out vec4 color;\n"
									 "void main()\n"
									 "{\n"
									 "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									 "}\n\0";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void render();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// render process here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		// glfw: swap buffers and poll IO events (keyspressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFWresources.
	glfwTerminate();
	return 0;
}

void render()
{
	// **************************************
	// Shader 处理
	// **************************************
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// 把它们链接为一个着色器程序对象
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检测链接着色器程序是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// **********************************************
	// 渲染对象
	GLfloat vertices[] = {
		-0.5, 0, 0,
		-0.25, 0.5, 0,
		0, 0, 0,
		0.25, 0.5, 0,
		0.5, 0, 0};
	GLuint indices[] = {
		// Note that we start from 0!
		0, 1, 2,
		2, 3, 4};

	// **********************************************
	// 顶点数组与缓冲对象的绑定
	GLuint VBO, VAO, EBO; // 缓冲对象的编号
	// 生成顶点数组与缓冲对象
	glGenVertexArrays(1, &VAO); // 顶点数组
	glGenBuffers(1, &VBO);		// 顶点缓冲
	glGenBuffers(1, &EBO);		// 索引缓冲

	glBindVertexArray(VAO);						// 绑定顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, VBO);			// 绑定顶点缓冲
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定索引缓冲

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		 // 复制顶点数组到一个顶点缓冲中
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 复制我们的索引数组到一个索引缓冲中

	// 设定顶点属性指针（下面的 0 是位置值）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0); // 该如何解析顶点数据
	glEnableVertexAttribArray(0);													   // 激活

	glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑 VBO
	glBindVertexArray(0);			  // 解绑 VAO

	// --分割线-- 以下代码必须放在消息循环中

	glUseProgram(shaderProgram);						 // 激活着色器程序
	glBindVertexArray(VAO);								 // 绑定 VA0
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制
	glBindVertexArray(0);								 // 解绑 VAO
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}