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

////////////////////////////////////////////////////////////
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
	checkBulletTargetCollisions();

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

////////////////////////////////////////////////////////////
void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
	if (m_bullet != nullptr)
	{
		window.draw(m_bullet->getBody());
	}
}

////////////////////////////////////////////////////////////
void Tank::setPosition(sf::Vector2f t_position)
{
	m_tankBase.setPosition(t_position);
	m_turret.setPosition(t_position);
}

////////////////////////////////////////////////////////////
int Tank::getScore()
{
	return m_score;
}

////////////////////////////////////////////////////////////
int Tank::getTargetsHit()
{
	return m_targetsHit;
}

////////////////////////////////////////////////////////////
int Tank::getBulletsFired()
{
	return m_bulletsFired;
}

////////////////////////////////////////////////////////////
void Tank::resetScore()
{
	m_score = 0;
	m_bulletsFired = 0;
	m_targetsHit = 0;
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

void Tank::increaseSpeed(float t_percent)
{
	m_speed += ACCELERATION * t_percent;
}

void Tank::decreaseSpeed(float t_percent)
{
	m_speed -= ACCELERATION * t_percent;
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
		m_bulletsFired++;
	}
}

////////////////////////////////////////////////////////////
void Tank::handleControllerInput(XBox360Controller t_controller)
{
	//////////////////////////////////////////////////////////////////////////////////////
	/// INPUT
	//////////////////////////////////////////////////////////////////////////////////////
	sf::Vector2f movementInputVector;

	// Vertical
	if (t_controller.currentState.LeftThumbStick.y > CONTROLLER_ANALOG_DEADZONE
		|| t_controller.currentState.LeftThumbStick.y < -CONTROLLER_ANALOG_DEADZONE)
	{
		movementInputVector.y = t_controller.currentState.LeftThumbStick.y;
	}

	// Horizontal
	if (t_controller.currentState.LeftThumbStick.x > CONTROLLER_ANALOG_DEADZONE
		|| t_controller.currentState.LeftThumbStick.x < -CONTROLLER_ANALOG_DEADZONE)
	{
		movementInputVector.x = t_controller.currentState.LeftThumbStick.x;
	}

	float inputSpeed = thor::length(movementInputVector);

	// Get the angle in degrees from the movement vector
	float inputDirection = atan2f(movementInputVector.y, movementInputVector.x) * MathUtility::RAD_TO_DEG;

	// Deal with negative rotations
	if (inputDirection < 0.0f)
	{
		inputDirection = 360 + inputDirection;
	}

	//////////////////////////////////////////////////////////////////////////////////////
	/// MOVEMENT
	//////////////////////////////////////////////////////////////////////////////////////
	if (inputSpeed > 100)
	{
		inputSpeed = 100;
	}

	if (fabsf(inputDirection - m_rotation) < 90.0f)
	{
		increaseSpeed(inputSpeed / 100.0f);
	}
	else
	{
		decreaseSpeed(inputSpeed / 100.0f);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	/// ROTATION
	//////////////////////////////////////////////////////////////////////////////////////
	if (inputSpeed > 0.0f)
	{
		float targetRotation = m_rotation;

		/*if (fabsf(inputDirection - m_rotation) < 90.0f)
		{
			targetRotation = m_rotation - 180;

			if (targetRotation < 0.0f)
			{
				
				targetRotation + 360;
			}
		}
		*/

		// Check if the absolute value of the distance between the values
		if (fabsf(inputDirection - targetRotation) > TURN_SPEED)
		{

			if (inputDirection > targetRotation)
			{
				if (fabsf(inputDirection - targetRotation) < 180)
				{
					increaseRotation();
				}
				else
				{
					decreaseRotation();
				}
			}
			else
			{
				if (fabsf(inputDirection - targetRotation) < 180)
				{
					decreaseRotation();
				}
				else
				{
					increaseRotation();
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	/// TURRET ROTATION
	//////////////////////////////////////////////////////////////////////////////////////
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

	if (t_controller.currentState.RTrigger > 50.0f
		&& m_bullet == nullptr && m_fireTimer == 0)
	{
		m_bullet = new Bullet(m_texture, m_turret.getPosition(), m_turret.getRotation(), 60.0f);
		m_fireTimer = FIRE_DELAY;
		m_bulletsFired++;
	}

	if (t_controller.currentState.RightThumbStickClick)
	{
		toggleCentring();
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

void Tank::checkBulletTargetCollisions()
{
	int numberOfTargets = m_targets.size();

	if (m_bullet != nullptr)
	{
		for (int i = 0; i < numberOfTargets; i++)
		{
			// Checks if either the tank base or turret has collided with the current target and the target is alive
			if (m_targets[i].active()
				&& CollisionDetector::collision(m_bullet->getBody(), m_targets[i].getSprite()))
			{
				delete m_bullet;
				m_bullet = nullptr;

				m_targets[i].setActive(false);

				m_targetsHit++;
				m_score += 10;

				// Check if there's a target after the current one
				if (i + 1 < numberOfTargets)
				{
					m_targets[i + 1].addSecondsToLive(m_targets[i].getSecondsToLive());
				}
				else
				{
					m_score += m_targets[i].getSecondsToLive();
				}

				break;
			}
		}
	}
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
std::string Tank::getStatistics()
{
	if (m_bulletsFired > 0)
	{
		return "Targets hit: " + std::to_string(m_targetsHit)
			+ "\nAccuracy: " + std::to_string(static_cast<int>((1.0 * m_targetsHit / m_bulletsFired) * 100.0))
			+ "%\nOverall Score: " + std::to_string(m_score);
	}
	else
	{
		return "Targets hit: " + std::to_string(m_targetsHit)
			+ "\nAccuracy: [No bullets fired]"
			+ "\nOverall Score: " + std::to_string(m_score);
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