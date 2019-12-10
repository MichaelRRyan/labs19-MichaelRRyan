/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/09/2019
/// Start time: 12:00 - Time taken: 53m - Added tank sprite, movement and rotation code
/// </summary>

#include "Game.h"
#include "ModeSelectScreen.h"


// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game() :
	m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default),
	//m_tank{ m_texture, m_wallSprites, m_targets },
	//m_controllerTank{ m_texture, m_wallSprites, m_targets },
	m_gameTimer{ 0.0 },
	m_targetsSpawned{ 0 },
	m_ROUND_TIME{ 60.0 },
	m_numberOfPlayers{ 0 },
	m_targetTimerShape{ 80.0f, 0.0f, 120u },
	m_menuScreen(m_guiTextures, m_menuBackground, m_font),
	m_modeSelectScreen(m_guiTextures, m_menuBackground, m_font),
	m_playerJoinScreen(m_guiTextures, m_menuBackground, m_font),
	m_gameState{ GameState::MenuScreen },
	m_TANK_POSITIONS{
		{m_TANK_OFFSET, m_TANK_OFFSET},
		{m_TANK_OFFSET, static_cast<float>(ScreenSize::s_height) - m_TANK_OFFSET},
		{static_cast<float>(ScreenSize::s_width) - m_TANK_OFFSET, m_TANK_OFFSET},
		{static_cast<float>(ScreenSize::s_width) - m_TANK_OFFSET, static_cast<float>(ScreenSize::s_height) - m_TANK_OFFSET} },
	m_TANK_TEXT_POSITIONS
	{
		{m_TANK_OFFSET, m_TANK_OFFSET},
		{m_TANK_OFFSET, static_cast<float>(ScreenSize::s_height) - m_TANK_OFFSET - 100.0f},
		{ static_cast<float>(ScreenSize::s_width) - m_TANK_OFFSET - 600.0f, m_TANK_OFFSET},
		{ static_cast<float>(ScreenSize::s_width) - m_TANK_OFFSET - 600.0f, static_cast<float>(ScreenSize::s_height) - m_TANK_OFFSET - 100.0f}
	},
	m_tanks{
		{m_texture, m_guiTextures, m_wallSprites, m_targets},
		{m_texture, m_guiTextures, m_wallSprites, m_targets},
		{m_texture, m_guiTextures, m_wallSprites, m_targets},
		{m_texture, m_guiTextures, m_wallSprites, m_targets} }
{
	m_window.setVerticalSyncEnabled(true);

	// Set the level number
	int currentLevel = 1;

	// Load the level data
	try
	{
		LevelLoader::load(currentLevel, m_level);
	}
	catch (std::exception &e)
	{
		std::cout << "Level load failed." << std::endl;
		std::cout << e.what() << std::endl;
		//throw(e);
	}

	loadTextures(); // Load the textures
	loadSounds();
	setupText(); // Load font and setup text

	// Setup the background
	m_bgSprite.setTexture(m_bgTexture);

	// Set the position for the tanks
	//m_tank.setPosition(m_level.m_tank.m_position);
	//m_controllerTank.setPosition({ m_level.m_tank.m_position.x - 200.0f, m_level.m_tank.m_position.y });

	// Generate the walls and targets
	generateWalls();
	generateTargets();

	// Setup the timer circle
	m_targetTimerShape.setFillColor(sf::Color{ 255, 0, 0, 150 });
	m_targetTimerShape.setOrigin(m_targetTimerShape.getRadius(), m_targetTimerShape.getRadius());

	// Connect the controllers
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_controllers[i].connect();
	}

	// Setup the menu
	m_menuScreen.setup();
	m_modeSelectScreen.setup();
	m_playerJoinScreen.setup();


	// PARTICLE TESTING TEMP
	/*m_tanks[0].setControlType(ControlType::Keyboard, ControlScheme::ArrowKeys);
	m_gameState = GameState::TargetPractice;
	m_numberOfPlayers++;*/
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= static_cast<sf::Int32>(MS_PER_UPDATE);
		}
		update(MS_PER_UPDATE);

		render();
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

