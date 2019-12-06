#include "MenuScreen.h"

MenuScreen::MenuScreen(sf::Texture const& t_guiSheet, sf::Texture const& t_background, sf::Font const& t_font) :
	m_playButton{ t_guiSheet, t_font, "PLAY", m_PLAY_BUTTON_POS },
	m_optionsButton{ t_guiSheet, t_font, "OPTIONS", m_OPTIONS_BUTTON_POS },
	m_exitButton{ t_guiSheet, t_font, "EXIT", m_EXIT_BUTTON_POS },
	m_background(t_background, { 0, 0, ScreenSize::s_width, ScreenSize::s_height })
{
	m_titleText.setFont(t_font);
	m_titleText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 4.0f);
	m_titleText.setString("BATTLE TANKS");
	m_titleText.setCharacterSize(100u);
	m_titleText.setFillColor(sf::Color::Black);
	m_titleText.setOutlineColor(sf::Color::White);
	m_titleText.setOutlineThickness(10.0f);

	m_partSys.setTexture(t_guiSheet);
	m_partSys.addTextureRect({ 0,0,100,100 });
}

void MenuScreen::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_background);

	m_playButton.draw(t_window);
	m_optionsButton.draw(t_window);
	m_exitButton.draw(t_window);

	t_window.draw(m_titleText);

	t_window.draw(m_partSys);
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
		t_gameState = GameState::PlayerJoinScreen;
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

void MenuScreen::update(double t_dt)
{
	m_partSys.update(m_partSysClock.restart());
}
