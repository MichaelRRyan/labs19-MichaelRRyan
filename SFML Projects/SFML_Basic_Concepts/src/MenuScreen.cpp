#include "MenuScreen.h"

MenuScreen::MenuScreen(sf::Texture const& t_texture, sf::Font const& t_font) :
	m_texture{ t_texture },
	m_playButton{ t_texture, t_font, "Play", m_PLAY_BUTTON_POS },
	m_optionsButton{ t_texture, t_font, "Options", m_OPTIONS_BUTTON_POS },
	m_exitButton{ t_texture, t_font, "Exit", m_EXIT_BUTTON_POS }
{
}

void MenuScreen::draw(sf::RenderWindow& t_window)
{
	m_playButton.draw(t_window);
	m_optionsButton.draw(t_window);
	m_exitButton.draw(t_window);
}

void MenuScreen::setup()
{
	m_playButton.setup();
	m_optionsButton.setup();
	m_exitButton.setup();
}

void MenuScreen::processEvents(sf::Event t_event, GameState& t_gameState, sf::RenderWindow& t_window)
{
	if (m_playButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::Gameplay;
	}

	if (m_optionsButton.processMouseEvents(t_event))
	{
		std::cout << "Options clicked" << std::endl;
	}

	if (m_exitButton.processMouseEvents(t_event))
	{
		t_window.close();
	}
}