////////////////////////////////////////////////////////////
void Game::processGameEvents(sf::Event& event)
{
	switch (m_gameState)
	{
	case GameState::MenuScreen:
		m_menuScreen.processEvents(event, m_gameState, m_window);
		break;
	case GameState::PlayerJoinScreen:
		m_playerJoinScreen.processEvents(event, m_gameState, m_tanks, m_numberOfPlayers);

		// If the gamestate is now mode select
		if (GameState::ModeSelect == m_gameState)
		{
			m_modeSelectScreen.updateLocked(m_numberOfPlayers);
		}
		break;
	case GameState::ModeSelect:
		m_modeSelectScreen.processEvents(event, m_gameState, m_numberOfPlayers);

		// If the gamestate is now target practice
		if (GameState::TargetPractice == m_gameState)
		{
			startTargetPractice();
		}
		// If the gamestate is now target practice
		else if (GameState::Versus == m_gameState)
		{
			startVersusGame();
		}

		break;
	case GameState::TargetPractice:
		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			if (m_tanks[i].getControlType() == ControlType::Keyboard)
			{
				m_tanks[i].processEvents(event);
			}
		}

		// check if the event is a a key button press
		if (sf::Event::KeyPressed == event.type)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_gameState = GameState::MenuScreen;
				break;
			case sf::Keyboard::Space:
				if (m_gameTimer == 0.0)
				{
					startTargetPractice();
				}
				break;
			}
		}
		break;
	case GameState::Versus:
		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			if (m_tanks[i].getControlType() == ControlType::Keyboard)
			{
				m_tanks[i].processEvents(event);
			}
		}

		// check if the event is a a key button press
		if (sf::Event::KeyPressed == event.type)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_gameState = GameState::MenuScreen;
				break;
			}
		}
		break;
	}
}

////////////////////////////////////////////////////////////
void Game::startTargetPractice()
{
	// Setup the tanks
	resetTanks();
	
	// Set the timers
	m_gameTimer = m_ROUND_TIME;
	m_spawnTimer.restart();
	m_clockTimer.restart();

	// Set text
	m_timerText.setString("Timer: " + std::to_string(static_cast<int>(ceil(m_gameTimer))));
	m_timerText.setCharacterSize(40u);
	m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
	m_timerText.setPosition(ScreenSize::s_width / 2, 30.0f);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_playerTexts[i].setPosition(m_TANK_TEXT_POSITIONS[i]);
	}

	// Set targets
	m_targetsSpawned = 0;
	for (Target& target : m_targets)
	{
		target.resetTarget(m_wallSprites);
	}
}

////////////////////////////////////////////////////////////
void Game::startVersusGame()
{
	resetTanks();

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_playerTexts[i].setPosition(m_TANK_TEXT_POSITIONS[i]);
	}

	m_gamePaused = false;
}

////////////////////////////////////////////////////////////
void Game::resetTanks()
{
	// Setup the tanks
	for (int i = 0; i < m_numberOfPlayers; i++)
	{
		int positionIndex = rand() % 4;
		bool positionTaken;

		while (true)
		{
			positionTaken = false;

			for (int j = 0; j < i; j++)
			{
				if (m_tanks[j].getPosition() == m_TANK_POSITIONS[positionIndex])
				{
					positionTaken = true;
					positionIndex = rand() % 4;
					break;
				}
			}

			if (!positionTaken)
			{
				break;
			}
		}

		m_tanks[i].setPosition(m_TANK_POSITIONS[positionIndex]);
		m_tanks[i].resetStats();
		m_tanks[i].resetRotation();
	}
}

