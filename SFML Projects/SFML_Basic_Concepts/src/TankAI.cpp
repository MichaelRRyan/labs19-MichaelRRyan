#include "TankAi.h"

////////////////////////////////////////////////////////////
TankAi::TankAi(sf::Texture const& texture, std::vector<sf::Sprite>& wallSprites)
	: m_aiBehaviour(AiBehaviour::SEEK_PLAYER)
	, m_texture(texture)
	, m_wallSprites(wallSprites)
	, m_steering(0, 0),
	m_active{ true },
	m_healthPercent{ 100.0f },
	m_healthIndicator{ 65.0f, 0.0f, 60u }
{
	// Initialises the tank base and turret sprites.
	initSprites();
}

////////////////////////////////////////////////////////////
void TankAi::update(Tank playerTanks[], const int t_numberOfPlayers, double dt)
{
	if (!m_active)
	{
		return;
	}

	sf::Vector2f vectorToPlayer = seek(playerTanks, t_numberOfPlayers);
	sf::Vector2f acceleration;

	switch (m_aiBehaviour)
	{
	case AiBehaviour::SEEK_PLAYER:
		if (thor::length(vectorToPlayer) != 0.0f)
		{
			m_steering += thor::unitVector(vectorToPlayer);
		}

		m_steering += collisionAvoidance();
		m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
		acceleration = m_steering / MASS;
		//m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
		m_velocity = MathUtility::truncate(m_velocity + acceleration, MAX_SPEED);

		break;
	case AiBehaviour::STOP:
		m_velocity = sf::Vector2f(0, 0);
		//motion->m_speed = 0;
	default:
		break;
	}

	// Now we need to convert our velocity vector into a rotation angle between 0 and 359 degrees.
	// The m_velocity vector works like this: vector(1,0) is 0 degrees, while vector(0, 1) is 90 degrees.
	// So for example, 223 degrees would be a clockwise offset from 0 degrees (i.e. along x axis).
	// Note: we add 180 degrees below to convert the final angle into a range 0 to 359 instead of -PI to +PI
	auto dest = atan2(-1 * m_velocity.y, -1 * m_velocity.x) / thor::Pi * 180 + 180;

	auto currentRotation = m_rotation;

	// Find the shortest way to rotate towards the player (clockwise or anti-clockwise)
	if (std::round(currentRotation - dest) == 0.0)
	{
		m_steering.x = 0;
		m_steering.y = 0;
	}

	else if ((static_cast<int>(std::round(dest - currentRotation + 360))) % 360 < 180)
	{
		// rotate clockwise
		m_rotation = static_cast<int>((m_rotation) + 1) % 360;
	}
	else
	{
		// rotate anti-clockwise
		m_rotation = static_cast<int>((m_rotation) - 1) % 360;
	}


	if (thor::length(vectorToPlayer) < MAX_SEE_AHEAD)
	{
		m_aiBehaviour = AiBehaviour::STOP;
	}
	else
	{
		m_aiBehaviour = AiBehaviour::SEEK_PLAYER;
	}

	updateMovement(dt);
}

////////////////////////////////////////////////////////////
void TankAi::render(sf::RenderWindow & window)
{
	// TODO: Don't draw if off-screen...
	
	if (m_active)
	{
		window.draw(m_tankBase);
		window.draw(m_turret);
	}
}

