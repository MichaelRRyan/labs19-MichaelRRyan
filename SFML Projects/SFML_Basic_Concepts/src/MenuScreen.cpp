#include "MenuScreen.h"

MenuScreen::MenuScreen(sf::Texture const& t_texture, sf::Font const& t_font) :
	m_texture{ t_texture },
	m_playButton{ t_texture, t_font, "Play", m_PLAY_BUTTON_POS }
{
}

void MenuScreen::draw(sf::RenderWindow& t_window)
{
	m_playButton.draw(t_window);
}

void MenuScreen::setup()
{
	m_playButton.setup();
}

void MenuScreen::processEvents(sf::Event t_event, GameState& t_gameState)
{
	if (m_playButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::Gameplay;
	}
}
