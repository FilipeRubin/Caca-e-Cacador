#pragma once

//#define FUN // Uncomment this if you want to have fun...

#include <glad/glad.h>

class Entity
{
protected:
	int m_x, m_y;
public:
	static GLuint s_vao;
	static GLint  s_uniformPos;
	static GLint  s_uniformColor;
public:
	static void Load(GLuint vao, GLint uniformPos, GLint uniformColor);
	Entity(int x = 0, int y = 0);
	void GetPosition(int* x, int* y) const;
	void SetPosition(int x, int y);
	virtual void Render() const = 0;
	virtual void Move(Entity* grid[30][30]) = 0;
};