## OpenGL: 着色器

典型着色器结构

``` cpp
#version version_number

in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

### 数据类型

基础数据类型：`int`、`float`、`double`、`uint`和`bool`。GLSL也有两种容器类型，它们会在这个教程中使用很多，分别是向量(Vector)和矩阵(Matrix)，其中矩阵我们会在之后的教程里再讨论。

#### 向量

GLSL中的向量是一个可以包含有1、2、3或者4个分量的容器

可以分别使用`.x`、`.y`、`.z`和`.w`来获取它们的第1、2、3、4个分量

允许对颜色使用`rgba`，或是对纹理坐标使用`stpq`访问相同的分量

允许一些有趣而灵活的分量选择方式，叫做**重组**

``` cpp
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

### 输入输出

如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入

当**类型和名字都一样**的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了

#### 顶点着色器

顶点着色器的输入特殊在，它从顶点数据中直接接收输入

``` cpp
#version 330 core
layout (location = 0) in vec3 position; // position变量的属性位置值为0

out vec4 vertexColor; // 为片段着色器指定一个颜色输出

void main()
{
    gl_Position = vec4(position, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
    vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f); // 把输出变量设置为暗红色
}
```

#### 片段着色器

片段着色器，它需要一个`vec4`颜色输出变量，因为片段着色器需要生成一个最终输出的颜色

``` cpp
#version 330 core
in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

out vec4 color; // 片段着色器输出的变量名可以任意命名，类型必须是vec4

void main()
{
    color = vertexColor;
}
```

### Uniform

Uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式

uniform变量必须在每个着色器程序对象中都是独一无二的，而且它可以被着色器程序的任意着色器在任意阶段访问

``` cpp
#version 330 core
out vec4 color;

uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

void main()
{
    color = ourColor;
}  
```

``` cpp
GLfloat timeValue = glfwGetTime();
GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```

用glGetUniformLocation查询uniform `ourColor`的位置值

通过glUniform4f函数设置uniform值

注意：查询uniform地址不要求你之前使用过着色器程序，但是更新一个uniform之前你**必须**先使用程序

