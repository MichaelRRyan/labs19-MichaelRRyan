#include "Tank.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 11/10/19
/// </summary>

////////////////////////////////////////////////////////////
Tank::Tank(sf::Texture const& t_texture, sf::Texture const& t_guiTexture, std::vector<sf::Sprite>& t_wallSprites, std::vector<Target>& t_targets) :
	m_texture{ t_texture },
	m_wallSprites{ t_wallSprites },
	m_targets{ t_targets },
	FRICTION{ 0.98f },
	SPEED_LIMIT{ 200.0f },
	ACCELERATION{ 1.0f },
	TURN_SPEED{ 0.8f },
	m_BULLET_DAMAGE{ 10.0f },
	FIRE_DELAY{ 1.0f },
	m_bullet{ nullptr },
	m_score{ 0 },
	m_healthPercent{ 100.0f },
	m_ptrController{ nullptr },
	m_healthIndicator{ 65.0f, 0.0f, 60u },
	m_fireRequested{ false },
	m_shootTimer{ s_TIME_BETWEEN_SHOTS }
{
	initSprites();
	setupParticleSystems(t_guiTexture);
}

////////////////////////////////////////////////////////////
void Tank::update(double dt)
{	
	if (m_healthPercent > 0.0f)
	{
		setPrevious(); // Set all "previous" variables

		m_moving = false;

		// Take input based on control type
		(ControlType::Keyboard == m_controlType) ? handleKeyInput() : handleControllerInput();

		// Clamp the speed to a minimum and maximum speed
		clamp(m_speed, -SPEED_LIMIT, SPEED_LIMIT);

		m_previousPosition = m_baseSprite.getPosition();
		sf::Vector2f m_newPosition; // Create a variable for new position calculations

		// Calculate the new position
		m_newPosition.x = m_baseSprite.getPosition().x + cosf(thor::toRadian(m_rotation)) * m_speed * (static_cast<float>(dt) / 1000.0f);
		m_newPosition.y = m_baseSprite.getPosition().y + sinf(thor::toRadian(m_rotation)) * m_speed * (static_cast<float>(dt) / 1000.0f);

		// Check if the turret is centring
		if (m_centringTurret)
		{
			centreTurret();
		}

		// Set the position of the tank base
		m_baseSprite.setPosition(m_newPosition);

		// Set the position of the turret with n offset for fire recoil
		sf::Vector2f directionVector = { cosf(thor::toRadian(m_rotation + m_turretRotation)), sinf(thor::toRadian(m_rotation + m_turretRotation)) };
		m_turretSprite.setPosition(m_newPosition - (directionVector * m_fireTimer * 3.0f));

		// Set the rotation of the tank base and turret
		m_baseSprite.setRotation(m_rotation);
		m_turretSprite.setRotation(m_rotation + m_turretRotation);

		if (!m_moving)
		{
			m_speed *= FRICTION; // Apply a friction force
		}

		// If the speed is less than 1 pixel per frame set it to 0 to avoid jitter
		if (abs(m_speed) < 1.0)
		{
			m_speed = 0.0;
			m_driveSound.stop();
		}
		// If the tank is moving
		else
		{
			// Manage the volume of the tank movement
			if (abs(m_speed) < 100)
			{
				m_driveSound.setVolume(abs(m_speed));
			}
			else
			{
				m_driveSound.setVolume(100.0f);
			}

			if (m_driveSound.getStatus() != sf::SoundSource::Status::Playing)
			{
				m_driveSound.play();
			}
		}

		// Manage turret rotation sounds
		if (m_previousTurretRotation != m_turretRotation)
		{
			// Only play the track if not already playing
			if (m_turretRotateSound.getStatus() != sf::SoundSource::Status::Playing)
			{
				m_turretRotateSound.play();
			}
		}
		else
		{
			m_turretRotateSound.stop();
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
			m_fireTimer -= m_fireClock.restart().asSeconds();
		}
		else if (m_fireTimer != 0.0f)
		{
			m_fireTimer = 0.0f;
		}
	}

	if (m_fireRequested)
	{
		m_shootTimer -= dt;

		if (m_shootTimer <= 0)
		{
			m_shootTimer = s_TIME_BETWEEN_SHOTS;
			m_fireRequested = false;
		}
	}

	m_pool.update(dt, m_wallSprites);
}

