#pragma once

#include "Entity.hpp"
#include "Prey.hpp"

class Prey;

class Hunter : public Entity
{
private:
	enum class DIRECTION
	{
		UP, DOWN, LEFT, RIGHT
	};

	enum class STATE
	{
		PATROL, PURSUE
	};
private:
	DIRECTION m_direction;
	STATE     m_state;
	Prey*     m_pTarget;
private:
	void Walk(Entity* grid[30][30]);
	void Rotate();
	void CheckSurroundings(Entity* grid[30][30]);
	void CheckForPrey(Entity* grid[30][30]);
	void PursuePrey(Entity* grid[30][30]);
public:
	Hunter(int x = 0, int y = 0);
	void Render() const override;
	void Move(Entity* grid[30][30]) override;
};