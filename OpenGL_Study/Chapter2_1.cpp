#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// VAO �����������Vertex Array Object��
// VBO ���㻺�����Vertex Buffer Object��
// IBO �����������Element Buffer Object��EBO��Index Buffer Object��

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
		//GLSL(OpenGL Shading Language)��д������ɫ��
		//version 330 core��OpenGL 3.3�Լ��͸��߰汾��
		//vec3 ���͵� aPos������λ��ֵ(Location) = 0 ����˼���ڵ�0��������in�ؼ��� �������е����붥������(Input Vertex Attribute)
		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		//out�ؼ��� �����������
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0";

#pragma region ��ʼ������������

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

		// ���� ����shader����
		// vertex shader��glad.h��������ɫ������
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// ����1����ɫ������2��Դ���ַ���������3����ɫ��Դ��
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		// ������ɫ��
		glCompileShader(vertexShader);
		// ���������
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
		// ���������
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// link shaders ������ɫ��
		// ��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾
		int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// ������Ӵ���
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// �������ɾ����
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ���ö������ݣ��������ݣ���������
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, // left  
			 0.5f, -0.5f, 0.0f, // right 
			 0.0f,  0.5f, 0.0f  // top   
		};
		//VAO ����������� ������������
		// glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ��á�
		// ͨ��glVertexAttribPointer���õĶ����������á�
		// ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		// �󶨶������鵽���㻺���У����ö������ԡ�
		glBindVertexArray(VAO);

		//���ƶ������鵽�����й�OpenGLʹ��
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// ����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
		//1��layout(location = 0)������position�������Ե�λ��ֵ�����ݴ������λ��
		//2���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3
		//3�����ݵ�����
		//4���Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������ - 1����1֮��
		//5������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮��
		//6��λ�������ڻ�������ʼλ�õ�ƫ����(Offset)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		// �ÿ� GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		// ��� VAO
		glBindVertexArray(0);


		// uncomment this call to draw in wireframe polygons.
		// �û���ģʽ����
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render loop
		while (!glfwWindowShouldClose(window))
		{
			// input
			processInput(window);

			// render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// ��glUseProgram��������֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������Ҳ����֮ǰд����ɫ��)��
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

			//1������ͼԪ���� 2�������������ʼ���� 3�����ƶ��ٸ�����
			glDrawArrays(GL_TRIANGLES, 0, 3);
			// glBindVertexArray(0); // no need to unbind it every time 

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// �ͷ���Դ
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		return 0;
	}
};