////////////////////////////////////////////////////////////
void TankAi::init(sf::Vector2f position)
{
	m_tankBase.setPosition(position);
	m_turret.setPosition(position);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width * 1.5f);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(wallSprite.getPosition());
		m_obstacles.push_back(circle);
	}

	m_healthIndicator.setFillColor(sf::Color{ 0, 255, 0, 100 });
	m_healthIndicator.setOrigin(65.0f, 65.0f);
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::seek(Tank t_playerTanks[], const int t_numberOfPlayers) const
{
	float shortestDistance{ 100000.0f }; // Impossibly high distance as an initialisation value
	int indexOfClosest{ -1 };

	// Loop to find the closest player
	for (int i = 0; i < t_numberOfPlayers; i++)
	{
		float distance = thor::length(t_playerTanks[i].getPosition() - m_tankBase.getPosition());
		if (t_playerTanks[i].getHealth() > 0.0f && distance < shortestDistance)
		{
			indexOfClosest = i;
			shortestDistance = thor::length(t_playerTanks[i].getPosition() - m_tankBase.getPosition());
		}
	}

	// Don't move if no player is left
	if (indexOfClosest < 0)
	{
		return { 0.0f, 0.0f };
	}

	return t_playerTanks[indexOfClosest].getPosition() - m_tankBase.getPosition();
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::collisionAvoidance()
{
	auto headingRadians = thor::toRadian(m_rotation);
	sf::Vector2f headingVector(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);

	m_ahead = m_tankBase.getPosition() + headingVector;
	m_halfAhead = m_tankBase.getPosition() + (headingVector * 0.5f);

	const sf::CircleShape mostThreatening = findMostThreateningObstacle();

	sf::Vector2f avoidance(0, 0);
	if (mostThreatening.getRadius() != 0.0)
	{		
		avoidance.x = m_ahead.x - mostThreatening.getPosition().x;
		avoidance.y = m_ahead.y - mostThreatening.getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
	}
	
	return avoidance;
}

////////////////////////////////////////////////////////////
const sf::CircleShape TankAi::findMostThreateningObstacle()
{
	sf::CircleShape mostThreatening{ 0.0f };

	for (sf::CircleShape circle : m_obstacles)
	{
		bool collides = MathUtility::lineIntersectsCircle(m_ahead, m_halfAhead, circle);
		if (collides && (mostThreatening.getRadius() == 0 || MathUtility::distance(m_tankBase.getPosition(), circle.getPosition())
			< MathUtility::distance(m_tankBase.getPosition(), mostThreatening.getPosition())))
		{
			mostThreatening = circle;
		}
	}

	float distance = MathUtility::distance(m_tankBase.getPosition() + m_ahead, mostThreatening.getPosition());

	if (distance > mostThreatening.getRadius() * 0.95f && distance < mostThreatening.getRadius() * 1.05f)
	{
		return sf::CircleShape{ 0.0f };
	}

	return mostThreatening;
}

////////////////////////////////////////////////////////////
void TankAi::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(95, 42, 100, 51);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(120, 3, 87, 37);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
}


////////////////////////////////////////////////////////////
void TankAi::updateMovement(double dt)
{
	double speed = thor::length(m_velocity);
	sf::Vector2f newPos(m_tankBase.getPosition().x + std::cos(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000),
		m_tankBase.getPosition().y + std::sin(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000));
	m_tankBase.setPosition(newPos.x, newPos.y);
	m_tankBase.setRotation(m_rotation);
	m_turret.setPosition(m_tankBase.getPosition());
	m_turret.setRotation(m_rotation);
}

////////////////////////////////////////////////////////////
bool TankAi::collidesWithPlayer(Tank const& playerTank) const
{
	if (m_active)
	{
		// Checks if the AI tank has collided with the player tank.
		if (CollisionDetector::collision(m_turret, playerTank.getTurret()) ||
			CollisionDetector::collision(m_tankBase, playerTank.getBase()))
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
const sf::Sprite TankAi::getBaseSprite() const
{
	return m_tankBase;
}

////////////////////////////////////////////////////////////
const sf::Sprite TankAi::getTurretSprite() const
{
	return m_turret;
}

////////////////////////////////////////////////////////////
void TankAi::takeDamage(float t_amount)
{
	m_healthPercent -= t_amount;

	if (m_healthPercent <= 0.0f)
	{
		m_active = false;

		m_tankBase.setPosition(-500.0f, -500.0f);
		m_turret.setPosition(-500.0f, -500.0f);
	}
}

////////////////////////////////////////////////////////////
const bool TankAi::isActive() const
{
	return m_active;
}

////////////////////////////////////////////////////////////
void TankAi::reset()
{
	m_active = true;
	m_tankBase.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 2.0f);
	m_turret.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 2.0f);
	m_healthPercent = 100.0f;
}

////////////////////////////////////////////////////////////
void TankAi::drawHealthIndicator(sf::RenderWindow& t_window)
{
	m_healthIndicator.setPosition(m_tankBase.getPosition());
	m_healthIndicator.setCompleteness(m_healthPercent / 100.0f);
	t_window.draw(m_healthIndicator);
}

////////////////////////////////////////////////////////////