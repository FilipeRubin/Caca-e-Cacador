#include "Hunter.hpp"
#include <cmath>

Hunter::Hunter(int x, int y)
	:Entity(x, y), m_direction(DIRECTION::UP), m_state(STATE::PATROL), m_pTarget(nullptr)
{

}

void Hunter::Walk(Entity* grid[30][30])
{
	if (m_direction == DIRECTION::UP and m_y == 29)
		m_direction = DIRECTION::DOWN;
	else if (m_direction == DIRECTION::DOWN and m_y == 0)
		m_direction = DIRECTION::UP;
	else if (m_direction == DIRECTION::LEFT and m_x == 0)
		m_direction = DIRECTION::RIGHT;
	else if (m_direction == DIRECTION::RIGHT and m_x == 29)
		m_direction = DIRECTION::LEFT;

	switch (m_direction)
	{
	case DIRECTION::UP:
	{
		grid[m_x][m_y] = nullptr;
		m_y++;
		break;
	}
	case DIRECTION::DOWN:
	{
		grid[m_x][m_y] = nullptr;
		m_y--;
		break;
	}
	case DIRECTION::LEFT:
	{
		grid[m_x][m_y] = nullptr;
		m_x--;
		break;
	}
	case DIRECTION::RIGHT:
	{
		grid[m_x][m_y] = nullptr;
		m_x++;
		break;
	}
	}

	grid[m_x][m_y] = this;
}

void Hunter::Rotate()
{
	if (m_direction == DIRECTION::UP)
		m_direction = rand() % 2 ? DIRECTION::LEFT : DIRECTION::RIGHT;
	else if (m_direction == DIRECTION::RIGHT)
		m_direction = rand() % 2 ? DIRECTION::UP : DIRECTION::DOWN;
	else if (m_direction == DIRECTION::DOWN)
		m_direction = rand() % 2 ? DIRECTION::RIGHT : DIRECTION::LEFT;
	else if(m_direction == DIRECTION::LEFT)
		m_direction = rand() % 2 ? DIRECTION::DOWN : DIRECTION::UP;
}

void Hunter::CheckSurroundings(Entity* grid[30][30])
{
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 and j == 0)
				continue;
			if (m_x + i == -1 or m_x + i == 30)
				continue;
			if (m_y + j == -1 or m_y + j == 30)
				continue;
			if (grid[m_x + i][m_y + j] != nullptr)
			{
				if (m_pTarget == ((Prey*)grid[m_x + i][m_y + j]))
				{
					m_pTarget = nullptr;
					//m_state = STATE::PATROL;
				}
				((Prey*)grid[m_x + i][m_y + j])->Die();
				grid[m_x + i][m_y + j] = nullptr;
			}
		}
	}
}

void Hunter::CheckForPrey(Entity* grid[30][30])
{
	m_state == STATE::PATROL;
	if (m_pTarget == nullptr)
	{
		int dirX = m_direction == DIRECTION::RIGHT ? 1 : m_direction == DIRECTION::LEFT ? -1 : 0;
		int dirY = m_direction == DIRECTION::UP    ? 1 : m_direction == DIRECTION::DOWN ? -1 : 0;

#ifndef FUN
		for (int i = 1; i <= 5; i++)
#else
		for (int i = 1; i <= 10; i++)
#endif
		{
			if (m_x + dirX * i == -1 or m_x + dirX * i == 30 or m_y + dirY * i == -1 or m_y + dirY * i == 30)
				break;
			if (grid[m_x + dirX * i][m_y + dirY * i] != nullptr)
			{
				m_pTarget = (Prey*)grid[m_x + dirX * i][m_y + dirY * i];
				m_state = STATE::PURSUE;
				break;
			}
		}
	}
}

void Hunter::PursuePrey(Entity* grid[30][30])
{
	if (m_pTarget == nullptr)
	{
		m_state = STATE::PATROL;
		return;
	}
	int x = 0;
	int y = 0;
	m_pTarget->GetPosition(&x, &y);

	int dirX = 0;
	int dirY = 0;

	if (x > m_x)
	{
		dirX = 1;
	}
	else if (x < m_x)
	{
		dirX = -1;
	}
	else if (y > m_y)
	{
		dirY = 1;
	}
	else if (y < m_y)
	{
		dirY = -1;
	}

	grid[m_x][m_y] = nullptr;
	m_x += dirX;
	m_y += dirY;
	grid[m_x][m_y] = this;
}

void Hunter::Render() const
{
	glBindVertexArray(s_vao);
	glUniform2f(s_uniformPos, (float)m_x, (float)m_y);
	if (m_state == STATE::PATROL)
		glUniform3f(s_uniformColor, 0.9f, 0.45f, 0.1f);
	else if (m_state == STATE::PURSUE)
		glUniform3f(s_uniformColor, 0.9f, 0.2f, 0.05f);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glUniform3f(s_uniformColor, 1.0f, 0.7f, 0.2f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void Hunter::Move(Entity* grid[30][30])
{
	if (m_state == STATE::PATROL)
	{
		(rand() % 2) ? Walk(grid) : Rotate();
		CheckSurroundings(grid);
		CheckForPrey(grid);
	}
	else if (m_state == STATE::PURSUE)
	{
		CheckSurroundings(grid);
		if (m_pTarget == nullptr)
			CheckForPrey(grid);
		PursuePrey(grid);
	}
}