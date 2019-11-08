#include "Tank.h"

Tank::Tank(sf::Texture const& t_texture, std::vector<sf::Sprite>& t_wallSprites, std::vector<Target>& t_targets) :
	m_texture{ t_texture },
	m_wallSprites{ t_wallSprites },
	m_targets{ t_targets },
	FRICTION{ 0.99 },
	SPEED_LIMIT{ 100.0 },
	ACCELERATION{ 2.0 },
	TURN_SPEED{ 0.8 },
	m_bullet{ nullptr },
	m_score{ 0 }
{
	initSprites();
}

void Tank::update(double dt)
{	
	// Clamp the speed to a minimum and maximum speed
	std::clamp(m_speed, -SPEED_LIMIT, SPEED_LIMIT);

	m_previousPosition = m_tankBase.getPosition();
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

	if (checkWallCollision())
	{
		deflect();
	}
	else
	{
		m_enableRotation = true;
	}

	checkBulletWallCollisions();

	// Checks bullet-target collisions and increases score if a target hit
	if (checkBulletTargetCollisions())
	{
		m_score += 10;
	}

	if (m_bullet != nullptr)
	{
		bool stillAlive = m_bullet->update(dt);
		if (!stillAlive)
		{
			m_bullet = nullptr;
		}
	}

	if (m_fireTimer > 0.0f)
	{
		m_fireTimer -= dt / 1000.0f;
	}
	else if (m_fireTimer != 0.0f)
	{
		m_fireTimer = 0.0f;
	}
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
	if (m_bullet != nullptr)
	{
		window.draw(m_bullet->getBody());
	}
}


void Tank::setPosition(sf::Vector2f t_position)
{
	m_tankBase.setPosition(t_position);
	m_turret.setPosition(t_position);
}

int Tank::getScore()
{
	return m_score;
}

void Tank::resetScore()
{
	m_score = 0;
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
void Tank::setPrevious()
{
	m_previousSpeed = m_speed;
	m_previousRotation = m_rotation;
	m_previousTurretRotation = m_turretRotation;
}

////////////////////////////////////////////////////////////
void Tank::handleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		increaseSpeed();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		increaseTurretRotation();
		m_centringTurret = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && m_bullet == nullptr && m_fireTimer == 0)
	{
		m_bullet = new Bullet(m_texture, m_turret.getPosition(), m_turret.getRotation(), 60.0f);
		m_fireTimer = FIRE_DELAY;
	}	
}

////////////////////////////////////////////////////////////
void Tank::handleControllerInput(XBox360Controller t_controller)
{
	if (t_controller.currentState.LeftThumbStick.y < -25.0f)
	{
		increaseSpeed();
	}
	else if (t_controller.currentState.LeftThumbStick.y > 25.0f)
	{
		decreaseSpeed();
	}

	if (t_controller.currentState.LeftThumbStick.x > 25.0f)
	{
		increaseRotation();
	}
	else if (t_controller.currentState.LeftThumbStick.x < -25.0f)
	{
		decreaseRotation();
	}

	if (t_controller.currentState.RightThumbStick.x > 25.0f)
	{
		increaseTurretRotation();
		m_centringTurret = false;
	}
	else if (t_controller.currentState.RightThumbStick.x < -25.0f)
	{
		decreaseTurretRotation();
		m_centringTurret = false;
	}

	if (t_controller.currentState.A && m_bullet == nullptr && m_fireTimer == 0)
	{
		m_bullet = new Bullet(m_texture, m_turret.getPosition(), m_turret.getRotation(), 60.0f);
		m_fireTimer = FIRE_DELAY;
	}
}

////////////////////////////////////////////////////////////
bool Tank::checkWallCollision()
{
	for (sf::Sprite const & sprite : m_wallSprites)
	{
		// Checks if either the tank base or turret has collided with the current wall sprite
		if (CollisionDetector::collision(m_turret, sprite)
			|| CollisionDetector::collision(m_tankBase, sprite))
		{
			return true;
		}
	}
	return false;
}

void Tank::checkBulletWallCollisions()
{
	if (m_bullet != nullptr)
	{
		for (sf::Sprite const& sprite : m_wallSprites)
		{
			// Checks if either the tank base or turret has collided with the current wall sprite
			if (CollisionDetector::collision(m_bullet->getBody(), sprite))
			{
				delete m_bullet;
				m_bullet = nullptr;
				break;
			}
		}
	}
}

bool Tank::checkBulletTargetCollisions()
{
	bool collision{ false };

	if (m_bullet != nullptr)
	{
		for (Target & target : m_targets)
		{
			// Checks if either the tank base or turret has collided with the current target and the target is alive
			if (target.active()
				&& CollisionDetector::collision(m_bullet->getBody(), target.getSprite()))
			{
				delete m_bullet;
				m_bullet = nullptr;
				target.setActive(false);
				collision = true;
				break;
			}
		}
	}

	return collision;
}

////////////////////////////////////////////////////////////
void Tank::deflect()
{
	// In case the tank was rotating
	adjustRotation();

	// If the tank was moving
	if (m_speed != 0)
	{
		// Temporarily disable turret rotations on collision.
		m_enableRotation = false;
		m_centringTurret = false;
		// Back up to position in previous frame.
		m_tankBase.setPosition(m_previousPosition);
		// Apply small force in opposite direction of travel
		if (m_previousSpeed < 0)
		{
			m_speed = 8;
		}
		else
		{
			m_speed = -8;
		}
	}
}

////////////////////////////////////////////////////////////
void Tank::adjustRotation()
{
	// If tank was rotating...
	if (m_rotation != m_previousRotation)
	{
		// Work out which direction to rotate the tank base post-collision.
		if (m_rotation > m_previousRotation)
		{
			m_rotation = m_previousRotation - 1;
		}
		else
		{
			m_rotation = m_previousRotation + 1;
		}
	}

	// If turret was rotating while tank was moving
	if (m_turretRotation != m_previousTurretRotation)
	{
		// Set the turret rotation back to it's pre-collision value
		m_turretRotation = m_previousTurretRotation;
	}
}

////////////////////////////////////////////////////////////
void Tank::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);

}