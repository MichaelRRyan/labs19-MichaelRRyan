#include "MenuScreen.h"

MenuScreen::MenuScreen(sf::Texture const& t_texture, sf::Font const& t_font) :
	m_playButton{ t_texture, t_font, "PLAY", m_PLAY_BUTTON_POS },
	m_optionsButton{ t_texture, t_font, "OPTIONS", m_OPTIONS_BUTTON_POS },
	m_exitButton{ t_texture, t_font, "EXIT", m_EXIT_BUTTON_POS }
{
	m_titleText.setFont(t_font);
	m_titleText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 4.0f);
	m_titleText.setString("TOP DOWN TANKS");
	m_titleText.setCharacterSize(80u);
	m_titleText.setFillColor(sf::Color{ 114, 183, 84 });
	m_titleText.setStyle(sf::Text::Bold | sf::Text::Italic);
	m_titleText.setOutlineColor(sf::Color{ 100, 61, 32});
	m_titleText.setOutlineThickness(5.0f);
}

void MenuScreen::draw(sf::RenderWindow& t_window)
{
	m_playButton.draw(t_window);
	m_optionsButton.draw(t_window);
	m_exitButton.draw(t_window);
	t_window.draw(m_titleText);
}

void MenuScreen::setup()
{
	m_playButton.setup();
	m_optionsButton.setup();
	m_exitButton.setup();
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2.0f, m_titleText.getGlobalBounds().height / 2.0f);
}

void MenuScreen::processEvents(sf::Event t_event, GameState& t_gameState, sf::RenderWindow& t_window)
{
	if (m_playButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::ModeSelect;
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