////////////////////////////////////////////////////////////
void Tank::updateParticleSys()
{
	m_explosionPartSys.update(m_particleClock.getElapsedTime());
	m_smokePartSys.update(m_particleClock.restart());
}

////////////////////////////////////////////////////////////
void Tank::render(sf::RenderWindow & window) 
{
	if (m_healthPercent > 0.0f)
	{
		window.draw(m_baseSprite);
		window.draw(m_turretSprite);
	}
	
	/*if (m_bullet != nullptr)
	{
		window.draw(m_bullet->getSprite());
	}*/

	m_pool.render(window);

	window.draw(m_smokePartSys);
	window.draw(m_explosionPartSys);
}

////////////////////////////////////////////////////////////
void Tank::setPosition(sf::Vector2f t_position)
{
	m_baseSprite.setPosition(t_position);
	m_turretSprite.setPosition(t_position);
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
	m_moving = true;
}

////////////////////////////////////////////////////////////
void Tank::decreaseSpeed()
{
	m_speed -= ACCELERATION;
	m_moving = true;
}

////////////////////////////////////////////////////////////
void Tank::increaseSpeed(float t_percent)
{
	m_speed += ACCELERATION * t_percent;
	m_moving = true;
}

////////////////////////////////////////////////////////////
void Tank::decreaseSpeed(float t_percent)
{
	m_speed -= ACCELERATION * t_percent;
	m_moving = true;
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
	if (KeyConfiguration::ArrowKeys == m_controlScheme)
	{
		arrowKeysInput();
	}
	else
	{
		YGHJKeysInput();
	}
}