////////////////////////////////////////////////////////////
void Game::generateWalls()
{
	sf::IntRect wallRect{ 5, 55, 34, 20 };

	// Create the walls
	for (ObstacleData const& obstacle : m_level.m_obstacles)
	{
		sf::Sprite sprite;
		sprite.setTexture(m_texture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(sprite);
	}
}

////////////////////////////////////////////////////////////
void Game::generateTargets()
{
	sf::IntRect targetRect{ 0, 0, 52, 52 };

	// Create the targets
	for (TargetData const& target : m_level.m_targets)
	{
		Target targetObject(m_texture, targetRect, target.m_position, target.m_randomOffset, target.m_rotation, target.m_durationSeconds);
		targetObject.resetTarget(m_wallSprites);
		m_targets.push_back(targetObject);
	}

	m_numberOfTargets = m_targets.size(); // Get the number of targets
}

////////////////////////////////////////////////////////////
void Game::setupText()
{
	// Load the font
	if (!m_font.loadFromFile("./resources/fonts/Disarmer-pvVD.ttf"))
	{
		std::string s("Error loading font file");
		throw std::exception(s.c_str());
	}

	// Setup the text objects
	m_timerText.setFont(m_font);
	m_timerText.setString("PRESS SPACE TO START");
	m_timerText.setCharacterSize(40u);
	m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
	m_timerText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 2.0f);
	m_timerText.setOutlineThickness(2.0f);
	m_timerText.setOutlineColor(sf::Color::Black);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_playerTexts[i].setFont(m_font);
		m_playerTexts[i].setCharacterSize(30u);
		m_playerTexts[i].setFillColor(sf::Color::White);
		m_playerTexts[i].setOutlineThickness(2.0f);
		m_playerTexts[i].setOutlineColor(sf::Color::Black);
	}

	m_bestScoreText.setFont(m_font);
	m_bestScoreText.setCharacterSize(35u);
	m_bestScoreText.setPosition(ScreenSize::s_width / 2, ScreenSize::s_height / 2);
	m_bestScoreText.setFillColor(sf::Color::White);
	m_bestScoreText.setOutlineThickness(2.0f);
	m_bestScoreText.setOutlineColor(sf::Color::Black);

	m_endGameText.setFont(m_font);
	m_endGameText.setCharacterSize(35u);
	m_endGameText.setPosition(ScreenSize::s_width / 2, ScreenSize::s_height / 2);
	m_endGameText.setFillColor(sf::Color::White);
	m_endGameText.setOutlineThickness(2.0f);
	m_endGameText.setOutlineColor(sf::Color::Black);
}

////////////////////////////////////////////////////////////
void Game::loadTextures()
{
	// Load tank sprite
	if (!m_texture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading spritesheet texture");
		throw std::exception(s.c_str());
	}

	// Load the background texture
	if (!m_bgTexture.loadFromFile(m_level.m_background.m_fileName))
	{
		std::string s("Error loading background texture");
		throw std::exception(s.c_str());
	}

	// Load the gui texture
	if (!m_guiTextures.loadFromFile("./resources/images/GUI.png"))
	{
		std::string s("Error loading GUI textures");
		throw std::exception(s.c_str());
	}

	if (!m_menuBackground.loadFromFile("./resources/images/menuBackground.png"))
	{
		std::string s("Error loading menu background texture");
		throw std::exception(s.c_str());
	}
}

void Game::loadSounds()
{
	if (!m_shotSoundBuffer.loadFromFile("./resources/audio/gunshot.wav"))
	{
		std::string s("Error loading tank shoot sound effect");
		throw std::exception(s.c_str());
	}

	if (!m_explosionSoundBuffer.loadFromFile("./resources/audio/explosion.wav"))
	{
		std::string s("Error loading tank explosion sound effect");
		throw std::exception(s.c_str());
	}

	if (!m_tankDriveSoundBuffer.loadFromFile("./resources/audio/tankDrive.wav"))
	{
		std::string s("Error loading tank drive sound effect");
		throw std::exception(s.c_str());
	}

	if (!m_tankTurretRotateSoundBuffer.loadFromFile("./resources/audio/tankTurretRotate.wav"))
	{
		std::string s("Error loading tank rotate sound effect");
		throw std::exception(s.c_str());
	}

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_tanks[i].setSounds(m_shotSoundBuffer, m_explosionSoundBuffer, m_tankDriveSoundBuffer, m_tankTurretRotateSoundBuffer);
	}
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	// Update the controllers
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_controllers[i].update();
	}

	switch (m_gameState)
	{
	case GameState::PlayerJoinScreen:
		m_playerJoinScreen.update(m_tanks, m_controllers, m_numberOfPlayers);
		break;
	case GameState::TargetPractice:
		updateTargetPractice(dt);
		break;
	case GameState::Versus:
		updateVersus(dt);
		break;
	}
}

