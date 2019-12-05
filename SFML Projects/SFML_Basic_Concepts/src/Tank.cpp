#include "Tank.h"

Tank::Tank(sf::Texture const& t_texture, std::vector<sf::Sprite>& t_wallSprites, std::vector<Target>& t_targets) :
	m_texture{ t_texture },
	m_wallSprites{ t_wallSprites },
	m_targets{ t_targets },
	FRICTION{ 0.99f },
	SPEED_LIMIT{ 100.0f },
	ACCELERATION{ 2.0f },
	TURN_SPEED{ 0.8f },
	m_BULLET_DAMAGE{ 10.0f },
	m_bullet{ nullptr },
	m_score{ 0 },
	m_healthPercent{ 100.0f },
	m_ptrController{ nullptr }
{
	initSprites();
}

////////////////////////////////////////////////////////////
void Tank::update(double dt)
{	
	setPrevious(); // Set all "previous" variables

	// Take input based on control type
	(ControlType::Keyboard == m_controlType) ? handleKeyInput() : handleControllerInput();

	// Clamp the speed to a minimum and maximum speed
	float clamp = std::clamp(m_speed, -SPEED_LIMIT, SPEED_LIMIT); // Clamp variable is simply to remove warnings

	m_previousPosition = m_tankBase.getPosition();
	sf::Vector2f m_newPosition; // Create a variable for new position calculations
	
	// Calculate the new position
	m_newPosition.x = m_tankBase.getPosition().x + cosf(thor::toRadian(m_rotation)) * m_speed * (static_cast<float>(dt) / 1000.0f);
	m_newPosition.y = m_tankBase.getPosition().y + sinf(thor::toRadian(m_rotation)) * m_speed * (static_cast<float>(dt) / 1000.0f);

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

	if (m_bullet != nullptr)
	{
		bool stillAlive = m_bullet->update(dt);
		if (!stillAlive)
		{
			m_bullet = nullptr;
		}
	}

	// Decrement the fire timer and set it to 0 if it goes below
	if (m_fireTimer > 0.0f)
	{
		m_fireTimer -= static_cast<float>(dt) / 1000.0f;
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
int Tank::getPercentTargetsHit()
{
	return static_cast<int>((1.0 * m_targetsHit / m_targets.size()) * 100.0);
}

////////////////////////////////////////////////////////////
int Tank::getAccuracy()
{
	if (m_bulletsFired > 0)
	{
		return static_cast<int>(1.0 * m_targetsHit / m_bulletsFired * 100.0);
	}
	else
	{
		return 0;
	}
}

////////////////////////////////////////////////////////////
void Tank::resetStats()
{
	m_score = 0;
	m_bulletsFired = 0;
	m_targetsHit = 0;
	m_healthPercent = 100.0f;
}

////////////////////////////////////////////////////////////
void Tank::resetRotation()
{
	m_rotation = 0.0f;
	m_turretRotation = 0.0f;
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
	if (ControlScheme::ArrowKeys == m_controlScheme)
	{
		arrowKeysInput();
	}
	else
	{
		YGHJJKeysInput();
	}
}

void Tank::arrowKeysInput()
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
	{
		decreaseTurretRotation();
		m_centringTurret = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Slash))
	{
		increaseTurretRotation();
		m_centringTurret = false;
	}

	// If the fire button is pressed and the bullet is null and the fire timer is zero
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
	{
		fireBullet();
	}
}

void Tank::YGHJJKeysInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		increaseSpeed();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
	{
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
	{
		decreaseRotation();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		increaseRotation();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		decreaseTurretRotation();
		m_centringTurret = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		increaseTurretRotation();
		m_centringTurret = false;
	}

	// If the fire button is pressed and the bullet is null and the fire timer is zero
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		fireBullet();
	}
}

////////////////////////////////////////////////////////////
void Tank::handleControllerInput()
{
	// Input
	sf::Vector2f movementInputVector;
	sf::Vector2f rightStickInputVector;

	movementInputVector = getControllerLSInput();
	rightStickInputVector = getControllerRSInput();

	// Calculate
	float inputSpeed = thor::length(movementInputVector);

	// Get the angle in degrees from the movement vector
	float inputDirection = atan2f(movementInputVector.y, movementInputVector.x) * static_cast<float>(MathUtility::RAD_TO_DEG);

	// Deal with negative rotations
	if (inputDirection < 0.0f)
	{
		inputDirection = 360 + inputDirection;
	}

	// Get the angle in degrees from the movement vector
	float rightStickInputDir = thor::toDegree(atan2f(rightStickInputVector.y, rightStickInputVector.x));

	// Deal with negative rotations
	if (rightStickInputDir < 0.0f)
	{
		rightStickInputDir = 360 + rightStickInputDir;
	}

	// Movement
	if (inputSpeed > 100)
	{
		inputSpeed = 100;
	}

	if (fabsf(inputDirection - m_rotation) < 90.0f) // If analog direction is on the front half of the tank
	{
		increaseSpeed(inputSpeed / 100.0f); // move forward
	}
	else
	{
		decreaseSpeed(inputSpeed / 100.0f); // Move backwards
	}

	// Tank Body Rotation
	if (inputSpeed > 0.0f) // If there is some analog input
	{
		handleControllerRotation(inputDirection);
	}

	// Turret Rotation
	if (thor::length(rightStickInputVector) > 0.0f) // If there is some analog input
	{
		handleControllerTurretRotation(rightStickInputDir);
	}
	

	// Manage firing
	if (m_ptrController->currentState.RTrigger > 50.0f)
	{
		fireBullet();
	}

	if (m_ptrController->currentState.RightThumbStickClick)
	{
		toggleCentring();
	}
}

