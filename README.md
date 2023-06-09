# Learning OpenGL 备忘

## 创建窗口

### GLFW

GLFW 用于创建 OpenGL 上下文和窗口，并处理输入。

### 创建工程

在 VS 中创建 sln 和 project

- 在 `Project - Properties - VC++ Derectories`  中添加 Include 和 Lib 目录
- 在 `Project - Properties - Linker - Input - Additional Dependencies` 中添加 glfw3.lib
- 在 Windows 下，VS 的 SDK 自带 opengl32.lib，只需要将其添加至 Linker

### 配置 GLAD

OpenGL 只是一个标准，具体实现由驱动开发商针对显卡实现，OpenGL 函数位置因为驱动的不同实现而有所不同。

手动获取 OpenGL 的函数地址非常痛苦，GLAD 用于运行时批量获取 OpenGL 的函数地址并保存到函数指针。

添加 GLAD：

- 将头文件放入 Include
- 将 glad.c 作为源文件加入工程

## 你好，窗口

创建窗口并渲染的基本流程：

- 初始化 GLFW
- 创建窗口并将窗口绑定到当前线程的上下文
- 通过 GLAD 获取 gl 函数地址
- 设定 viewport 的 size
- 循环处理输入并渲染
- 释放资源

## 你好，三角形

要记住的 3 个术语：

- 顶点数组对象：Vertex Array Object，VAO
- 顶点缓冲对象：Vertex Buffer Object，VBO
- 元素缓冲对象：Element Buffer Object，EBO（索引缓冲对象：Index Buffer Object，IBO）

OpenGL 的大部分工作都是关于把 3D 坐标转变为 2D 像素：

- 将 3D 坐标转换为 2D 坐标
- 将 2D 坐标转变为实际有颜色的像素（坐标精确表示一个点在 2D 空间中的位置，像素是这个点的近似值，受限于分辨率）

图形渲染管线可以被划分为几个阶段，所有阶段都是高度专门化（有特定的程序）并且容易并行执行，显卡有成千上万的小处理核心来执行每个阶段的小程序，这些小程序被称为着色器（Shader）。

部分 Shader 允许开发者自己编写，以替代默认的 Shader，OpenGL 的 Shader 使用 GLSL（OpenGL Shading Language）来编写：

- 顶点着色器：Vertex Shader
- 几何着色器：Geometry Shader
- 片段着色器：Fragment Shader

图形渲染管线各个阶段：

- 顶点输入：以数组的形式传入 3 个 3D 坐标，用来表示一个三角形，这个数组称为顶点数据（Vertex Data），顶点数据是一系列顶点的集合。顶点是一个 3D 坐标的数据；而顶点数据用顶点属性（Vertex Attribute）表示，可以包含任何数据，比如，每个顶点包含 3D 坐标和颜色值
- 顶点着色：将 3D 坐标转换成另一种 3D 坐标，并对顶点属性做一些基本操作
- 图元装配（Primitive Assembly）：顶点数据构成的可能是一系列点，线或者三角形，需要我们提示 OpenGL，这些提示叫做图元（Primitive）。这一阶段会将点装配成指定图元的形状，比如三角形
- 几何着色：可以通过产生新顶点构造处新图元来生成其他形状
- 光栅化：将图元映射到屏幕上对应的像素，生成片段（Fragment，渲染一个像素所需的全部数据），并将超出视野之外的像素裁切掉
- 片段着色：计算一个像素的最终颜色，这是所有 OpenGL 高级效果产生的地方。通常，片段着色器包含 3D 场景的数据（比如光照、阴影等等），这些数据可以被用来计算像素的颜色。
- Alpha 测试和混合：测试深度和透明度，最终确定像素的颜色

### 顶点输入

顶点输入的基本步骤：

- 定义三角形在正则坐标下的坐标值（顶点数据）
- 创建 VBO 对象以及对应的 buffer（位于 GPU 内存中），用于管理顶点数据（存储、释放）
  - 创建 buffer
  - 绑定 buffer 并指定类型
  - 向 buffer 填入数据
- 现在，顶点数据储存在 GPU 内存中，用 VBO 这个对象进行管理

### 顶点着色器

