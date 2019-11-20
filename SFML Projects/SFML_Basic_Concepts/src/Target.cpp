#include "Target.h"

static int s_targetsSpawned = 0;

Target::Target(sf::Texture& t_texture, sf::IntRect t_textureRect, sf::Vector2f t_basePosition,
			   int t_randomOffset, float t_rotation, float t_durationSeconds) :
	m_BASE_POSITION{ t_basePosition },
	m_RANDOM_OFFSET{ t_randomOffset },
	m_BASE_SECONDS_TO_LIVE{ t_durationSeconds },
	m_active{ false }
{
	m_sprite.setTexture(t_texture);
	m_sprite.setTextureRect(t_textureRect);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
	m_sprite.setRotation(t_rotation);
}

void Target::resetTarget(std::vector<sf::Sprite> t_walls)
{
	// Decides the random offset and sets the position
	float xPosition = m_BASE_POSITION.x + rand() % (m_RANDOM_OFFSET * 2 + 1) - m_RANDOM_OFFSET;
	m_sprite.setPosition(xPosition, m_BASE_POSITION.y);

	m_secondsToLive = m_BASE_SECONDS_TO_LIVE;
	m_active = false;

	// Check the target is not colliding with a wall in the current position
	for (sf::Sprite const& obstacle : t_walls)
	{
		while (CollisionDetector::collision(m_sprite, obstacle)) // While the tank is colliding
		{
			m_sprite.move(5.0f, 0.0f); // Move the tank 5 pixels right
		}
	}
}

float Target::getSecondsToLive()
{
	return m_secondsToLive;
}

void Target::addSecondsToLive(float t_seconds)
{
	m_secondsToLive += t_seconds;
}

void Target::update(sf::Time t_timeSinceLastUpdate)
{
	if (m_secondsToLive > 0.0f)
	{
		m_secondsToLive -= t_timeSinceLastUpdate.asSeconds();

		if (m_secondsToLive <= 0.0f)
		{
			m_secondsToLive = 0.0;
			m_active = false;
		}
	}
}

void Target::draw(sf::RenderWindow& t_window, sf::CircleShape& t_timerCircle) const
{
	t_timerCircle.setRadius(m_secondsToLive * 10);
	t_timerCircle.setOrigin(t_timerCircle.getRadius(), t_timerCircle.getRadius());
	t_timerCircle.setPosition(m_sprite.getPosition());
	
	t_window.draw(t_timerCircle);
	t_window.draw(m_sprite);
}

void Target::setActive(bool t_activeState)
{
	m_active = t_activeState;
}

bool Target::active() const
{
	return m_active;
}

sf::Sprite Target::getSprite() const
{
	return m_sprite;
}