////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////
void Tank::YGHJKeysInput()
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
	//////////////////////////////////////////////////////////////////////////////////////
	/// INPUT
	//////////////////////////////////////////////////////////////////////////////////////
	sf::Vector2f movementInputVector = getControllerLSInput();

	float inputSpeed = thor::length(movementInputVector);

	// Get the angle in degrees from the movement vector
	float inputDirection = thor::toDegree(atan2f(movementInputVector.y, movementInputVector.x));

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
		// Check if the absolute value of the distance between the values
		if (fabsf(inputDirection - m_rotation) > TURN_SPEED)
		{

			if (inputDirection > m_rotation)
			{
				if (fabsf(inputDirection - m_rotation) < 180)
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
				if (fabsf(inputDirection - m_rotation) < 180)
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
	// Turret Rotation
	sf::Vector2f rightStickInputVector = rightStickInputVector = getControllerRSInput();

	// Get the angle in degrees from the movement vector
	float rightStickInputDir = thor::toDegree(atan2f(rightStickInputVector.y, rightStickInputVector.x));

	// Deal with negative rotations
	if (rightStickInputDir < 0.0f)
	{
		rightStickInputDir = 360 + rightStickInputDir;
	}

	// Turret Rotation
	if (thor::length(rightStickInputVector) > 0.0f) // If there is some analog input
	{
		handleControllerTurretRotation(rightStickInputDir);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	/// TURRET FIRING AND CENTERING
	//////////////////////////////////////////////////////////////////////////////////////
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
		if (CollisionDetector::collision(m_turretSprite, sprite)
			|| CollisionDetector::collision(m_baseSprite, sprite))
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
void Tank::checkBulletWallCollisions()
{
	if (m_bullet != nullptr)
	{
		for (sf::Sprite const& sprite : m_wallSprites)
		{
			// Checks if either the tank base or turret has collided with the current wall sprite
			if (CollisionDetector::collision(m_bullet->getSprite(), sprite))
			{
				// Emit smoke particles at the point of impact
				m_smokeEmitter.setParticlePosition(m_bullet->getSprite().getPosition()); // Emit particles at tank barrel end
				m_smokeEmitter.setParticleVelocity(thor::Distributions::deflect(-m_bullet->getVelocity() * 0.2f, 45.f)); // Emit towards direction with deviation of 45°
				m_smokePartSys.addEmitter(m_smokeEmitter, sf::seconds(0.1f));

				delete m_bullet;
				m_bullet = nullptr;
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////
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
				&& CollisionDetector::collision(m_bullet->getSprite(), m_targets[i].getSprite()))
			{
				// Delete the bullet hit
				delete m_bullet;
				m_bullet = nullptr;

				// Emit explosion particles on impact
				m_explosionEmitter.setParticlePosition(m_targets[i].getSprite().getPosition()); // Emit particles at tank barrel end
				m_explosionEmitter.setParticleVelocity(thor::Distributions::deflect({ 50.0f, 0.0 }, 180.f)); // Emit towards direction with deviation of 180°
				m_explosionPartSys.addEmitter(m_explosionEmitter, sf::seconds(0.3f));

				m_explosionSound.play();

				// Set the target to inactive
				m_targets[i].setActive(false);

				// Increase target hit and score
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
		m_baseSprite.setPosition(m_previousPosition);
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
	// Check collisions only if the tank is alive
	if (t_tank.getHealth() > 0.0f)
	{
		// Check against tank on tank collision
		if (CollisionDetector::collision(m_baseSprite, t_tank.getSprite()))
		{
			deflect();
		}

		// If the bullet is alive
		if (m_bullet != nullptr)
		{
			// Check collisions between bullet and tank
			if (CollisionDetector::collision(m_bullet->getSprite(), t_tank.getSprite()))
			{
				t_tank.takeDamage(m_BULLET_DAMAGE); // Damage the other tank
				delete m_bullet; // Delete the bullet
				m_bullet = nullptr; // Set the bullet pointer to nullptr to avoid dangling pointer
			}
		}
	}
}

////////////////////////////////////////////////////////////
void Tank::takeDamage(float t_amount)
{
	m_healthPercent -= t_amount;

	// If health is less than or equal to 0, play death sound and emit particles
	if (m_healthPercent <= 0.0f)
	{
		m_healthPercent = 0;
		m_explosionSound.play();

		// Emit explosion particles
		m_explosionEmitter.setParticlePosition(m_baseSprite.getPosition()); // Emit particles at tank position
		m_explosionEmitter.setParticleVelocity(thor::Distributions::deflect({ 50.0f, 0.0 }, 180.f)); // Emit towards direction with deviation of 180°
		m_explosionPartSys.addEmitter(m_explosionEmitter, sf::seconds(0.3f));
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
	return m_baseSprite;
}

void Tank::setSounds(sf::SoundBuffer const& t_shotSoundBuffer, sf::SoundBuffer const& t_explosionSoundBuffer, sf::SoundBuffer const& t_driveSoundBuffer,
						sf::SoundBuffer const& t_turretRotateSound)
{
	m_shotSound.setBuffer(t_shotSoundBuffer);
	m_explosionSound.setBuffer(t_explosionSoundBuffer);
	m_driveSound.setBuffer(t_driveSoundBuffer);
	m_turretRotateSound.setBuffer(t_turretRotateSound);
}

////////////////////////////////////////////////////////////
void Tank::fireBullet()
{
	if (m_bullet == nullptr && m_fireTimer == 0)
	{
		m_bullet = new Bullet(m_texture, m_turretSprite.getPosition(), m_turretSprite.getRotation(), 60.0f);
		m_fireTimer = FIRE_DELAY;
		m_bulletsFired++;
		m_shotSound.play();
		requestFire();

		sf::Vector2f directionVector = { cos(thor::toRadian(m_turretSprite.getRotation())), sin(thor::toRadian(m_turretSprite.getRotation())) };
		sf::Vector2f tankBarrelPosition = m_turretSprite.getPosition() + directionVector * 60.0f;

		m_smokeEmitter.setParticlePosition(tankBarrelPosition); // Emit particles at tank barrel end
		m_smokeEmitter.setParticleVelocity(thor::Distributions::deflect(directionVector * 300.0f, 45.0f)); // Emit towards direction with deviation of 45°
		m_smokePartSys.addEmitter(m_smokeEmitter, sf::seconds(0.1f));

		m_fireClock.restart();
	}
}

////////////////////////////////////////////////////////////
void Tank::setControlType(ControlType t_controlType, KeyConfiguration t_controlScheme)
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
		if (KeyConfiguration::ArrowKeys == m_controlScheme)
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
	return m_baseSprite.getPosition();
}

////////////////////////////////////////////////////////////
void Tank::drawHealthIndicator(sf::RenderWindow& t_window)
{
	m_healthIndicator.setPosition(m_baseSprite.getPosition());
	m_healthIndicator.setCompleteness(m_healthPercent / 100.0f);
	t_window.draw(m_healthIndicator);
}

////////////////////////////////////////////////////////////
void Tank::initSprites()
{
	// Initialise the tank base
	m_baseSprite.setTexture(m_texture);
	sf::IntRect baseRect(95, 42, 100, 51);
	m_baseSprite.setTextureRect(baseRect);
	m_baseSprite.setOrigin(static_cast<float>(baseRect.width) / 2.0f, static_cast<float>(baseRect.height) / 2.0f);

	// Initialise the turret
	m_turretSprite.setTexture(m_texture);
	sf::IntRect turretRect(120, 3, 87, 37);
	m_turretSprite.setTextureRect(turretRect);
	m_turretSprite.setOrigin(static_cast<float>(turretRect.width) / 3.0f, static_cast<float>(turretRect.height) / 2.0f);

	m_healthIndicator.setFillColor(sf::Color{ 0, 255, 0, 100 });
	m_healthIndicator.setOrigin(65.0f, 65.0f);
}

////////////////////////////////////////////////////////////
void Tank::setupParticleSystems(sf::Texture const& t_guiTexture)
{
	// Setup the smoke particle system
	m_smokePartSys.setTexture(t_guiTexture);

	m_smokePartSys.addTextureRect({ 550, 0, 100, 100 });
	m_smokePartSys.addTextureRect({ 650, 0, 100, 100 });
	m_smokePartSys.addTextureRect({ 550, 100, 100, 100 });
	m_smokePartSys.addTextureRect({ 650, 100, 100, 100 });

	m_smokeEmitter.setEmissionRate(120);
	m_smokeEmitter.setParticleLifetime(sf::seconds(0.30f));
	m_smokeEmitter.setParticleTextureIndex(thor::Distributions::uniform(0, 3));
	m_smokeEmitter.setParticleScale(sf::Vector2f{ 0.3f, 0.3f });

	thor::FadeAnimation fader(0.0f, 0.8f);
	m_smokePartSys.addAffector(thor::AnimationAffector(fader));

	// Setup the explosion particle system
	m_explosionPartSys.setTexture(t_guiTexture);

	for (int row = 0; row < 2; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			m_explosionPartSys.addTextureRect({ 500 + 100 * col, 200 + 100 * row, 100, 100 });
		}
	}

	m_explosionPartSys.addTextureRect({ 550, 0, 100, 100 });
	m_explosionPartSys.addTextureRect({ 650, 0, 100, 100 });
	m_explosionPartSys.addTextureRect({ 550, 100, 100, 100 });

	m_explosionEmitter.setEmissionRate(30);
	m_explosionEmitter.setParticleLifetime(sf::seconds(0.60f));
	m_explosionEmitter.setParticleTextureIndex(thor::Distributions::uniform(0, 8));
	m_explosionEmitter.setParticleScale(sf::Vector2f{ 0.1f, 0.1f });

	m_explosionPartSys.addAffector(thor::ScaleAffector({ 1.5f, 1.5f }));
	m_explosionPartSys.addAffector(thor::AnimationAffector(fader));
}

////////////////////////////////////////////////////////////
void Tank::clamp(float& t_value, float const t_minValue, float const t_maxValue)
{
	if (t_value > t_maxValue)
	{
		t_value = t_maxValue;
	}
	else if (t_value < t_minValue)
	{
		t_value = t_minValue;
	}
}

////////////////////////////////////////////////////////////
void Tank::requestFire()
{
	m_fireRequested = true;
	if (m_shootTimer == s_TIME_BETWEEN_SHOTS)
	{
		sf::Vector2f tipOfTurret(m_turretSprite.getPosition().x + 2.0f, m_turretSprite.getPosition().y);
		tipOfTurret.x += std::cos(MathUtility::DEG_TO_RAD * m_turretSprite.getRotation()) * ((m_turretSprite.getLocalBounds().top + m_turretSprite.getLocalBounds().height) * 1.7f);
		tipOfTurret.y += std::sin(MathUtility::DEG_TO_RAD * m_turretSprite.getRotation()) * ((m_turretSprite.getLocalBounds().top + m_turretSprite.getLocalBounds().height) * 1.7f);
		m_pool.create(m_texture, tipOfTurret.x, tipOfTurret.y, m_baseSprite.getRotation());
	}
}

////////////////////////////////////////////////////////////