```glsl
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

- `in`：表示输入
- `layout (location = 0)`：表示变量 aPos 的值来自于 VBO 的属性 0，关于后续关于 VBO 的属性 0，后续“链接顶点属性”会展开。
- `vec3`：声明变量类型为 3 维向量
- `gl_Position`：顶点着色器的输出

在实际的程序中，输入数据通常不是正则坐标，需要在这里进行转换。

### 编译着色器

- 字符串硬编码 shader 源码
- 创建 shader 及其对象
- 编译 shader
- 获取 shader 编译失败后的错误信息（可省略）

### 片段着色器

```glsl
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
```

- `out`：表示输出

类似顶点着色器步骤编译片段着色器。

### 链接并使用 shader program

要将 shader 链接至 shader program，然后在程序运行的时候使用 program，这样，被使用的 program 才会在渲染的过程中被调用。

当链接 shader 至一个 program 的时候，它会把每个 shader 的输出链接到下个 shader 的输入。当输出和输入不匹配的时候，你会得到一个链接错误。

链接并使用 shader program 的步骤：

- 创建 shader program
- attach 并 link 2 个 shader
- 可省略，用于获取 shader program link 失败后的错误信息
- 使用 program，后续每个 Shader 调用和渲染调用都会用到这个 program
- 删除 shader

### 链接顶点属性

有了顶点数据、顶点着色器和片段着色器，GPU 还需要知道如何解释内存中的顶点数据，以及如何将顶点数据链接到 shader 上：

```glsl
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
```

- @param0：标识当前 vertex 的属性，相当于将当前 vertex 数据传到 vertex shader 中的 location = 0 指定的变量 aPos
- @param1：指定一个 vertex 的元素数量为 3
- @param2：指定 vertex 的类型为 float
- @param3：表明 vertex 数据不需要被标准化到 [0, 1]
- @param4：步长，表明每组 vertex 属性的元素个数，比如 {x, y, z, r, g, b, a} 的时候为 7
- @param5：偏移，表明 vertex 从哪个位置开始取，比如 {x, y, z, r, g, b, a} 如果想去颜色就设为 3

顶点属性默认关闭，需要将其启动：

```glsl
glEnableVertexAttribArray(0);
```

### 顶点数组对象（VAO）

每个物体在调用渲染操作之前，都需要如下操作：

- 绑定 VBO：glBindBuffer
- 将顶点数据传入 VBO：glBufferData
- 链接顶点属性：glVertexAttribPointer
- 启动顶点属性：glEnableVertexAttribArray

当物体数量很多，比如有很多个三角形，那么 VBO 数量就会很多，每次画三角形之前，都需要重新设置 VBO，这将会非常痛苦。于是，引入 VAO，VAO 会存储多个 VBO，后续可以通过索引来访问不同的 VBO。下图中 VAO2 将 VBO2 中的 position 信息放到索引 0，将 VBO2 的 color 信息放到索引 1：

![img](README.assets/vertex_array_objects.png)

VAO 创建后，遇到以下调用就会自动将信息记录下来：

- glEnableVertexAttribArray/glDisableVertexAttribArray
- glVertexAttribPointer

### 索引缓冲对象（IBO/EBO）

绘制多个三角形的时候，有些顶点会被重复使用，为了节省空间，引入 IBO/EBO，通过 index 来访问顶点。

另外，IBO/EBO 也会存储在 VAO 中：

![img](README.assets/vertex_array_objects_ebo.png)

## Shader

### GLSL

Shader 的典型结构：

- in：输入变量，也叫做顶点属性
- out
- uniform
- main

### 数据类型

- vec<n>: float
- bvec<n>: bool
- ivec<n>: int
- uvec<n>: uint
- dvec<n>: double

获取 vec4 的分量的方式：

- `.x` `.y` `.z` `.w`
- 颜色：`rgba`
- 纹理坐标：`stpq`

向量的重组：

```glsl
vec2 v2;
vec4 v4 = v2.xyxx;
vec3 v3 = v4.zyw;
vec4 v4_ = v2.xxxx + v3.yxzy;

vec2 foo1 = vec2(0.5, 0.7);
vec4 bar1 = vec4(foo1, 0.0, 0.0);
vec4 bar2 = vec4(bar1.xyz, 1.0);
```

### 输入和输出

shader 使用 `in` 和 `out` 设定输入和输出，只要一个输出变量与下一个 shader 阶段的输入匹配，就能往下传递。但在 vertex shader 和 fragment shader 会有点特殊。

vertex shader 应该就收一种特殊形式的输入，否则效率会很低。 vertex shader 的输入的特殊在于：它接收顶点数据作为输入。为了定义顶点数据如何管理，我们使用 `layout(location = xxx)` 来指定输入变量，这样我们在 VBO 上的顶点属性就可以映射到 shader 中。

> 也可以使用 `glGetAttributeLocation` 来查询属性位置值，比如：
>
> ```glsl
> #version 150 core
> in vec2 position;
> void main()
> {
>     gl_Position = vec4(position, 0.0, 1.0);
> }
> ```
>
> ```c
> unsigned int VBO;
> glGenBuffers(1, &VBO);
> glBindBuffer(GL_ARRAY_BUFFER, VBO);
> glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
> 
> // 获取 shader program 中的 position 属性的索引
> GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
> glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
> glEnableVertexAttribArray(posAttrib);
> ```

fragment shader 的特殊在于：它的输出是一个 `vec4` 来表示颜色，如果没有定义颜色，OpenGL 会将物体渲染成黑色。

当我们打算从一个 shader 向另一个 shader 发送数据时，我们必须在发送方 shader 声明一个输出，在接收方 shader 声明一个类似的输入，当类型和名字都一样的时候，OpenGL 会把两个变量链接在一起。

### Uniform

uniform 是一种从 CPU 向 GPU 中 shader 发送数据的方式。uniform 与顶点属性的区别：

- uniform 是全局的，uniform 变量在每个 shader program 中都是独一无二，它可以被 shader program 的任意 shader 在任意阶段访问
- uniform 在设置后一直保存着数据，直到被更新

```glsl
#version 330 core
out vec4 FragColor;
uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量
void main()
{
    FragColor = ourColor;
}
```

## 添加顶点属性：color

- 顶点数据添加 color 数据
- vertex shader 添加 color 输入以接收新的顶点属性
- vertex shader 添加 color 输出以向 fragment shader 传入 color 数据
- fragment shader 添加 color 输入
- 链接 color 顶点属性，将 VBO 中的 color 数据 map 到 vertex shader 中

顶点赋予了不同颜色以后，三角形的中间部分会根据重心坐标（barycentric），对颜色进行插值。

## 创建 Shader 类

- 每次都要 Shader 的创建写大量重复代码，更好的做法是创建 Shader 类来减少重复。
- shader 源码硬编码到 cpp 的字符串中，更好的做法是放到单独的文件中，在 cpp 中读取文件





