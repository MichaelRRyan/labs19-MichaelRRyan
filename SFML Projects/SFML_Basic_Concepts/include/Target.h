#ifndef TARGET_H
#define TARGET_H

#include <SFML/Graphics.hpp>
#include <CollisionDetector.h>
#include "CircularSectorShape.h"

class Target
{
public:
	Target() = default;
	Target(sf::Texture &t_texture, sf::IntRect t_textureRect, sf::Vector2f t_basePosition,
		   int t_randomOffset, float t_rotation, float t_durationSeconds);

	void resetTarget(std::vector<sf::Sprite> t_walls);

	float getSecondsToLive();
	void addSecondsToLive(float t_seconds);

	void update(sf::Time t_timeSinceLastUpdate);
	void draw(sf::RenderWindow& t_window, CircularSectorShape& t_timerShape) const;

	void setActive(bool t_activeState);

	bool active() const;
	sf::Sprite getSprite() const;

protected:
	const float m_BASE_SECONDS_TO_LIVE;
	const sf::Vector2f m_BASE_POSITION;
	const int m_RANDOM_OFFSET;

	sf::Sprite m_sprite;
	float m_maxTimeToLive;
	float m_secondsToLive;
	bool m_active;
};

#endif // !TARGET_H