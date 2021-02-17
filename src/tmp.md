### 序

搞一大堆缓冲还是为了更好的传输数据：开显存，将顶点数据传到显存中，链接着色器程序，GPU 着色


### Vertex Buffer Object (VBO)

一次性将大量顶点交付给渲染管线


### Vertex Array Object (VAO)

对顶点的结构的抽象描述，标识谁是位置谁是颜色


### Index (Element) Buffer Object (EBO)

实现顶点复用

---

传入 VBO，使用 VAO


``` cpp
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
```