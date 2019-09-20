/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/09/2019
/// Start time: 12:00 - Time taken: 53m - Added tank sprite, movement and rotation code
/// </summary>

#include "Game.h"
#include <iostream>



// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default)
{
	m_window.setVerticalSyncEnabled(true);

	// Wrong
	// Creating a texture as a local variable means it's deleted once the function is completed.
	// Creating it here means the texture comes up as a white rectangle with the same size as the sprite.
	// sf::Texture texture;


	// Correct
	if (!m_texture.loadFromFile("E-100.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	m_sprite.setTexture(m_texture);

	// Setting the origin like this will make the centre the sprite. Since the position of the sprite is
	// set to { 0, 0 }, 3/4 of the sprite will be outside the screen
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.0f, m_sprite.getGlobalBounds().height / 2.0f);

	// Sets the position of the sprite so it's fully visible
	// Static casts the window size to perfectly centre the sprite on screen
	m_sprite.setPosition(static_cast<float>(ScreenSize::s_width / 2), static_cast<float>(ScreenSize::s_height / 2) - 50.0f);

	// Most sprites should be facing right by default, this one is not
	// Applying a rotation of negative 90 degrees should fix that
	// Rotates the tank by negative 90 degrees clockwise
	m_sprite.rotate(-90.0f);
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

	// Convert the angle (radians) to degrees and set the sprite rotation
	m_sprite.setRotation((m_tankRotation * 180.0f/ acosf(-1.0f)) - 90.0f);
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));

	m_window.draw(m_sprite);

	m_window.display();
}





