#include "MenuScreen.h"

MenuScreen::MenuScreen(sf::Font const& t_font) :
	m_playButton{ AssetManager::getTexture("gui"), t_font, "PLAY", m_PLAY_BUTTON_POS },
	m_helpButton{ AssetManager::getTexture("gui"), t_font, "HELP", m_OPTIONS_BUTTON_POS },
	m_exitButton{ AssetManager::getTexture("gui"), t_font, "EXIT", m_EXIT_BUTTON_POS },
	m_background(AssetManager::getTexture("menuBackground"), { 0, 0, ScreenSize::s_width, ScreenSize::s_height })
{
	m_titleText.setFont(t_font);
	m_titleText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 4.0f);
	m_titleText.setString("BATTLE TANKS");
	m_titleText.setCharacterSize(100u);
	m_titleText.setFillColor(sf::Color::Black);
	m_titleText.setOutlineColor(sf::Color::White);
	m_titleText.setOutlineThickness(10.0f);
}

void MenuScreen::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_background);

	m_playButton.draw(t_window);
	m_helpButton.draw(t_window);
	m_exitButton.draw(t_window);

	t_window.draw(m_titleText);
}

void MenuScreen::setup()
{
	m_playButton.setup();
	m_helpButton.setup();
	m_exitButton.setup();
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2.0f, m_titleText.getGlobalBounds().height / 2.0f);
}

void MenuScreen::processEvents(sf::Event t_event, GameState& t_gameState, sf::RenderWindow& t_window)
{
	if (m_playButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::PlayerJoinScreen;
	}

	if (m_helpButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::HelpScreen;
	}

	if (m_exitButton.processMouseEvents(t_event))
	{
		t_window.close();
	}
}
