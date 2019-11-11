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
	m_ROUND_TIME{ 60.0 }
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
	m_playerOneScoreText.setCharacterSize(40u);

	m_playerTwoScoreText.setFont(m_font);
	m_playerTwoScoreText.setCharacterSize(40u);
	m_playerTwoScoreText.setPosition(ScreenSize::s_width - 400, 0.0f);
	
	int currentLevel = 1;

	try
	{
		LevelLoader::load(currentLevel, m_level);
	}
	catch (std::exception &e)
	{
		std::cout << "Level load failed." << std::endl;
		std::cout << e.what() << std::endl;
		throw(e);
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

	m_bgSprite.setTexture(m_bgTexture);
	m_tank.setPosition(m_level.m_tank.m_position);
	m_controllerTank.setPosition({ m_level.m_tank.m_position.x - 200.0f, m_level.m_tank.m_position.y });

	generateWalls();
	generateTargets();

	m_timerCircle.setFillColor(sf::Color{ 255, 0, 0, 150 });

	m_controller.connect();
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
			lag -= MS_PER_UPDATE;
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
				m_tank.setPosition(m_TANK_POSITIONS[rand() % 4]);
				m_tank.resetScore();

				// Set the timers
				m_gameTimer = m_ROUND_TIME;
				m_spawnTimer.restart();
				m_clockTimer.restart();
				
				// Set text
				m_timerText.setString("Timer: " + std::to_string(static_cast<int>(ceil(m_gameTimer))));
				m_timerText.setCharacterSize(40u);
				m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
				m_timerText.setPosition(ScreenSize::s_width / 2, 30.0f);

				// Set targets
				m_targetsSpawned = 0;
				for (Target& target : m_targets)
				{
					target.resetTarget(m_wallSprites);
				}
			}
			break;
		}
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

	if (m_gameTimer > 0.0)
	{
		m_timerText.setString("Timer: " + std::to_string(static_cast<int>(ceil(m_gameTimer))));

		m_tank.setPrevious();
		m_tank.handleKeyInput();
		m_tank.update(dt);

		m_controllerTank.setPrevious();
		m_controllerTank.handleControllerInput(m_controller);
		m_controllerTank.update(dt);

		// Timer
		sf::Time timeSinceLastUpdate = m_clockTimer.restart();
		m_gameTimer -= timeSinceLastUpdate.asSeconds();

		// Spawn new target
		if (m_targets.size() > 0 && m_spawnTimer.getElapsedTime().asSeconds() > (m_ROUND_TIME - 5.0) / m_numberOfTargets)
		{
			if (m_targetsSpawned < m_numberOfTargets)
			{
				m_targets[m_targetsSpawned].setActive(true);
				m_targetsSpawned++;
				m_spawnTimer.restart();
			}
			else
			{
				std::cout << "Attempting to spawn a target out of range" << std::endl;
			}
		}

		// Decrease the target's time to live
		for (Target& target : m_targets)
		{
			if (target.active())
			{
				target.update(timeSinceLastUpdate);
			}
		}

		m_playerOneScoreText.setString("Player1's Score: " + std::to_string(m_tank.getScore()));
		m_playerTwoScoreText.setString("Player2's Score: " + std::to_string(m_controllerTank.getScore()));

		if (m_gameTimer < 0.0)
		{
			m_gameTimer = 0.0;
			m_timerText.setString("PRESS SPACE TO START");
			m_playerOneScoreText.setString("Player1:\n" + m_tank.getStatistics());
			m_playerTwoScoreText.setString("Player2:\n" + m_controllerTank.getStatistics());
			m_timerText.setCharacterSize(40u);
			m_timerText.setOrigin(m_timerText.getGlobalBounds().width / 2.0f, m_timerText.getGlobalBounds().height / 2.0f);
			m_timerText.setPosition(ScreenSize::s_width / 2, ScreenSize::s_height / 2);
		}
	}
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));

	m_window.draw(m_bgSprite);

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

	for (sf::Sprite const & obstacle : m_wallSprites)
	{
		m_window.draw(obstacle);
	}
	
	m_window.draw(m_timerText);
	m_window.draw(m_playerOneScoreText);
	m_window.draw(m_playerTwoScoreText);

	m_window.display();
}