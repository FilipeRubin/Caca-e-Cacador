#ifdef _WIN32
#include <Windows.h>
#include <string>
#endif
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Entity.hpp"
#include "Prey.hpp"
#include "Hunter.hpp"

float GetDeltaTime()
{
	float currentTime = (float)glfwGetTime();
	static float lastTime = currentTime;
	float r = currentTime - lastTime;
	lastTime = currentTime;

	return r;
}

void DrawGrid(GLuint& shaderProgram, GLint& uniform_pos, GLint& uniform_color, GLuint& vao)
{
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			glUniform2f(uniform_pos, (float)i, (float)j);

			if ((i + j) % 2 == 0)
			{
				glUniform3f(uniform_color, 0.45f, 0.45f, 0.45f);
			}
			else
			{
				glUniform3f(uniform_color, 0.55f, 0.55f, 0.55f);
			}

			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	}
}

int main()
{
	glfwInit();
	GLFWwindow* pWindow = glfwCreateWindow(600, 600, "Caca e cacador", nullptr, nullptr);

	glfwMakeContextCurrent(pWindow);

	gladLoadGL();

	srand((unsigned int)time(NULL));

#ifndef FUN
	int preyCount = rand() % 6 + 5;
#else
	int preyCount = 899;
#endif

	Entity* grid[30][30]{ nullptr };

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			grid[i][j] = nullptr;
		}
	}

	Hunter hunter = Hunter();
	Prey::s_pHunter = &hunter;

	{
		int posX = rand() % 30;
		int posY = rand() % 30;

		hunter.SetPosition(posX, posY);
		grid[posX][posY] = &hunter;
	}

	Prey* preys = new Prey[preyCount];

	for (int i = 0; i < preyCount; i++)
	{
		bool foundPlace = false;
		
		while (!foundPlace)
		{
			int posX = rand() % 30;
			int posY = rand() % 30;

			if (grid[posX][posY] == nullptr)
			{
				preys[i].SetPosition(posX, posY);
				grid[posX][posY] = &preys[i];
				foundPlace = true;
			}
		}
	}

	GLfloat cell[8]
	{
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	GLuint vbo = 0U;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cell), cell, GL_STATIC_DRAW);

	GLuint vao = 0U;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0U, 2, GL_FLOAT, GL_FALSE, 8, (void*)0);
	glEnableVertexAttribArray(0U);

	const GLchar* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec2 vPos;

uniform vec2 cellPos;

void main()
{
	vec4 finalPos = vec4((vPos.x / 15.0) - 1.0 + (cellPos.x / 15.0), (vPos.y / 15.0) - 1.0 + (cellPos.y / 15.0), 0.0, 1.0);
    gl_Position = finalPos;
}
)";

	const GLchar* fragmentShaderSource = R"(
#version 330 core

uniform vec3 cellColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(cellColor, 1.0);
}
)";

	GLuint shaderProgram  = glCreateProgram();
	GLuint vertexShader   = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint uniform_pos   = glGetUniformLocation(shaderProgram, "cellPos");
	GLint uniform_color = glGetUniformLocation(shaderProgram, "cellColor");

	Entity::Load(vao, uniform_pos, uniform_color);

	float stepTime = 3.0f;
	
	unsigned long long int stepCount = 0ULL;

	while (!glfwWindowShouldClose(pWindow))
	{
		stepTime -= GetDeltaTime();

		if (stepTime <= 0.0f)
		{
			hunter.Move(grid);
			for (int i = 0; i < preyCount; i++)
			{
				preys[i].Move(grid);
			}
#ifndef FUN
			stepTime = 0.2f;
#else
			stepTime = 0.0000001f;
#endif
			stepCount++;
			
			if (Prey::s_aliveCount == 0U)
				glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		DrawGrid(shaderProgram, uniform_pos, uniform_color, vao);
		for (int i = 0; i < preyCount; i++)
		{
			preys[i].Render();
		}

		hunter.Render();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	delete[preyCount] preys;
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(pWindow);
	glfwTerminate();

#ifdef _WIN32
	MessageBox(NULL, (std::wstring(L"Step count: ") + std::to_wstring(stepCount)).c_str(), L"End of application", MB_OK | MB_ICONQUESTION);
#else
	std::cout << "Step count: " << stepCount << '\n';
	std::cin.get();
#endif

	return 0;
}