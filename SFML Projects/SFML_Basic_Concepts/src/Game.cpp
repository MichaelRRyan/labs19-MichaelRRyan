/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/09/2019
/// Start time: 12:00 - Time taken: 53m - Added tank sprite, movement and rotation code
/// </summary>

#include "Game.h"


// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game() :
	m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default),
	m_tank{ m_texture, m_wallSprites, m_targets },
	m_controllerTank{ m_texture, m_wallSprites, m_targets },
	m_gameTimer{ 0.0 },
	m_targetsSpawned{ 0 },
	m_ROUND_TIME{ 60.0 },
	m_tempShape{ 50.0f },
	m_menuScreen(m_guiTextures, m_font),
	m_gameState{ GameState::MenuScreen }
{
	m_window.setVerticalSyncEnabled(true);

	if (!m_font.loadFromFile("./resources/fonts/arial.ttf"))
	{
		std::string s("Error loading font file");
		throw std::exception(s.c_str());
	}

	m_timerText.setFont(m_font);
	m_timerText.setString("PRESS SPACE TO START");
	m_timerText.setCharacterSize(40u);
	m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
	m_timerText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 2.0f);

	m_playerOneScoreText.setFont(m_font);
	m_playerOneScoreText.setCharacterSize(35u);
	m_playerOneScoreText.setFillColor(sf::Color::Black);

	m_playerTwoScoreText.setFont(m_font);
	m_playerTwoScoreText.setCharacterSize(35u);
	m_playerTwoScoreText.setPosition(ScreenSize::s_width - 400, 0.0f);
	m_playerTwoScoreText.setFillColor(sf::Color::Black);

	m_bestScoreText.setFont(m_font);
	m_bestScoreText.setCharacterSize(35u);
	m_bestScoreText.setPosition(ScreenSize::s_width / 2, ScreenSize::s_height / 2 + 100);
	m_bestScoreText.setFillColor(sf::Color::Black);
	
	
	int currentLevel = 1;

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

	// Load tank sprite
	if (!m_texture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading spritesheet texture");
		throw std::exception(s.c_str());
	}

	if (!m_bgTexture.loadFromFile(m_level.m_background.m_fileName))
	{
		std::string s("Error loading background texture");
		throw std::exception(s.c_str());
	}

	if (!m_guiTextures.loadFromFile("./resources/images/GUI.png"))
	{
		std::string s("Error loading GUI textures");
		throw std::exception(s.c_str());
	}

	m_bgSprite.setTexture(m_bgTexture);
	m_tank.setPosition(m_level.m_tank.m_position);
	m_controllerTank.setPosition({ m_level.m_tank.m_position.x - 200.0f, m_level.m_tank.m_position.y });

	generateWalls();
	generateTargets();

	m_timerCircle.setFillColor(sf::Color{ 255, 0, 0, 150 });

	m_controller.connect();

	m_menuScreen.setup();
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
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		case sf::Keyboard::S:
			m_tank.toggleCentring();
			break;
		case sf::Keyboard::Space:
			if (m_gameTimer == 0.0)
			{
				startRound();
			}
			break;
		}
	}

	m_menuScreen.processEvents(event, m_gameState);
}

////////////////////////////////////////////////////////////
void Game::startRound()
{
	int tankOnePosition = rand() % 4;
	m_tank.setPosition(m_TANK_POSITIONS[tankOnePosition]);
	m_tank.resetScore();
	m_tank.resetRotation();

	int tankTwoPosition = rand() % 4;

	while (tankTwoPosition == tankOnePosition)
	{
		tankTwoPosition = rand() % 4;
	}

	//m_controllerTank.setPosition(m_TANK_POSITIONS[tankTwoPosition]);
	m_controllerTank.setPosition({ ScreenSize::s_width / 2.0f, ScreenSize::s_height / 2.0f });
	m_controllerTank.resetScore();
	m_controllerTank.resetRotation();

	// Set the timers
	m_gameTimer = m_ROUND_TIME;
	m_spawnTimer.restart();
	m_clockTimer.restart();

	// Set text
	m_timerText.setString("Timer: " + std::to_string(static_cast<int>(ceil(m_gameTimer))));
	m_timerText.setCharacterSize(40u);
	m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
	m_timerText.setPosition(ScreenSize::s_width / 2, 30.0f);

	m_playerOneScoreText.setPosition(0.0f, 0.0f);
	m_playerTwoScoreText.setPosition(ScreenSize::s_width - 400, 0.0f);

	// Set targets
	m_targetsSpawned = 0;
	for (Target& target : m_targets)
	{
		target.resetTarget(m_wallSprites);
	}
}

