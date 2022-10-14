#pragma once

#include <glad/glad.h>
#include "Entity.hpp"
#include "Hunter.hpp"

class Hunter;

class Prey : public Entity
{
private:
	bool m_alive;
public:
	static unsigned int s_aliveCount;
	static Hunter* s_pHunter;
public:
	Prey(int x = 0, int y = 0);
	void Render() const override;
	void Move(Entity* grid[30][30]) override;
	void Die();
};