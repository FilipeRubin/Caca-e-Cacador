#include "Prey.hpp"
#include <cmath>
#include <iostream>

unsigned int Prey::s_aliveCount =      0U;
Hunter*      Prey::s_pHunter    = nullptr;

Prey::Prey(int x, int y)
	:Entity(x, y), m_alive(true)
{
	s_aliveCount++;
}

void Prey::Render() const
{
	if (!m_alive)
		return;
	glBindVertexArray(s_vao);
	glUniform2f(s_uniformPos, (float)m_x, (float)m_y);
	glUniform3f(s_uniformColor, 0.1f, 0.45f, 0.9f);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glUniform3f(s_uniformColor, 0.2f, 0.7f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void Prey::Move(Entity* grid[30][30])
{
	if (!m_alive)
		return;

	bool foundPlace = false;

	while (!foundPlace)
	{
		int dirX = 0, dirY = 0;

		while (dirX == 0 and dirY == 0)
		{
#ifndef FUN
			int chance = rand() % 10;
#else
			int chance = rand() % 2;
#endif
			if (chance or !s_pHunter) // 90% chance of going to a random place
			{
				//std::cout << "I went to a random place\n";
				dirX = rand() % 3 - 1;
				dirY = rand() % 3 - 1;
			}
			else // 10% chance of going away from the hunter
			{
				//std::cout << "I'm running from the hunter\n";
				int x = 0;
				int y = 0;
				s_pHunter->GetPosition(&x, &y);
				if (x > m_x)
					dirX = -1;
				else if (x < m_x)
					dirX = 1;
				if (y > m_y)
					dirY = -1;
				else if (y < m_y)
					dirY = 1;
			}

			if (m_x + dirX > 29 or m_x + dirX < 0)
			{
				dirX = 0;
			}
			if (m_y + dirY > 29 or m_y + dirY < 0)
			{
				dirY = 0;
			}
		}

		if (grid[m_x + dirX][m_y + dirY] == nullptr)
		{
			foundPlace = true;
			grid[m_x + dirX][m_y + dirY] = this;
			grid[m_x][m_y] = nullptr;
			m_x += dirX;
			m_y += dirY;
		}
		else
		{
			foundPlace = true;
		}
	}
}

void Prey::Die()
{
	m_alive = false;
	s_aliveCount--;
}