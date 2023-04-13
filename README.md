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
- `layout (location = 0)`：设定了输入变量的位置值？
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