////////////////////////////////////////////////////////////
void Game::generateWalls()
{
	sf::IntRect wallRect{ 2, 129, 33, 23 };

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

void Game::generateTargets()
{
	sf::IntRect targetRect{ 64, 107, 105, 47 };

	// Create the walls
	for (TargetData const& target : m_level.m_targets)
	{
		Target targetObject(m_texture, targetRect, target.m_position, target.m_randomOffset, target.m_rotation, target.m_durationSeconds);
		targetObject.resetTarget(m_wallSprites);
		m_targets.push_back(targetObject);
	}

	m_numberOfTargets = m_targets.size();
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	m_controller.update();

	if (GameState::Gameplay == m_gameState)
	{
		if (m_gameTimer > 0.0)
		{
			m_timerText.setString("Timer: " + std::to_string(static_cast<int>(ceil(m_gameTimer))));

			updatePlayers(dt);

			// Timer
			sf::Time timeSinceLastUpdate = m_clockTimer.restart();
			m_gameTimer -= timeSinceLastUpdate.asSeconds();

			// Spawn new target
			if (m_targets.size() > 0 && m_spawnTimer.getElapsedTime().asSeconds() > (m_ROUND_TIME - 5.0) / m_numberOfTargets)
			{
				spawnTarget();
			}

			updateTargets(timeSinceLastUpdate);

			m_playerOneScoreText.setString("Player1's Score: " + std::to_string(m_tank.getScore()));
			m_playerTwoScoreText.setString("Player2's Score: " + std::to_string(m_controllerTank.getScore()));

			if (m_gameTimer < 0.0)
			{
				endRound();
			}
		}
	}
}

////////////////////////////////////////////////////////////
void Game::updatePlayers(double dt)
{
	m_tank.setPrevious();
	m_tank.handleKeyInput();
	m_tank.update(dt);

	if (m_tank.checkBulletTargetCollisions())
	{
		spawnTarget();
	}

	m_controllerTank.setPrevious();
	m_controllerTank.handleControllerInput(m_controller);
	m_controllerTank.update(dt);

	if (m_controllerTank.checkBulletTargetCollisions())
	{
		spawnTarget();
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
	if (m_targetsSpawned < m_numberOfTargets)
	{
		m_targets[m_targetsSpawned].setActive(true);
		m_targetsSpawned++;
	}
	else
	{
		std::cout << "Attempting to spawn a target out of range" << std::endl;
	}

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
	m_playerOneScoreText.setString("Player1:\n" + m_tank.getStatistics());
	m_playerTwoScoreText.setString("Player2:\n" + m_controllerTank.getStatistics());

	// Set the text positions
	m_playerOneScoreText.setPosition(100.0f, 100.0f);
	m_playerTwoScoreText.setPosition(ScreenSize::s_width - 500, 100.0f);

	// Move the timer text and rescale it
	m_timerText.setCharacterSize(40u);
	m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
	m_timerText.setPosition(ScreenSize::s_width / 2, ScreenSize::s_height / 2);

	// Save the new stats
	try
	{
		// Player 1
		Stats stats;
		stats.m_score = m_tank.getScore();
		stats.m_accuracy = m_tank.getAccuracy();
		stats.m_percentTargetsHit = m_tank.getPercentTargetsHit();
		RoundStatsSaver::saveRoundStats(stats);

		// Player 2
		stats.m_score = m_controllerTank.getScore();
		stats.m_accuracy = m_controllerTank.getAccuracy();
		stats.m_percentTargetsHit = m_controllerTank.getPercentTargetsHit();
		RoundStatsSaver::saveRoundStats(stats);
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
	m_window.clear(sf::Color(0, 0, 0, 0));

	if (GameState::MenuScreen == m_gameState) // Draw menu
	{
		m_menuScreen.draw(m_window);
	}
	if (GameState::Gameplay == m_gameState) // Draw gameplay
	{
		m_window.draw(m_bgSprite);

		// If the game round is still going, draw the tanks, and targets
		if (m_gameTimer > 0.0)
		{
			m_tank.render(m_window);
			m_controllerTank.render(m_window);

			// Draw the targets
			for (Target const& target : m_targets)
			{
				if (target.active())
				{
					target.draw(m_window, m_timerCircle);
				}
			}
		}

		// Draw all the obstacles
		for (sf::Sprite const& obstacle : m_wallSprites)
		{
			m_window.draw(obstacle);
		}

		m_window.draw(m_timerText);
		m_window.draw(m_playerOneScoreText);
		m_window.draw(m_playerTwoScoreText);

		if (m_gameTimer <= 0.0)
		{
			m_window.draw(m_bestScoreText);
		}

		//m_window.draw(m_tempShape);
	}

	m_window.display();
}