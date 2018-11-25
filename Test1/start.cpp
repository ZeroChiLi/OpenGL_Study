#include <glad/glad.h>		// OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

//每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		// 主版本号：3  OpenGL 3.3  GLFW会在创建OpenGL上下文时做出适当的调整
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		// 次版本号：3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// 核心模式(Core-profile)。只能使用OpenGL功能的一个子集（没有我们已不再需要的向后兼容特性）
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// Mac OS X系统 初始化代码中这些配置

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	// glfwGetProcAddress 加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 渲染循环(Render Loop)
	// glfwWindowShouldClose: GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// 状态设置函数
		glClear(GL_COLOR_BUFFER_BIT);			// 状态使用的函数  缓冲位,指定要清空的缓冲

		// glfw: 会交换颜色缓冲
		// 前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制
		glfwSwapBuffers(window);

		//函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}

	// glfw: 释放/删除之前的分配的所有资源
	glfwTerminate();

	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	// 如果没有按下，glfwGetKey将会返回GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: 每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 对于视网膜(Retina)显示屏，width和height都会明显比原输入值更高一点。
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}