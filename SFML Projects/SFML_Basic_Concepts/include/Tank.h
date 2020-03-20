#ifndef TANK_H
#define TANK_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 11/10/19
/// </summary>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "XBox360Controller.h"
#include "Bullet.h"
#include "Target.h"
#include "Thor/Math.hpp"
#include "GameConfig.h"
#include "CircularSectorShape.h"
#include "Thor/Particles.hpp"
#include "Thor/Math.hpp"
#include "Thor/Animations.hpp"
#include "ProjectilePool.h"
#include "TankAI.h"
#include "AssetManager.h"

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
	Tank(std::vector<sf::Sprite>& t_wallSprites, std::vector<Target>& t_targets);

	/// <summary>
	/// @brief update the tank
	/// </summary>
	/// <param name="dt"></param>
	void update(double dt, TankAi & t_tankAI);

	/// <summary>
	/// @brief Update the particle systems
	/// </summary>
	void updateParticleSys();

	/// <summary>
	/// @Draws the tank and it's particle system
	/// </summary>
	/// <param name="window"></param>
	void render(sf::RenderWindow& window);

	/// <summary>
	/// @brief Set the position of the tank body and turret
	/// </summary>
	/// <param name="t_position"></param>
	void setPosition(sf::Vector2f t_position);

	/// <summary>
	/// @brief Returns the tank's score value
	/// </summary>
	/// <returns>Tank's score</returns>
	int getScore();

	/// <summary>
	/// @brief Returns the tank's targets hit value
	/// </summary>
	/// <returns>Tank's target hit percent</returns>
	int getPercentTargetsHit();

	/// <summary>
	/// @brief Returns the tank's accuracy value
	/// </summary>
	/// <returns>Tank's accuracy</returns>
	int getAccuracy();

	/// <summary>
	/// @brief Sets the tank's score value
	/// </summary>
	void resetStats();

	/// <summary>
	/// @brief Reset the rotation on the tank base and turret
	/// </summary>
	void resetRotation();

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
	/// @brief Increases the speed  the inputted percent of the acceleration value, max speed is capped at 100.
	/// 
	/// </summary>
	void increaseSpeed(float t_percent);

	/// <summary>
	/// @brief Decreases the speed by the inputted percent of the acceleration value, min speed is capped at -100.
	/// 
	/// </summary>
	void decreaseSpeed(float t_percent);


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
	/// @brief takes input for the arrow key configuration of the input
	/// </summary>
	void arrowKeysInput();

	/// <summary>
	/// @brief takes input for the YGHJ key configuration of the input
	/// </summary>
	void YGHJKeysInput();

	/// <summary>
	/// @brief handles controller input from the player to move the tank
	/// </summary>
	void handleControllerInput();

	/// <summary>
	/// @brief handles controller turret rotation
	/// </summary>
	void handleControllerTurretRotation(float t_inputDirection);

	/// <summary>
	/// @brief handles left stick input
	/// </summary>
	sf::Vector2f getControllerLSInput();

	/// <summary>
	/// @brief handles right stick input
	/// </summary>
	sf::Vector2f getControllerRSInput();

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

	/// <summary>
	/// @brief adjust the rotation upon collision
	/// </summary>
	void adjustRotation();

	/// <summary>
	/// @brief check if two tanks have collided
	/// </summary>
	/// <param name="t_tank">tank to collide with</param>
	void checkTanktoTankCollisions(Tank &t_tank);

	/// <summary>
	/// @brief removes from health the value given, then checks if dead and create particles and play sound if so
	/// </summary>
	/// <param name="t_amount">damage amount</param>
	void takeDamage(float t_amount);

	/// <summary>
	/// Returns the tanks health percentage
	/// </summary>
	/// <returns></returns>
	float getHealth();

	/// <summary>
	/// Return a string of the player statistics
	/// </summary>
	/// <returns></returns>
	std::string getStatistics();

	/// <summary>
	/// Return the tank base sprite
	/// </summary>
	/// <returns></returns>
	sf::Sprite getBase() const;

	/// <summary>
	/// Return the tank turret sprite
	/// </summary>
	/// <returns></returns>
	sf::Sprite getTurret() const;

	/// <summary>
	/// @brief set the sound buffer for the shot sound effect
	/// </summary>
	/// <param name="t_shotSoundBuffer">Shot sound buffer</param>
	void setSounds(sf::SoundBuffer const& t_shotSoundBuffer, sf::SoundBuffer const& t_explosionSoundBuffer, sf::SoundBuffer const& t_driveSoundBuffer,
					sf::SoundBuffer const& t_turretRotateSound);

	/// <summary>
	/// @brief Fires a bullet from the tank
	/// </summary>
	void fireBullet();

	/// <summary>
	/// @brief sets the type of controls the tank will use
	/// </summary>
	/// <param name="t_controlType"></param>
	/// <param name="t_controlScheme"></param>
	void setControlType(ControlType t_controlType, KeyConfiguration t_controlScheme = KeyConfiguration::None);

	/// <summary>
	/// @brief Returns the control type the tank object is in (e.g. controller, keyboard)
	/// </summary>
	/// <returns>Control type</returns>
	ControlType getControlType();

	/// <summary>
	/// @brief sets the pointer to the controller
	/// </summary>
	/// <param name="t_ptrController">Controller pointer</param>
	void setControllerPtr(XBox360Controller* t_ptrController);

	/// <summary>
	/// @brief return the controller pointer's joystick index. Returns -1 if nullptr
	/// </summary>
	/// <returns>Controller pointer's joystick index</returns>
	int getJoystickIndex();

	/// <summary>
	/// @brief Process player input events
	/// </summary>
	/// <param name="t_event">Player input</param>
	void processEvents(sf::Event t_event);

	/// <summary>
	/// @brief returns the position of the tank body
	/// </summary>
	/// <returns>tank position</returns>
	sf::Vector2f getPosition() const;

	/// <summary>
	/// @brief Draws the health indicator
	/// </summary>
	/// <param name="t_window"></param>
	void drawHealthIndicator(sf::RenderWindow &t_window);

	/// <summary>
	/// @brief enable or disable the turret depending on the input
	/// </summary>
	/// <param name="t_value">Enabled or not</param>
	void setTurretEnabled(bool t_value);

