#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

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

	GLchar *shader_src = new GLchar[4096];
	fill(shader_src, shader_src + 4096, 0);

	ReadShaderSource("vertex_shader.shader", shader_src);

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &shader_src, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	fill(shader_src, shader_src + 4096, 0);
	ReadShaderSource("fragment_shader.shader", shader_src);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shader_src, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	delete[] shader_src;

	// 准备顶点数据

	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,	// 右上角
		0.5f, -0.5f, 0.0f,	// 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f	// 左上角
	};

	GLuint indices[] = {
		0, 1, 3, // 第一个三角形
		1, 2, 3	 // 第二个三角形
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// 解绑VAO（不是EBO！）
	glBindVertexArray(0);

	// 程序循环
	while (!glfwWindowShouldClose(window))
	{
		// 渲染过程开始
		glUseProgram(shaderProgram);

		GLfloat timeValue = fmod(glfwGetTime(), 2.0f);
		if (timeValue > 1.0f)
			timeValue = 2.0f - timeValue;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "rtime");
		glUseProgram(shaderProgram);
		glUniform1f(vertexColorLocation, timeValue);

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