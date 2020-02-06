#include "ProjectilePool.h"

////////////////////////////////////////////////////////////
void ProjectilePool::create(sf::Texture const & texture, float x, float y, float rotation)
{
	// If no projectiles available, simply re-use the next in sequence.
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}
	
	m_projectiles.at(m_nextAvailable).init(texture, x, y, rotation);
}

////////////////////////////////////////////////////////////
bool ProjectilePool::update(double dt, std::vector<sf::Sprite> & wallSprites, std::pair<sf::Sprite, sf::Sprite> t_aiTankSprites)
{	
	// The number of active projectiles.
	int activeCount = 0;
	// Assume the pool is not full initially.
	m_poolFull = false;

	bool targetHit = false;

	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		std::pair<bool, bool> result = m_projectiles.at(i).update(dt, wallSprites, t_aiTankSprites);

		if(result.first)
		{
			// If this projectile has expired, make it the next available.
			m_nextAvailable = i;
		}
		else
		{
			// So we know how many projectiles are active.
			activeCount++;
		}

		if (result.second)
		{
			targetHit = true;
		}
	}
	// If no projectiles available, set a flag.
	if (s_POOL_SIZE == activeCount)
	{		
		m_poolFull = true;
	}

	return targetHit;
}

////////////////////////////////////////////////////////////
void ProjectilePool::render(sf::RenderWindow & window)
{
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		// If projectile is active...
		if (m_projectiles.at(i).inUse())
		{
			window.draw(m_projectiles.at(i).m_projectile);
		}
	}
}