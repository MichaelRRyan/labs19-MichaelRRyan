/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/09/2019
/// Start time: 12:00 - Time taken: 53m - Added tank sprite, movement and rotation code
/// </summary>

#include "Game.h"



// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default)
{
	m_window.setVerticalSyncEnabled(true);

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
	if (!m_texture.loadFromFile("E-100.png"))
	{
		std::string s("Error loading tank texture");
		throw std::exception(s.c_str());
	}

	if (!m_bgTexture.loadFromFile(m_level.m_background.m_fileName))
	{
		std::string s("Error loading background texture");
		throw std::exception(s.c_str());
	}

	if (!m_spriteSheetTexture.loadFromFile("SpriteSheet.png"))
	{
		std::string s("Error loading sprite sheet texture");
		throw std::exception(s.c_str());
	}

	// Extract the wall image from the sprite sheet
	sf::Sprite sprite;
	sf::IntRect wallRect(2, 129, 33, 23);
	sprite.setTexture(m_spriteSheetTexture);
	sprite.setTextureRect(wallRect);

	for (auto &obstacle : m_level.m_obstacles)
	{
		// Set the position of the obstacles using the level data
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);
		m_obstacles.push_back(sprite);
	}


	m_sprite.setTexture(m_spriteSheetTexture);
	m_sprite.setTextureRect(sf::IntRect{ {0,42}, {88,88} });

	m_tankTurret.setTexture(m_spriteSheetTexture);
	m_tankTurret.setTextureRect(sf::IntRect{ {0,0}, {102,34} });

	m_bgSprite.setTexture(m_bgTexture);

	m_sprite.setPosition(m_level.m_tank.m_position);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.0f, 23.0f);
	m_tankTurret.setOrigin(40, m_tankTurret.getGlobalBounds().height / 2.0f);

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
		case sf::Keyboard::Up:
			// Up key was pressed...
			break;
		default:
			break;
		}
	}
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	sf::Vector2f m_movement;

	// Get rotate input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_tankRotation -= m_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_tankRotation += m_turnSpeed;
	}

	// Get rotate input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_tankTurretRotation -= m_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_tankTurretRotation += m_turnSpeed;
	}

	// Get movement input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_movement = { cos(m_tankRotation), sin(m_tankRotation) };
		m_movement *= m_moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_movement = { cos(m_tankRotation), sin(m_tankRotation) };
		m_movement *= -m_moveSpeed;
	}

	// Move the sprite
	m_sprite.move(m_movement);
	m_tankTurret.setPosition(m_sprite.getPosition());

	// Convert the angle (radians) to degrees and set the sprite rotation
	float tankAngle = m_tankRotation * 180.0f / acosf(-1.0f);
	float tankTurretAngle = m_tankTurretRotation * 180.0f / acosf(-1.0f);
	m_sprite.setRotation(tankAngle);
	m_tankTurret.setRotation(tankAngle + tankTurretAngle);
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));

	m_window.draw(m_bgSprite);
	m_window.draw(m_sprite);
	m_window.draw(m_tankTurret);

	for (sf::Sprite &obstacle : m_obstacles)
	{
		m_window.draw(obstacle);
	}

	m_window.display();
}





