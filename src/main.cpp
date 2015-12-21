#include <Puzzle_Dunjun/common.hpp>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

GLOBAL const int g_windowWidth = 854;
GLOBAL const int g_windowHeight = 400;

void glfwHints()
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_VERSION_MINOR, 1);
}

int main( int argc, char** argv)
{
	

	GLFWwindow* window;

	if (!glfwInit())
		return EXIT_FAILURE;

	glfwHints(); //calls check for openGL version (in this case 2.1)
	window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Puzzle_Dunjun", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	
	glewInit();
	
	float vertices[] = {
		+0.0f, +0.5f,  // Vertex 1
		-0.5f, -0.5f,  // Vertex 2
		+0.5f, -0.5f,  // Vertex 3
	};

	//use GPU memory via vertexbufferObject

	GLuint vbo;  // VertexBufferObject 
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //binds GL_ARRAY_BUFFER to vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
		GL_STATIC_DRAW  -- useful for drawing background (no need to update)
		GL_DYNAMIC_DRAW -- things that will be drawn more than updated
		GL_STREAM_DRAW  -- things that will be updated constantly or more than drawn
		sprites are also things (later in lesson)
	*/

	// now for the shaders :)

	const char* vertexShaderText = {
		"#version 120\n"
		"\n"
		"attribute vec2 vertPosition;"
		"void main()"
		"{"
		"   gl_Position = vec4(vertPosition, 0.0, 1.0);"
		"}"
	};

	const char* fragmentShaderText = {
		"#version 120\n"
		"\n"
		"uniform vec3 uniColor;"
		""
		"void main()"
		"{"
		"	gl_FragColor = vec4(uniColor, 1.0);"
		"}"
	};

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderText, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
	glCompileShader(fragmentShader);

	// you are allowed to make changes to your shaders BEFORE you link the two
	// shaders together

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindAttribLocation(shaderProgram, 0, "vertPosition");

	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	GLint uniColor = glGetUniformLocation(shaderProgram, "uniColor");
	


	bool running = true;
	bool fullscreen = false;
	while (running)
	{
		glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Things
		{
			float time = glfwGetTime();  //very handy function in GLFW (shortcut)
			glUniform3f(uniColor, 0.0f, 0.0f, 0.5f *(1.0f + sin(3.0f * time)));

			glEnableVertexAttribArray(0);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glDisableVertexAttribArray(0);
		}





		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwWindowShouldClose(window) || 
			glfwGetKey(window, GLFW_KEY_ESCAPE))
			running = false;

		if (glfwGetKey(window, GLFW_KEY_F11))
		{
			fullscreen = !fullscreen;

			GLFWwindow* newWindow;

			glfwHints();

			if (fullscreen)
			{
				int count; //GLFWvidmode gets an array of possible video modes (resolution is a subset)
				const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
				
				newWindow = glfwCreateWindow(modes[count - 1].width,
							modes[count - 1].height, "Puzzle_Dunjun",
							glfwGetPrimaryMonitor(), window);
				
			}
			else
			{
				newWindow = glfwCreateWindow(g_windowWidth, g_windowHeight,
							"Puzzle_Dunjun", nullptr, window);
			}

			glfwDestroyWindow(window);
			window = newWindow;
			glfwMakeContextCurrent(window);
		}//endif Key_F11

	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}