#include "..\include\PlayerJoinScreen.h"

PlayerJoinScreen::PlayerJoinScreen(sf::Texture const& t_guiSheet, sf::Texture const& t_background, sf::Font const& t_font) :
	m_background(t_background, { 0, 0, ScreenSize::s_width, ScreenSize::s_height }),
	m_continueButton(t_guiSheet, t_font, "Continue", m_CONTINUE_BUTTON_POS),
	m_backButton(t_guiSheet, t_font, "Back", m_BACK_BUTTON_POS)
{
	m_continueButton.setLocked(true);
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
			if (sf::Keyboard::Period == t_event.type)
			{
				// If there's no players in the game, add player1 with keyboard config
				if (0 == t_numberOfPlayers)
				{
					t_tanks[0].setControlType(ControlType::Keyboard, ControlScheme::ArrowKeys);
					t_numberOfPlayers++;
					std::cout << t_numberOfPlayers;
				}
				// Else if there is at least one player but they're in control config, switch to keyboard
				else if (ControlType::Controller == t_tanks[0].getControlType())
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
}

void PlayerJoinScreen::setup()
{
	m_continueButton.setup();
	m_backButton.setup();
}