private:
	/// <summary>
	/// @brief Sets up all the sprites
	/// </summary>
	void initSprites();

	/// <summary>
	/// Setup the particle systems with the passed in texture file
	/// </summary>
	/// <param name="t_guiTexture">GUI textures</param>
	void setupParticleSystems(sf::Texture const& t_guiTexture);

	/// <summary>
	/// @brief Clamps t_value to a minimum and maximum value
	/// </summary>
	/// <param name="t_value">value to clamp</param>
	/// <param name="t_minValue">minimum value</param>
	/// <param name="t_maxValue">maximum value</param>
	void clamp(float& t_value, float const t_minValue, float const t_maxValue);

	/// <summary>
	/// @brief Generates a request to fire a projectile.
	/// A projectile will be fired from the tip of the turret only if time limit between shots
	///  has expired. Fire requests are not queued.
	/// </summary>
	void requestFire();


	// Private data members

	// Constants
	const float FRICTION;
	const float SPEED_LIMIT;
	const float ACCELERATION;
	const float TURN_SPEED;
	const float FIRE_DELAY;
	const float m_BULLET_DAMAGE;

	static int const s_TIME_BETWEEN_SHOTS{ 800 };

	// Tank sprites and textures
	sf::Texture const& m_texture;
	sf::Sprite m_baseSprite;
	sf::Sprite m_turretSprite;
	
	// Tank sounds
	sf::Sound m_shotSound;
	sf::Sound m_explosionSound;
	sf::Sound m_driveSound;
	sf::Sound m_turretRotateSound;

	// A references to targets and walls
	std::vector<sf::Sprite>& m_wallSprites;
	std::vector<Target>& m_targets;

	// Previous variables for collisions
	sf::Vector2f m_previousPosition{ 0.0f, 0.0f };
	float m_previousSpeed{ 0.0 };
	float m_previousRotation;
	float m_previousTurretRotation{ 0.0 };

	// Speed value
	float m_speed{ 0.0 };

	// The current rotation as applied to tank base
	float m_rotation{ 0.0 };
	
	// The current rotation as applied to tank turret
	float m_turretRotation{ 0.0 };

	// Clock to find the time until able to fire again
	sf::Clock m_fireClock; 

	// Time until able to fire again (Effected by time but used a float for calculations)
	float m_fireTimer{ 0 };

	// Projectile
	bool m_fireRequested;
	int m_shootTimer;
	bool m_turretEnabled;

	// Turret centering and collision bools
	bool m_enableRotation{ true };
	bool m_centringTurret{ false };
	bool m_centringClockwise{ false };
	bool m_moving;

	// Pointer to hold a bullet upon fire
	// Only one as there will never be an instance in which the bullet is still alive after 1 second (fire rate)
	Bullet* m_bullet; 

	ProjectilePool m_pool;

	// Statistics
	int m_targetsHit;
	int m_score;
	int m_bulletsFired;
	float m_healthPercent;

	// Visual indicator for tank health
	CircularSectorShape m_healthIndicator;

	// Controller Support
	const float CONTROLLER_ANALOG_DEADZONE{ 25.0f };
	XBox360Controller *m_ptrController;

	// Control configurations
	ControlType m_controlType;
	KeyConfiguration m_controlScheme;

	// Particle systems
	sf::Clock m_particleClock;

	thor::UniversalEmitter m_smokeEmitter;
	thor::UniversalEmitter m_explosionEmitter;

	thor::ParticleSystem m_smokePartSys;
	thor::ParticleSystem m_explosionPartSys;
};

#endif // !TANK_H