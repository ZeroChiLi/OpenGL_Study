#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// VAO 顶点数组对象：Vertex Array Object，
// VBO 顶点缓冲对象：Vertex Buffer Object，
// IBO 索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，

class Chapter2_1
{
public:
	static const unsigned int SCR_WIDTH = 800;
	static const unsigned int SCR_HEIGHT = 600;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	static void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
	
	static int Start()
	{
		//GLSL(OpenGL Shading Language)编写顶点着色器
		//version 330 core：OpenGL 3.3以及和更高版本中
		//vec3 类型的 aPos变量。位置值(Location) = 0 ，意思放在第0个索引。in关键字 声明所有的输入顶点属性(Input Vertex Attribute)
		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		//out关键字 声明输出变量
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0";

#pragma region 初始化，创建窗口

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, Chapter2_1::framebuffer_size_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

#pragma endregion

		// 创建 编译shader程序
		// vertex shader，glad.h：创建着色器对象
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// 参数1：着色器对象，2：源码字符串数量，3：着色器源码
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		// 编译着色器
		glCompileShader(vertexShader);
		// 检查编译错误
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// 检查编译错误
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// link shaders 链接着色器
		// 着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本
		int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// 检查链接错误
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// 链接完就删掉。
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// 配置顶点数据，缓存数据，顶点属性
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, // left  
			 0.5f, -0.5f, 0.0f, // right 
			 0.0f,  0.5f, 0.0f  // top   
		};
		//VAO 顶点数组对象 包含以下内容
		// glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
		// 通过glVertexAttribPointer设置的顶点属性配置。
		// 通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		// 绑定顶点数组到顶点缓存中，配置顶点属性。
		glBindVertexArray(VAO);

		//复制顶点数组到缓冲中供OpenGL使用
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
		//1，layout(location = 0)定义了position顶点属性的位置值，数据传到这个位置
		//2，顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3
		//3，数据的类型
		//4，是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是 - 1）到1之间
		//5，步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后
		//6，位置数据在缓冲中起始位置的偏移量(Offset)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		// 置空 GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		// 解绑 VAO
		glBindVertexArray(0);


		// uncomment this call to draw in wireframe polygons.
		// 用画线模式绘制
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render loop
		while (!glfwWindowShouldClose(window))
		{
			// input
			processInput(window);

			// render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// 在glUseProgram函数调用之后，每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

			//1，绘制图元类型 2，顶点数组的起始索引 3，绘制多少个顶点
			glDrawArrays(GL_TRIANGLES, 0, 3);
			// glBindVertexArray(0); // no need to unbind it every time 

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// 释放资源
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		return 0;
	}
};
