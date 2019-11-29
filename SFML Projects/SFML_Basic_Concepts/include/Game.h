/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/09/2019
/// </summary>

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ScreenSize.h"
#include "LevelLoader.h"
#include "RoundStatsSaver.h"
#include "Tank.h"
#include "XBox360Controller.h"
#include "Target.h"
#include "CircularSectorShape.h"
#include "MenuScreen.h"
#include "GameState.h"
#include "ModeSelectScreen.h"

class Game
{
public:
	/// <summary>
	/// @brief Default constructor that initialises the SFML window, 
	///   and sets vertical sync enabled. 
	/// </summary>
	Game();

	/// <summary>
	/// @brief the main game loop.
	/// 
	/// A complete loop involves processing SFML events, updating and drawing all game objects.
	/// The actual elapsed time for a single game loop results (lag) is stored. If this value is 
	///  greater than the notional time for one loop (MS_PER_UPDATE), then additional updates will be 
	///  performed until the lag is less than the notional time for one loop.
	/// The target is one update and one render cycle per game loop, but slower PCs may 
	///  perform more update than render operations in one loop.
	/// </summary>
	void run();

protected:
	/// <summary>
	/// @brief Placeholder to perform updates to all game objects.
	/// </summary>
	/// <param name="time">update delta time</param>
	void update(double dt);

	/// <summary>
	/// @brief Updates all the player objects
	/// </summary>
	/// <param name="dt">update delta time</param>
	void updatePlayers(double dt);

	/// <summary>
	/// @brief Updates all the targets
	/// </summary>
	/// <param name="t_timeSinceLastUpdate">time since the last update</param>
	void updateTargets(sf::Time t_timeSinceLastUpdate);

	/// <summary>
	/// @brief Attempts to spawn the next target on screen
	/// </summary>
	void spawnTarget();

	/// <summary>
	/// @brief Handles all end of round functionality
	/// </summary>
	void endRound();

	/// <summary>
	/// @brief Sets up everything for a new round
	/// </summary>
	void startRound();

	/// <summary>
	/// @brief Draws the background and foreground game objects in the SFML window.
	/// The render window is always cleared to black before anything is drawn.
	/// </summary>
	void render();

	/// <summary>
	/// @brief Checks for events.
	/// Allows window to function and exit. 
	/// Events are passed on to the Game::processGameEvents() method.
	/// </summary>	
	void processEvents();

	/// <summary>
	/// @brief Handles all user input.
	/// </summary>
	/// <param name="event">system event</param>
	void processGameEvents(sf::Event&);

	/// <summary>
	/// @brief Creates the wall sprites and loads them into a vector.
	/// Note that sf::Sprite is considered a light weight class, so 
	///  storing copies (instead of pointers to sf::Sprite) in std::vector is acceptable.
	/// </summary>
	void generateWalls();

	/// <summary>
	/// @brief Creates the target objects and loads them into a vector.
	/// </summary>
	void generateTargets();

	/// <summary>
	/// @brief setup and load font and text
	/// </summary>
	void setupText();

	/// <summary>
	/// @brief Load all the textures
	/// </summary>
	void loadTextures();

	// main window
	sf::RenderWindow m_window;

	sf::Font m_font;
	sf::Text m_timerText;
	sf::Text m_playerOneText;
	sf::Text m_playerTwoText;
	sf::Text m_bestScoreText;

	// Sprites
	sf::Texture m_texture;
	sf::Texture m_bgTexture;
	sf::Texture m_spriteSheetTexture;

	sf::Sprite m_sprite;
	sf::Sprite m_tankTurret;
	sf::Sprite m_bgSprite;
	std::vector<sf::Sprite> m_obstacles;

	// Wall sprites
	std::vector<sf::Sprite> m_wallSprites;
	std::vector<Target> m_targets;

	// Instance of a tank object as a player controlled object
	Tank m_tank;
	Tank m_controllerTank;

	XBox360Controller m_controller;

	// Load the game level data
	LevelData m_level;

	// Timers
	const double m_ROUND_TIME;

	sf::Clock m_clockTimer;
	sf::Clock m_spawnTimer;
	double m_gameTimer;

	const float m_TANK_OFFSET{ 100.0f };
	const sf::Vector2f m_TANK_POSITIONS[4]{
		{m_TANK_OFFSET, m_TANK_OFFSET},
		{m_TANK_OFFSET, static_cast<float>(ScreenSize::s_height) - m_TANK_OFFSET},
		{static_cast<float>(ScreenSize::s_width) - m_TANK_OFFSET, m_TANK_OFFSET},
		{static_cast<float>(ScreenSize::s_width) - m_TANK_OFFSET, static_cast<float>(ScreenSize::s_height) - m_TANK_OFFSET} };

	//sf::CircleShape m_timerCircle;
	int m_targetsSpawned;
	int m_numberOfTargets;

	CircularSectorShape m_tempShape;

	sf::Texture m_guiTextures;
	MenuScreen m_menuScreen;
	ModeSelectScreen m_modeSelectScreen;

	GameState m_gameState;
};
