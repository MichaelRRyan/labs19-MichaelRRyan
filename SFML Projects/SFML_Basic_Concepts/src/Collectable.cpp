#include "Collectable.h"

Collectable::Collectable() :
	m_active{ false }
{
	m_sprite.setTexture(AssetManager::getTexture("spriteSheet"));
	m_sprite.setTextureRect({ 0, 80, 43, 57 });
}

////////////////////////////////////////////////////////////
void Collectable::reset(std::vector<sf::Sprite> const& t_walls, std::vector<Collectable> const & t_collectables)
{
	bool colliding = false;

	// Set the position of the cargo
	do
	{
		// Set the position of the cargo sprite
		m_sprite.setPosition(static_cast<float>(rand() % (ScreenSize::s_width - 200) + 100), static_cast<float>(rand() % (ScreenSize::s_height - 200) + 100));

		colliding = false; // No longer colliding until proven otherwise

		// Check if colliding with a wall
		for (sf::Sprite const& wall : t_walls)
		{
			if (CollisionDetector::collision(m_sprite, wall))
			{
				colliding = true;
				break;
			}
		}

		// Check if colliding with another collectable if not already colliding
		if (!colliding)
		{
			// Loop through collectables
			for (Collectable const& collectable : t_collectables)
			{
				// Only collide with active collectables (have already been spawned)
				if (collectable.isActive())
				{
					// Don't check collisions with ourself
					if (this != &collectable)
					{
						// Check the collision
						if (CollisionDetector::collision(m_sprite, collectable.getSprite()))
						{
							colliding = true; // Is now colliding
							break; // No need to check further collectables
						}
					}
				}
			}
		}
	}
	while (colliding);

	// The cargo is now active
	m_active = true;
}

////////////////////////////////////////////////////////////
void Collectable::draw(sf::RenderWindow& t_window) const
{
	if (m_active)
	{
		t_window.draw(m_sprite);
	}
}

////////////////////////////////////////////////////////////
bool const Collectable::isActive() const
{
	return m_active;
}

////////////////////////////////////////////////////////////
void Collectable::setActive(bool const t_state)
{
	m_active = t_state;
}

////////////////////////////////////////////////////////////
sf::Sprite const& Collectable::getSprite() const
{
	return m_sprite;
}

////////////////////////////////////////////////////////////