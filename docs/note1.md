## OpenGL: 图形渲染管线 (Hello Triangle)

*注：本文为 LearnOpenGL-CN 的摘抄*

Graphics Pipeline，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程

图形渲染管线接受一组3D坐标，然后把它们转变为你屏幕上的有色2D像素输出

在GPU上为每一个（渲染管线）阶段运行各自的小程序，从而在图形渲染管线中快速处理你的数据。这些小程序叫做着色器

Vertex/Geometry/Fragment Shader 是可编程的

### 图形渲染管线的组成

**顶点着色器**(Vertex Shader)，它把一个单独的顶点作为输入。顶点着色器主要的目的是把3D坐标转为另一种3D坐标

**图元装配**(Primitive Assembly)阶段将顶点着色器输出的所有顶点作为输入，并所有的点装配成指定图元的形状

**几何着色器**把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状

**光栅化阶段**(Rasterization Stage)，这里它会把图元映射为最终屏幕上相应的像素，生成片段(Fragment)

**片段着色器**的主要目的是计算一个像素的最终颜色

**Alpha测试和混合(Blending)阶段**。这个阶段检测片段的对应的深度（和模板(Stencil)）值，也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)

### 顶点缓冲对象 VBO

定义顶点数据以后，我们会把它作为输入发送给图形渲染管线的第一个处理阶段：顶点着色器。

它会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。

我们通过顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存(通常被称为显存)中储存大量顶点。

使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。

生成 VBO 对象，绑定缓冲，复制数据到缓冲

使用glGenBuffers函数和一个缓冲ID生成一个VBO对象

使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上（顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER）

使用glBufferData函数，它会把顶点数据复制到缓冲的内存中

### 着色器 Shader

#### 顶点着色器

``` cpp
#version 330 core

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
```

使用`in`关键字，在顶点着色器中声明所有的输入顶点属性（现在我们只关心位置(Position)数据，所以我们只需要一个顶点属性）

为了设置顶点着色器的输出，我们必须把位置数据赋值给预定义的gl_Position变量

#### 片段着色器

``` cpp
#version 330 core

out vec4 color;

void main()
{
    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
```

片段着色器只需要一个输出变量，这个变量是一个4分量向量，它表示的是最终的输出颜色

#### 编译着色器

在运行时动态编译着色器的源码

用glCreateShader创建这个着色器对象，把这个着色器源码附加到着色器对象，然后编译它

#### 着色器程序对象

着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。如果要使用刚才编译的着色器我们必须把它们链接为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序

当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入

当输出和输入不匹配的时候，你会得到一个连接错误

``` cpp
GLuint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);
```

### 链接顶点属性

须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性

使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）

使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性

---

至此，绘制一个物体的代码

``` cpp
// 0. 复制顶点数组到缓冲中供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 1. 设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// 2. 当我们渲染一个物体时要使用着色器程序
glUseProgram(shaderProgram);
// 3. 绘制物体
someOpenGLFunctionThatDrawsOurTriangle();
```

有没有一些方法可以使我们把所有这些状态配置储存在一个对象中，并且可以通过绑定这个对象来恢复状态呢？

### 顶点数组对象 VAO

VAO 使在不同顶点数据和属性配置之间切换变得非常简单

VAO 可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中

这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了

``` cpp
// ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
// 1. 绑定VAO
glBindVertexArray(VAO);
// 2. 把顶点数组复制到缓冲中供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. 设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
//4. 解绑VAO
glBindVertexArray(0);

[...]

// ..:: 绘制代（游戏循环中） :: ..
// 5. 绘制物体
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
someOpenGLFunctionThatDrawsOurTriangle();
glBindVertexArray(0);
```

对于一个储存了我们顶点属性配置和应使用的VBO的顶点数组对象，生成/配置所有的VAO（和必须的VBO及属性指针)，然后储存它们供后面使用

当我们打算绘制物体的时候就拿出相应的VAO，绑定它，绘制完物体后，再解绑VAO。

### 索引缓冲对象

为所有顶点提供坐标可能会有重复而造成浪费

``` cpp
GLfloat vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

GLuint indices[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
```

EBO也是一个缓冲，它专门储存索引，OpenGL通过这些顶点的索引来决定该绘制哪个顶点

绘制时，用glDrawElements来替换glDrawArrays函数，来指明我们从索引缓冲渲染

顶点数组对象同样可以保存索引缓冲对象的绑定状态，绑定VAO的同时也会自动绑定EBO

``` cpp
// ..:: 初始化代码 :: ..
// 1. 绑定顶点数组对象
glBindVertexArray(VAO);
// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 3. 设定顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// 4. 解绑VAO（不是EBO！）
glBindVertexArray(0);

[...]

// ..:: 绘制代码（游戏循环中） :: ..

glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);
```

### 综合

``` cpp
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
const GLchar *fragmentShaderSource = "#version 330 core\n"
									 "out vec4 color;\n"
									 "void main()\n"
									 "{\n"
									 "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									 "}\n\0";

// settings
const unsigned int SCR_WIDTH = 512;
const unsigned int SCR_HEIGHT = 512;

void render();
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

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

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
```

