#pragma once
#include <SFML/Graphics.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "XBox360Controller.h"
#include "Bullet.h"
#include "Target.h"

/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:
	/// <summary>
	/// @brief Constructor that stores drawable state (texture, sprite) for the tank.
	/// Stores references to the texture and container of wall sprites. 
	/// Creates sprites for the tank base and turret from the supplied texture.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="texture">A reference to the container of wall sprites</param>  
	Tank(sf::Texture const& t_texture, std::vector<sf::Sprite>& t_wallSprites, std::vector<Target>& t_targets);
	void update(double dt);
	void render(sf::RenderWindow& window);
	void setPosition(sf::Vector2f t_position);

	/// <summary>
	/// @brief Returns the tank's score value
	/// </summary>
	/// <returns>Tank's score</returns>
	int getScore();

	/// <summary>
	/// @brief Increases the speed by 1, max speed is capped at 100.
	/// 
	/// </summary>
	void increaseSpeed();

	/// <summary>
	/// @brief Decreases the speed by 1, min speed is capped at -100.
	/// 
	/// </summary>
	void decreaseSpeed();

	/// <summary>
	/// @brief Increases the rotation by 1 degree, wraps to 0 degrees after 359.
	/// 
	/// </summary>
	void increaseRotation();

	/// <summary>
	/// @brief Decreases the rotation by 1 degree, wraps to 359 degrees after 0.
	/// 
	/// </summary>
	void decreaseRotation();

	/// <summary>
	/// @brief Increases the rotation of the turret by the rotation speed
	/// </summary>
	void increaseTurretRotation();

	/// <summary>
	/// @brief Decreases the rotation of the turret by the rotation speed
	/// </summary>
	void decreaseTurretRotation();

	/// <summary>
	/// @brief Enable the centring of the turret
	/// </summary>
	void toggleCentring();

	/// <summary>
	/// @brief Centre the turret with the tank body
	/// </summary>
	void centreTurret();

	/// <summary>
	/// @brief set the previous variables
	/// </summary>
	void setPrevious();

	/// <summary>
	/// @brief handles key input from the player to move the tank
	/// </summary>
	void handleKeyInput();

	/// <summary>
	/// @brief handles controller input from the player to move the tank
	/// </summary>
	void handleControllerInput(XBox360Controller t_controller);

	/// <summary>
	/// @brief Checks for collisions between the tank and the walls.
	/// 
	/// </summary>
	/// <returns>True if collision detected between tank and wall.</returns>
	bool checkWallCollision();

	/// <summary>
	/// @brief Checks for collisions between the bullet and the walls.
	/// 
	/// </summary>
	void checkBulletWallCollisions();

	/// <summary>
	/// @brief Checks for collisions between the bullet and the targets.
	/// 
	/// </summary>
	bool checkBulletTargetCollisions();

	/// <summary>
	/// @brief Stops the tank if moving and applies a small increase in speed in the opposite direction of travel.
	/// If the tank speed is currently 0, the rotation is set to a value that is less than the previous rotation value
	///  (scenario: tank is stopped and rotates into a wall, so it gets rotated towards the opposite direction).
	/// If the tank is moving, further rotations are disabled and the previous tank position is restored.
	/// The tank speed is adjusted so that it will travel slowly in the opposite direction. The tank rotation 
	///  is also adjusted as above if necessary (scenario: tank is both moving and rotating, upon wall collision it's 
	///  speed is reversed but with a smaller magnitude, while it is rotated in the opposite direction of it's 
	///  pre-collision rotation).
	/// </summary>
	void deflect();

	void adjustRotation();

private:
	void initSprites();
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	sf::Texture const& m_texture;

	sf::Vector2f m_previousPosition{ 0.0f, 0.0f };

	// A reference to the container of wall sprites.
	std::vector<sf::Sprite>& m_wallSprites;
	std::vector<Target>& m_targets;

	// The tank speed.
	const double FRICTION;
	const double SPEED_LIMIT;
	const double ACCELERATION;
	const double TURN_SPEED;
	const float FIRE_DELAY{ 1.0f };

	double m_previousSpeed{ 0.0 };
	double m_speed{ 0.0 };

	// The current rotation as applied to tank base.
	double m_previousRotation;
	double m_rotation{ 0.0 };
	double m_previousTurretRotation{ 0.0 };
	double m_turretRotation{ 0.0 };

	float m_fireTimer{ 0 };

	bool m_enableRotation{ true };
	bool m_centringTurret{ false };
	bool m_centringClockwise{ false };

	Bullet* m_bullet;

	// Score
	int m_score;
};
