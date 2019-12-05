/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/09/2019
/// </summary>

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
#include "PlayerJoinScreen.h"

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
	/// @brief Sets up everything for a new round in target practice mode
	/// </summary>
	void startTargetPractice();

	/// <summary>
	/// @brief Sets up everything for a new game in versus mode
	/// </summary>
	void startVersusGame();

	/// <summary>
	/// @brief set the position of the tanks
	/// </summary>
	void resetTanks();

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

	/// <summary>
	/// @brief Load all the sounds
	/// </summary>
	void loadSounds();


	/////////////////////////////////////////////////////////////////////
	/// Protected Data Members
	////////////////////////////////////////////////////////////////////

	/// VISUALS
	// main window
	sf::RenderWindow m_window;

	//Font and text
	sf::Font m_font;

	sf::Text m_timerText;
	sf::Text m_bestScoreText;
	sf::Text m_playerTexts[MAX_PLAYERS];

	// Texures and sprites
	sf::Texture m_texture;
	sf::Texture m_bgTexture;
	sf::Texture m_guiTextures;
	sf::Texture m_menuBackground;

	sf::Sprite m_bgSprite;
	std::vector<sf::Sprite> m_wallSprites; // Wall sprites

	std::vector<Target> m_targets;
	CircularSectorShape m_targetTimerShape;

	// Instance of a tank object as a player controlled object
	//Tank m_tank;
	//Tank m_controllerTank;

	XBox360Controller m_controllers[MAX_PLAYERS];

	Tank m_tanks[MAX_PLAYERS];

	// Audio
	sf::SoundBuffer m_shotSoundBuffer;
	sf::SoundBuffer m_explosionSoundBuffer;

	// Load the game level data
	LevelData m_level;

	// Timers
	const double m_ROUND_TIME;
	double m_gameTimer;

	sf::Clock m_clockTimer;
	sf::Clock m_spawnTimer;

	const float m_TANK_OFFSET{ 100.0f }; // Offset in from the screen boundaries that the tank will spawn
	const sf::Vector2f m_TANK_POSITIONS[4]; // Corner positions the tank spawns in

	int m_targetsSpawned; // Number of targets spawned in a round
	int m_numberOfTargets; // Number targets loaded from a file

	int m_numberOfPlayers;

	MenuScreen m_menuScreen;
	ModeSelectScreen m_modeSelectScreen;
	PlayerJoinScreen m_playerJoinScreen;

	GameState m_gameState; // Current state the game is in
	GameState m_previousGameState;
};