////////////////////////////////////////////////////////////
void Game::updateTargetPractice(double dt)
{
	// If the round is still on
	if (m_gameTimer > 0.0)
	{
		m_timerText.setString("Timer: " + std::to_string(static_cast<int>(ceil(m_gameTimer)))); // Set the timer text string

		updatePlayers(dt); // update the players (tanks)

		// Update timer
		sf::Time timeSinceLastUpdate = m_clockTimer.restart();
		m_gameTimer -= timeSinceLastUpdate.asSeconds();

		// Spawn new target
		if (m_targets.size() > 0 && m_spawnTimer.getElapsedTime().asSeconds() > (m_ROUND_TIME - 5.0) / m_numberOfTargets)
		{
			spawnTarget();
		}

		// Update the targets
		updateTargets(timeSinceLastUpdate);

		// Update the player score text

		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			m_playerTexts[i].setString(std::to_string(m_tanks[i].getScore()));
			m_playerTexts[i].setPosition(m_tanks[i].getPosition().x, m_tanks[i].getPosition().y + 50.0f);
		}

		// If the round ends
		if (m_gameTimer < 0.0)
		{
			endRound();
		}
	}
}

////////////////////////////////////////////////////////////
void Game::updateVersus(double dt)
{
	if (!m_gamePaused)
	{
		int tanksAlive = 0;

		// update the first player if alive
		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			m_tanks[i].update(dt);

			if (m_tanks[i].getHealth() > 0.0f)
			{
				tanksAlive++;
			}

			// Check collisions with all other alive tanks
			for (int j = 0; j < m_numberOfPlayers; j++)
			{
				if (i != j) // Don't check collisions with ourselves
				{
					if (m_tanks[j].getHealth() > 0.0f)
					{
						m_tanks[i].checkTanktoTankCollisions(m_tanks[j]);
					}
				}
			}

			m_playerTexts[i].setString("player" + std::to_string(i + 1) + " HP: " + std::to_string(static_cast<int>(m_tanks[i].getHealth())) + "%");
		}

		if (tanksAlive < 2)
		{
			m_gamePaused = true;

			// Find the index of the last player alive
			int index = 0;
			for (int i = 0; i < m_numberOfPlayers; i++)
			{
				if (m_tanks[i].getHealth() > 0.0f)
				{
					index = i;
					break;
				}
			}

			m_endGameText.setString("GAME OVER! Player" + std::to_string(index + 1) + " has won!\nPress Escape to exit");
			m_endGameText.setOrigin(m_endGameText.getGlobalBounds().width / 2.0f, m_endGameText.getGlobalBounds().height / 2.0f);
		}
	}

	// update the particle systems even if the game is paused
	for (int i = 0; i < m_numberOfPlayers; i++)
	{
		m_tanks[i].updateParticleSys();
	}
}

////////////////////////////////////////////////////////////
void Game::updatePlayers(double dt)
{
	for (int i = 0; i < m_numberOfPlayers; i++)
	{
		// update the first player
		m_tanks[i].update(dt);

		if (m_tanks[i].checkBulletTargetCollisions()) // If a target is destroyed spawn a new one
		{
			spawnTarget();
		}
	}
}

////////////////////////////////////////////////////////////
void Game::updateTargets(sf::Time t_timeSinceLastUpdate)
{
	// Update all the targets
	for (Target& target : m_targets)
	{
		if (target.active())
		{
			target.update(t_timeSinceLastUpdate);
		}
	}

	// If all targets were hit, end the round
	if (m_targetsSpawned == m_numberOfTargets)
	{
		for (Target const & target : m_targets)
		{
			if (target.active())
			{
				return;
			}
		}
		m_gameTimer = -1.0;
	}
}

