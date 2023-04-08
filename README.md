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

