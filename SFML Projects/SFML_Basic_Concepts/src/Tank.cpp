#include "Tank.h"

Tank::Tank(sf::Texture const& texture, sf::Vector2f const& pos) :
	m_texture(texture),
	FRICTION{ 0.99 },
	SPEED_LIMIT{ 100.0 },
	ACCELERATION{ 2.0 },
	TURN_SPEED{ 0.8 }
{
	initSprites(pos);
}

void Tank::update(double dt)
{	
	// Get input and move the tank
	HandleKeyInput();

	// Clamp the speed to a minimum and maximum speed
	std::clamp(m_speed, -SPEED_LIMIT, SPEED_LIMIT);

	sf::Vector2f m_newPosition; // Create a variable for new position calculations
	
	// Calculate the new position
	m_newPosition.x = m_tankBase.getPosition().x + cos(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);
	m_newPosition.y = m_tankBase.getPosition().y + sin(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);

	// Check if the turret is centring
	if (m_centringTurret)
	{
		centreTurret();
	}

	// Set the position of the tank base and turret
	m_tankBase.setPosition(m_newPosition);
	m_turret.setPosition(m_newPosition);

	// Set the rotation of the tank base and turret
	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_rotation + m_turretRotation);

	m_speed *= FRICTION; // Apply a friction force

	// If the speed is less than 1 pixel per frame set it to 0 to avoid jitter
	if (m_speed < 1.0 && m_speed > -1.0)
	{
		m_speed = 0.0;
	}
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
}

void Tank::setPosition(sf::Vector2f t_position)
{
	m_tankBase.setPosition(t_position);
	m_turret.setPosition(t_position);
}

////////////////////////////////////////////////////////////
void Tank::increaseSpeed()
{
	m_speed += ACCELERATION;
}

////////////////////////////////////////////////////////////
void Tank::decreaseSpeed()
{
	m_speed -= ACCELERATION;
}

////////////////////////////////////////////////////////////
void Tank::increaseRotation()
{
	m_rotation += TURN_SPEED;
	if (m_rotation >= 360.0)
	{
		m_rotation = 0;
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseRotation()
{
	m_rotation -= TURN_SPEED;
	if (m_rotation <= 0.0)
	{
		m_rotation = 359.0;
	}
}

////////////////////////////////////////////////////////////
void Tank::increaseTurretRotation()
{
	m_turretRotation += TURN_SPEED;
	if (m_turretRotation >= 360.0)
	{
		m_turretRotation = 0;
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseTurretRotation()
{
	m_turretRotation -= TURN_SPEED;
	if (m_turretRotation <= 0.0)
	{
		m_turretRotation = 359.0;
	}
}

////////////////////////////////////////////////////////////
void Tank::toggleCentring()
{
	m_centringTurret = !m_centringTurret;
	m_centringClockwise = (m_turretRotation > 180);
}

////////////////////////////////////////////////////////////
void Tank::centreTurret()
{
	// Give the turret a snap zone
	if (m_turretRotation < 360 - TURN_SPEED && m_turretRotation > TURN_SPEED)
	{
		if (m_centringClockwise)
		{
			increaseTurretRotation();
		}
		else
		{
			decreaseTurretRotation();
		}
	}
	else // Snap the turret if close enough to 0
	{
		m_turretRotation = 0;
		m_centringTurret = false;
	}
}

////////////////////////////////////////////////////////////
void Tank::HandleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		increaseSpeed();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		decreaseRotation();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		increaseRotation();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		decreaseTurretRotation();
		m_centringTurret = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		increaseTurretRotation();
		m_centringTurret = false;
	}
}

void Tank::initSprites(sf::Vector2f const & pos)
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_tankBase.setPosition(pos);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	m_turret.setPosition(pos);

}