////////////////////////////////////////////////////////////
void Game::spawnTarget()
{
	if (m_targetsSpawned < m_numberOfTargets) // If not all targets have been spawned yet
	{
		m_targets.at(m_targetsSpawned).setActive(true);
		m_targetsSpawned++;
	}
	else // Display that no targets are left to spawn
	{
		std::cout << "Attempting to spawn a target out of range" << std::endl;
	}

	// Reset the spawn timer
	m_spawnTimer.restart();
}

////////////////////////////////////////////////////////////
void Game::endRound()
{
	// Display previous best score
	Stats newStats = RoundStatsSaver::returnNthBestScore(0);
	m_bestScoreText.setString("Previous Best :\nTargets hit: " + std::to_string(newStats.m_percentTargetsHit)
		+ "%\nAccuracy: " + std::to_string(newStats.m_accuracy)
		+ "%\nOverall Score: " + std::to_string(newStats.m_score));
	m_bestScoreText.setOrigin(m_bestScoreText.getGlobalBounds().width / 2.0f, 0.0f);

	// Reset the timer
	m_gameTimer = 0.0;

	// Set the text object strings
	m_timerText.setString("PRESS SPACE TO START");

	for (int i = 0; i < m_numberOfPlayers; i++)
	{
		m_playerTexts[i].setString("Player" + std::to_string(i + 1) + ":\n" + m_tanks[i].getStatistics());
	}

	// Move the timer text and rescale it
	m_timerText.setCharacterSize(40u);
	m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
	m_timerText.setPosition(ScreenSize::s_width / 2, ScreenSize::s_height / 2 - 100.0f);

	// Save the new stats
	try
	{
		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			// Player 1
			Stats stats;
			stats.m_score = m_tanks[i].getScore();
			stats.m_accuracy = m_tanks[i].getAccuracy();
			stats.m_percentTargetsHit = m_tanks[i].getPercentTargetsHit();
			RoundStatsSaver::saveRoundStats(stats);
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Stat saver failed." << std::endl;
		std::cout << e.what() << std::endl;
	}
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear();

	switch (m_gameState)
	{
	case GameState::MenuScreen:
		m_menuScreen.draw(m_window);
		break;
	case GameState::PlayerJoinScreen:
		m_playerJoinScreen.draw(m_window, m_numberOfPlayers);
		break;
	case GameState::ModeSelect:
		m_modeSelectScreen.draw(m_window);
		break;
	case GameState::TargetPractice:
		

		m_window.draw(m_bgSprite);

		// Draw all the obstacles
		for (sf::Sprite const& obstacle : m_wallSprites)
		{
			m_window.draw(obstacle);
		}

		// If the game round is still going, draw the tanks, and targets
		if (m_gameTimer > 0.0)
		{
			// Draw the targets
			for (Target const& target : m_targets)
			{
				if (target.active())
				{
					target.draw(m_window, m_targetTimerShape);
				}
			}

			// Draw the tanks
			for (int i = 0; i < m_numberOfPlayers; i++)
			{
				m_tanks[i].render(m_window);
			}
		}

		m_window.draw(m_timerText);
		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			m_window.draw(m_playerTexts[i]);
		}

		if (m_gameTimer <= 0.0)
		{
			m_window.draw(m_bestScoreText);
		}
		break;

	case GameState::Versus:
		m_window.clear();

		m_window.draw(m_bgSprite);

		// Draw all the obstacles
		for (sf::Sprite const& obstacle : m_wallSprites)
		{
			m_window.draw(obstacle);
		}

		// Draw the tanks if they're alive
		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			if (m_tanks[i].getHealth() > 0.0f)
			{
				m_tanks[i].drawHealthIndicator(m_window);
			}
		}

		// Draw the tanks if they're alive
		for (int i = 0; i < m_numberOfPlayers; i++)
		{
			m_tanks[i].render(m_window);
		}

		if (m_gamePaused)
		{
			m_window.draw(m_endGameText);
		}

		break;
	}


	m_window.display();
}