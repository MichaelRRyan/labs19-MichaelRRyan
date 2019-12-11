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
		m_playerSprites[i].setPosition(static_cast<float>((ScreenSize::s_width / (MAX_PLAYERS + 1)) * (i + 1)), static_cast<float>(ScreenSize::s_height / 2));
		m_playerSprites[i].setColor(sf::Color{ 180, 180, 180, 180});

		m_playerControlSprites[i].setTexture(t_guiSheet);
		m_playerControlSprites[i].setOrigin(101.0f, 42.0f);
		m_playerControlSprites[i].setPosition(static_cast<float>((ScreenSize::s_width / (MAX_PLAYERS + 1)) * (i + 1)), static_cast<float>(ScreenSize::s_height / 2.0f - (textureRext.height / 2.0f) - 40.0f));
	}

	m_playerSelectBackground.setPosition(ScreenSize::s_width * 0.1f, ScreenSize::s_height / 4.0f);
	m_playerSelectBackground.setSize({ ScreenSize::s_width * 0.8f, ScreenSize::s_height / 2.2f });
	
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
					t_tanks[0].setControlType(ControlType::Keyboard, KeyConfiguration::ArrowKeys);
					t_numberOfPlayers++;
					m_playerSprites[0].setColor(sf::Color::Green);
					m_continueButton.setLocked(false);
					m_playerControlSprites[0].setTextureRect({ 132, 303, 202, 83 });
				}
				// Else if there is at least one player but they're in control config, switch to keyboard
				else if (t_numberOfPlayers > 0
						&& ControlType::Controller == t_tanks[0].getControlType())
				{
					t_tanks[0].setControlType(ControlType::Keyboard, KeyConfiguration::ArrowKeys);
					m_playerControlSprites[0].setTextureRect({ 132, 303, 202, 83 });
				}
				// Else if there is only one player and they're in keyboard config, remove them from the game
				else if (1 == t_numberOfPlayers 
						&& ControlType::Keyboard == t_tanks[0].getControlType())
				{
					t_numberOfPlayers--;
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
					t_tanks[1].setControlType(ControlType::Keyboard, KeyConfiguration::YGHJKeys);
					t_numberOfPlayers++;
					m_playerSprites[1].setColor(sf::Color::Red);
					m_playerControlSprites[1].setTextureRect({ 132, 386, 202, 83 });
				}
				// Else if there is at least one player but they're in control config, switch to keyboard
				else if (t_numberOfPlayers > 1
						 && ControlType::Controller == t_tanks[1].getControlType())
				{
					t_tanks[1].setControlType(ControlType::Keyboard, KeyConfiguration::YGHJKeys);
					m_playerControlSprites[1].setTextureRect({ 132, 386, 202, 83 });
				}
				// Else if there is only two players and they the last is in keyboard config, remove them from the game
				else if (2 == t_numberOfPlayers
					&& ControlType::Keyboard == t_tanks[1].getControlType())
				{
					t_numberOfPlayers--;
					m_playerSprites[1].setColor(sf::Color{ 180, 180, 180, 180 });
				}
			}
		}
	}
}

void PlayerJoinScreen::update(Tank t_tanks[], XBox360Controller t_controllers[], int& t_numberOfPlayers)
{
	// Loop through all the controllers
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Check if A is just pressed
		if (t_controllers[i].currentState.A
			&& !t_controllers[i].previousState.A)
		{
			bool alreadyUsed = false;

			// Loop through all players to check if this controller is already used
			for (int j = 0; j < t_numberOfPlayers; j++)
			{
				// Check if the current controller is already being used
				if (t_tanks[j].getControlType() == ControlType::Controller
					&& t_controllers[i].joystickIndex == t_tanks[j].getJoystickIndex())
				{
					// If the current player is the last player in the game, remove them
					if (t_numberOfPlayers - 1 == j)
					{
						t_tanks[j].setControllerPtr(nullptr);
						m_playerSprites[j].setColor(sf::Color{ 180, 180, 180, 180 });
						t_numberOfPlayers--;
					}
					
					alreadyUsed = true;
					break;
				}
			}
			
			// Add the controller to a player
			if (!alreadyUsed)
			{
				if (t_numberOfPlayers < MAX_PLAYERS)
				{
					t_tanks[t_numberOfPlayers].setControlType(ControlType::Controller);
					t_tanks[t_numberOfPlayers].setControllerPtr(&t_controllers[i]);
					m_playerSprites[t_numberOfPlayers].setColor(m_PLAYER_COLOURS[t_numberOfPlayers]);
					m_playerControlSprites[t_numberOfPlayers].setTextureRect({ 132, 220, 202, 83 });
					t_numberOfPlayers++;
					m_continueButton.setLocked(false);
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
}

void PlayerJoinScreen::draw(sf::RenderWindow& t_window, int t_numberOfPlayers)
{
	t_window.draw(m_background);
	m_continueButton.draw(t_window);
	m_backButton.draw(t_window);

	t_window.draw(m_playerSelectBackground);

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		t_window.draw(m_playerSprites[i]);

		if (i < t_numberOfPlayers)
		{
			t_window.draw(m_playerControlSprites[i]);
		}
	}
}

void PlayerJoinScreen::setup()
{
	m_continueButton.setup();
	m_backButton.setup();
}