////////////////////////////////////////////////////////////
void Tank::handleControllerRotation(float t_inputDirection)
{
	if (fabsf(t_inputDirection - m_rotation) < 90.0f) // Rotating while going forward
	{
		// Check if the absolute value of the distance between the values is greater than  the turn speed to avoid jitter
		if (fabsf(t_inputDirection - m_rotation) > TURN_SPEED)
		{

			if (t_inputDirection > m_rotation)
			{
				if (fabsf(t_inputDirection - m_rotation) < 180)
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
				if (fabsf(t_inputDirection - m_rotation) < 180)
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
	else // Rotate while going backwards
	{
		// Check if the absolute value of the distance between the values is greater than  the turn speed to avoid jitter
		if (fabsf(t_inputDirection - m_rotation) > TURN_SPEED)
		{
			if (t_inputDirection < m_rotation)
			{
				if (fabsf(t_inputDirection - m_rotation) < 180)
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
				if (fabsf(t_inputDirection - m_rotation) < 180)
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

}

////////////////////////////////////////////////////////////
void Tank::handleControllerTurretRotation(float t_inputDirection)
{
	float trueTurretRotation = m_turretRotation + m_rotation;

	if (trueTurretRotation > 360.0f)
	{
		trueTurretRotation = trueTurretRotation - 360.0f;
	}

	// Deal with negative rotations
	if (trueTurretRotation < 0.0f)
	{
		trueTurretRotation = 360 + trueTurretRotation;
	}



	// Check if the absolute value of the distance between the values is greater than  the turn speed to avoid jitter
	if (fabsf(t_inputDirection - trueTurretRotation) > TURN_SPEED)
	{

		if (t_inputDirection > trueTurretRotation)
		{
			if (fabsf(t_inputDirection - trueTurretRotation) < 180)
			{
				increaseTurretRotation();
			}
			else
			{
				decreaseTurretRotation();
			}
		}
		else
		{
			if (fabsf(t_inputDirection - trueTurretRotation) < 180)
			{
				decreaseTurretRotation();
			}
			else
			{
				increaseTurretRotation();
			}
		}
	}

}

////////////////////////////////////////////////////////////
sf::Vector2f Tank::getControllerLSInput()
{
	sf::Vector2f movementInputVector;

	// Vertical
	if (m_ptrController->currentState.LeftThumbStick.y > CONTROLLER_ANALOG_DEADZONE
		|| m_ptrController->currentState.LeftThumbStick.y < -CONTROLLER_ANALOG_DEADZONE)
	{
		movementInputVector.y = m_ptrController->currentState.LeftThumbStick.y;
	}

	// Horizontal
	if (m_ptrController->currentState.LeftThumbStick.x > CONTROLLER_ANALOG_DEADZONE
		|| m_ptrController->currentState.LeftThumbStick.x < -CONTROLLER_ANALOG_DEADZONE)
	{
		movementInputVector.x = m_ptrController->currentState.LeftThumbStick.x;
	}

	return movementInputVector;
}

////////////////////////////////////////////////////////////
sf::Vector2f Tank::getControllerRSInput()
{
	sf::Vector2f rightStickInputVector;

	// Vertical
	if (m_ptrController->currentState.RightThumbStick.y > CONTROLLER_ANALOG_DEADZONE
		|| m_ptrController->currentState.RightThumbStick.y < -CONTROLLER_ANALOG_DEADZONE)
	{
		rightStickInputVector.y = m_ptrController->currentState.RightThumbStick.y;
		m_centringTurret = false;
	}

	// Horizontal
	if (m_ptrController->currentState.RightThumbStick.x > CONTROLLER_ANALOG_DEADZONE
		|| m_ptrController->currentState.RightThumbStick.x < -CONTROLLER_ANALOG_DEADZONE)
	{
		rightStickInputVector.x = m_ptrController->currentState.RightThumbStick.x;
		m_centringTurret = false;
	}

	return rightStickInputVector;
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
	int numberOfTargets = m_targets.size();
	bool collision{ false };

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
					m_score += static_cast<int>(m_targets[i].getSecondsToLive());
				}

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
void Tank::checkTanktoTankCollisions(Tank& t_tank)
{
	if (t_tank.getHealth() > 0.0f)
	{
		if (CollisionDetector::collision(m_tankBase, t_tank.getSprite()))
		{
			deflect();
		}

		if (m_bullet != nullptr)
		{
			if (CollisionDetector::collision(m_bullet->getBody(), t_tank.getSprite()))
			{
				t_tank.takeDamage(m_BULLET_DAMAGE);
				delete m_bullet;
				m_bullet = nullptr;
			}
		}
	}
}

////////////////////////////////////////////////////////////
void Tank::takeDamage(float t_amount)
{
	m_healthPercent -= t_amount;

	if (m_healthPercent <= 0.0f)
	{
		m_healthPercent = 0;
		m_explosionSound.play();
	}
}

////////////////////////////////////////////////////////////
float Tank::getHealth()
{
	return m_healthPercent;
}

////////////////////////////////////////////////////////////
std::string Tank::getStatistics()
{
	if (m_bulletsFired > 0) // Return the tank statistics
	{
		return "Targets hit: " + std::to_string(static_cast<int>((1.0 * m_targetsHit / m_targets.size()) * 100.0))
			+ "%\nAccuracy: " + std::to_string(static_cast<int>((1.0 * m_targetsHit / m_bulletsFired) * 100.0))
			+ "%\nOverall Score: " + std::to_string(m_score);
	}
	else // Avoid a divide by zero
	{
		return "Targets hit: " + std::to_string(static_cast<int>((1.0 * m_targetsHit / m_targets.size()) * 100.0))
			+ "%\nAccuracy: [No bullets fired]"
			+ "\nOverall Score: " + std::to_string(m_score);
	}
}

////////////////////////////////////////////////////////////
sf::Sprite Tank::getSprite()
{
	return m_tankBase;
}

void Tank::setSounds(sf::SoundBuffer const& t_shotSoundBuffer, sf::SoundBuffer const& t_explosionSoundBuffer)
{
	m_shotSound.setBuffer(t_shotSoundBuffer);
	m_explosionSound.setBuffer(t_explosionSoundBuffer);
}

////////////////////////////////////////////////////////////
void Tank::fireBullet()
{
	if (m_bullet == nullptr && m_fireTimer == 0)
	{
		m_bullet = new Bullet(m_texture, m_turret.getPosition(), m_turret.getRotation(), 60.0f);
		m_fireTimer = FIRE_DELAY;
		m_bulletsFired++;
		m_shotSound.play();
	}
}

////////////////////////////////////////////////////////////
void Tank::setControlType(ControlType t_controlType, ControlScheme t_controlScheme)
{
	m_controlType = t_controlType;
	m_controlScheme = t_controlScheme;
}

////////////////////////////////////////////////////////////
ControlType Tank::getControlType()
{
	return m_controlType;
}

////////////////////////////////////////////////////////////
void Tank::setControllerPtr(XBox360Controller *t_ptrController)
{
	m_ptrController = t_ptrController;
}

////////////////////////////////////////////////////////////
int Tank::getJoystickIndex()
{
	if (m_ptrController != nullptr)
	{
		return m_ptrController->joystickIndex;
	}

	return -1;
}

////////////////////////////////////////////////////////////
void Tank::processEvents(sf::Event t_event)
{
	if (sf::Event::KeyPressed == t_event.type)
	{
		if (ControlScheme::ArrowKeys == m_controlScheme)
		{
			if (sf::Keyboard::L == t_event.key.code)
			{
				toggleCentring();
			}
		}
		else
		{
			if (sf::Keyboard::Num2 == t_event.key.code)
			{
				toggleCentring();
			}
		}
	}
}

////////////////////////////////////////////////////////////
sf::Vector2f Tank::getPosition()
{
	return m_tankBase.getPosition();
}

////////////////////////////////////////////////////////////
void Tank::drawHealthIndicator(sf::RenderWindow& t_window)
{
	m_healthIndicator.setPosition(m_tankBase.getPosition());
	m_healthIndicator.setCompleteness(m_healthPercent / 100.0f);
	t_window.draw(m_healthIndicator);
}

////////////////////////////////////////////////////////////
void Tank::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(95, 42, 100, 51);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(static_cast<float>(baseRect.width) / 2.0f, static_cast<float>(baseRect.height) / 2.0f);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(120, 3, 87, 37);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(static_cast<float>(turretRect.width) / 3.0f, static_cast<float>(turretRect.height) / 2.0f);

	m_healthIndicator.setRadius(65.0f);
	m_healthIndicator.setFillColor(sf::Color{ 0, 255, 0, 100 });
	m_healthIndicator.setOrigin(65.0f, 65.0f);
}