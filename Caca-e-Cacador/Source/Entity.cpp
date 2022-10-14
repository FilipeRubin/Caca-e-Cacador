#include "Entity.hpp"

GLuint Entity::s_vao = 0U;
GLint  Entity::s_uniformPos = 0;
GLint  Entity::s_uniformColor = 0;

void Entity::Load(GLuint vao, GLint uniformPos, GLint uniformColor)
{
	Entity::s_vao = vao;
	Entity::s_uniformPos = uniformPos;
	Entity::s_uniformColor = uniformColor;
}

Entity::Entity(int x, int y) : m_x(x), m_y(y)
{

}

void Entity::GetPosition(int* x, int* y) const
{
	*x = m_x;
	*y = m_y;
}

void Entity::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}