#include "Bullet.h"

Bullet::Bullet(sf::Texture const& t_texture, sf::Vector2f t_position, float t_angleDeg, float t_barrelOffset) :
	m_texture{ t_texture },
	m_speed{ 1000.0f }
{
	float angleRad = t_angleDeg * (3.14f / 180.0f);
	sf::Vector2f directionVector{ cosf(angleRad), sinf(angleRad) };

	m_velocity = { directionVector * m_speed };

	m_sprite.setTexture(t_texture);
	m_sprite.setTextureRect({ 53, 60, 7, 15 });
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.0f, m_sprite.getGlobalBounds().height / 2.0f);

	m_sprite.setPosition(t_position + (directionVector * t_barrelOffset));
	m_sprite.setRotation(t_angleDeg - 90.0f);
}

bool Bullet::update(double dt)
{
	m_sprite.move(m_velocity * static_cast<float>(dt / 1000.0));

	if (m_sprite.getPosition().x < 0 || m_sprite.getPosition().x > ScreenSize::s_width
		|| m_sprite.getPosition().y < 0 || m_sprite.getPosition().y > ScreenSize::s_height)
	{
		std::cout << "Bullet Deleted" << std::endl;
		delete this;
		return false;
	}

	return true;
}
