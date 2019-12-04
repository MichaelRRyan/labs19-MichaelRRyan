#include "..\include\PlayerJoinScreen.h"

PlayerJoinScreen::PlayerJoinScreen(sf::Texture const& t_guiSheet, sf::Texture const& t_background, sf::Font const& t_font) :
	m_background(t_background, { 0, 0, ScreenSize::s_width, ScreenSize::s_height }),
	m_continueButton(t_guiSheet, t_font, "Continue", m_CONTINUE_BUTTON_POS),
	m_backButton(t_guiSheet, t_font, "Back", m_BACK_BUTTON_POS)
{
	m_continueButton.setLocked(true);

	sf::IntRect textureRext{ 0, 202, 127, 266 };
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_playerSprites[i].setTexture(t_guiSheet);
		m_playerSprites[i].setTextureRect(textureRext);
		m_playerSprites[i].setOrigin(textureRext.width / 2.0f, textureRext.height / 2.0f);
		m_playerSprites[i].setPosition((ScreenSize::s_width / (MAX_PLAYERS + 1)) * (i + 1) , ScreenSize::s_height / 2.0f);
		m_playerSprites[i].setColor(sf::Color{ 180, 180, 180, 180});
	}

	m_playerSelectBackground.setSize({ ScreenSize::s_width / 1.17f, ScreenSize::s_height / 2.2f });
	m_playerSelectBackground.setPosition(100.0f, ScreenSize::s_height / 4.0f);
}

void PlayerJoinScreen::processEvents(sf::Event t_event, GameState& t_gameState, Tank t_tanks[], int& t_numberOfPlayers)
{
	// GUI button presses
	if (m_continueButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::ModeSelect;
	}
	
	if (m_backButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::MenuScreen;
	}

	if (t_numberOfPlayers < MAX_PLAYERS)
	{
		// Keyboard presses
		if (sf::Event::KeyReleased == t_event.type)
		{
			// PLAYER1 SPECIFIC
			if (sf::Keyboard::Period == t_event.key.code)
			{
				// If there's no players in the game, add player1 with keyboard config
				if (0 == t_numberOfPlayers)
				{
					t_tanks[0].setControlType(ControlType::Keyboard, ControlScheme::ArrowKeys);
					t_numberOfPlayers++;
					std::cout << t_numberOfPlayers;
					m_playerSprites[0].setColor(sf::Color::Green);
					m_continueButton.setLocked(false);
				}
				// Else if there is at least one player but they're in control config, switch to keyboard
				else if (t_numberOfPlayers > 0
						&& ControlType::Controller == t_tanks[0].getControlType())
				{
					t_tanks[0].setControlType(ControlType::Keyboard, ControlScheme::ArrowKeys);
					std::cout << t_numberOfPlayers;
				}
				// Else if there is only one player and they're in keyboard config, remove them from the game
				else if (1 == t_numberOfPlayers 
						&& ControlType::Keyboard == t_tanks[0].getControlType())
				{
					t_numberOfPlayers--;
					std::cout << t_numberOfPlayers;
					m_playerSprites[0].setColor(sf::Color{ 180, 180, 180, 180 });
					m_continueButton.setLocked(true);
				}
			}
			// PLAYER2 SPECIFIC
			else if (sf::Keyboard::W == t_event.key.code)
			{
				// If there's 1 player in the game, add player2 with keyboard config
				if (1 == t_numberOfPlayers)
				{
					t_tanks[1].setControlType(ControlType::Keyboard, ControlScheme::YGHJKeys);
					t_numberOfPlayers++;
					std::cout << t_numberOfPlayers;
					m_playerSprites[1].setColor(sf::Color::Red);
				}
				// Else if there is at least one player but they're in control config, switch to keyboard
				else if (t_numberOfPlayers > 1
						 && ControlType::Controller == t_tanks[1].getControlType())
				{
					t_tanks[1].setControlType(ControlType::Keyboard, ControlScheme::YGHJKeys);
					std::cout << t_numberOfPlayers;
				}
				// Else if there is only two players and they the last is in keyboard config, remove them from the game
				else if (2 == t_numberOfPlayers
					&& ControlType::Keyboard == t_tanks[1].getControlType())
				{
					t_numberOfPlayers--;
					std::cout << t_numberOfPlayers;
					m_playerSprites[1].setColor(sf::Color{ 180, 180, 180, 180 });
				}
			}

		}
	}
}

void PlayerJoinScreen::update(Tank t_tanks[], int& t_numberOfPlayers)
{
}

void PlayerJoinScreen::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_background);
	m_continueButton.draw(t_window);
	m_backButton.draw(t_window);

	t_window.draw(m_playerSelectBackground);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		t_window.draw(m_playerSprites[i]);
	}
}

void PlayerJoinScreen::setup()
{
	m_continueButton.setup();
	m_backButton.setup();
}
