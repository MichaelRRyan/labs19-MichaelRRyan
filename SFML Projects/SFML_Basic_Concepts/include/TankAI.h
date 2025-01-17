#pragma once
#include "MathUtility.h"
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>
#include <queue>
#include "CollisionDetector.h"
#include "ScreenSize.h"
#include "CircularSectorShape.h"
#include "GameConfig.h"
#include "ProjectilePool.h"
#include "AssetManager.h"
#include "Raycast.h"

class Tank; // forward reference

class TankAi
{
public:
	/// <summary>
	/// @brief Constructor that stores a reference to the obstacle container.
	/// Initialises steering behaviour to seek (player) mode, sets the AI tank position and
	///  initialises the steering vector to (0,0) meaning zero force magnitude.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="wallSprites">A reference to the container of wall sprites</param>
	TankAi(std::vector<sf::Sprite> & wallSprites);

	/// <summary>
	/// @brief Steers the AI tank towards the player tank avoiding obstacles along the way.
	/// Gets a vector to the player tank and sets steering and velocity vectors towards
	/// the player if current behaviour is seek. If behaviour is stop, the velocity vector
	/// is set to 0. Then compute the correct rotation angle to point towards the player tank. 
	/// If the distance to the player tank is < MAX_SEE_AHEAD, then the behaviour is changed from seek to stop.
	/// Finally, recalculate the new position of the tank base and turret sprites.
	/// </summary>
	/// <param name="playerTank">A reference to the player tank</param>
	/// <param name="dt">update delta time</param>
	void update(Tank playerTanks[], const int t_numberOfPlayers, double dt);

	/// <summary>
	/// @brief Draws the tank base and turret.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow & window);

	/// <summary>
	/// @brief Initialises the obstacle container and sets the tank base/turret sprites to the specified position.
	/// <param name="position">An x,y position</param>
	/// </summary>
	void init(sf::Vector2f position);

	/// <summary>
	/// @brief Checks for collision between the AI and player tanks.
	///
	/// </summary>
	/// <param name="player">The player tank instance</param>
	/// <returns>True if collision detected between AI and player tanks.</returns>
	bool collidesWithPlayer(Tank const& playerTank) const;

	const sf::Sprite getBaseSprite() const;
	const sf::Sprite getTurretSprite() const;

	/// <summary>
	/// @brief removes from health the value given, then checks if dead
	/// </summary>
	/// <param name="t_amount">damage amount</param>
	void takeDamage(float t_amount);

	/// <summary>
	/// Returns true if the tank is active (alive)
	/// </summary>
	/// <returns></returns>
	const bool isActive() const;

	/// <summary>
	/// Takes a value to set the active bool to
	/// </summary>
	/// <param name="t_active"></param>
	void reset();

	void drawHealthIndicator(sf::RenderWindow& t_window);

	enum class AiType
	{
		AI_ID_NONE,
		AI_ID_SEEK_SHOOT_AT_PLAYER
	};

	enum class AIState
	{
		PatrolMap,
		AttackPlayer
	};

private:
	void initSprites();

	void updateMovement(double dt);

	sf::Vector2f seek(Tank t_playerTanks[], const int t_numberOfPlayers) const;

	sf::Vector2f collisionAvoidance();

	const sf::CircleShape findMostThreateningObstacle();

	void lookForPlayer(Tank playerTanks[], const int t_numberOfPlayers);

	void pickNewPatrolLocation();

	void updateVisionCone(float dt);

	void updateScanAngle(float dt);

	void requestFire();

	// A reference to the sprite sheet texture.
	sf::Texture const & m_texture;

	// A sprite for the tank base.
	sf::Sprite m_tankBase;

	// A sprite for the turret
	sf::Sprite m_turret;

	// A reference to the container of wall sprites.
	std::vector<sf::Sprite> & m_wallSprites;

	// The current rotation as applied to tank base and turret.
	double m_rotation{ 0.0 };

	// Current velocity.
	sf::Vector2f m_velocity;

	// Steering vector.
	sf::Vector2f m_steering;

	// The ahead vector.
	sf::Vector2f m_ahead;

	// The half-ahead vector.
	sf::Vector2f m_halfAhead;

	// The maximum see ahead range.
	static float constexpr MAX_SEE_AHEAD{ 50.0f };

	// The maximum avoidance turn rate.
	static float constexpr MAX_AVOID_FORCE{ 50.0f };

	// 
	static float constexpr MAX_FORCE{ 10.0f };

	// The maximum speed for this tank.
	float MAX_SPEED = 50.0f;

	// A container of circles that represent the obstacles to avoid.
	std::vector<sf::CircleShape> m_obstacles;
	std::vector<sf::Vector2f> m_wallLines;

	enum class AiBehaviour
	{
		SEEK_PLAYER,
		STOP,
		RETREAT
	} m_aiBehaviour;

	const float MASS{ 5.0f };

	bool m_active;

	float m_healthPercent;

	CircularSectorShape m_healthIndicator;

	AIState m_state;

	static int const s_visionLinePoints{ 51 };
	sf::VertexArray m_visionCone{ sf::TriangleFan, s_visionLinePoints };
	sf::Color m_coneColour;

	const float m_VISION_CONE_SPREAD;
	const float m_VISION_CONE_RADIUS;

	float m_currentVisionRadius;

	sf::Vector2f m_goalLocation;

	float m_scanAngle;

	static int const s_TIME_BETWEEN_SHOTS{ 1500 };
	const float m_BULLET_DAMAGE;

	ProjectilePool m_pool;
	bool m_fireRequested;
	int m_shootTimer;

	bool m_enemyWithinRange;
};

#include "Tank.h"