#include "HelpScreen.h"

HelpScreen::HelpScreen(sf::Texture const& t_guiSheet, sf::Texture const& t_background, sf::Font const& t_font) :
	m_backButton(t_guiSheet, t_font, "Back", m_BACK_BUTTON_POS),
	m_backgroundSprite(t_background, { 0, 0, ScreenSize::s_width, ScreenSize::s_height }),
	m_controlsText("", t_font, 25u)
{
}

void HelpScreen::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_backgroundSprite);
	t_window.draw(m_containerShape);
	t_window.draw(m_controlsSprite);
	t_window.draw(m_controlsText);

	m_backButton.draw(t_window);
}

void HelpScreen::setup()
{
	m_backButton.setup();

	if (!m_controlsTexture.loadFromFile("./resources/images/ControlsImage.png"))
	{
		std::cout << "Error loading controls images spritesheet" << std::endl;
	}

	m_controlsSprite.setTexture(m_controlsTexture);
	m_controlsSprite.setTextureRect({ 0, 0, 340, 220 });
	m_controlsSprite.setOrigin(m_controlsSprite.getGlobalBounds().width / 2.0f, m_controlsSprite.getGlobalBounds().height / 2.0f);
	m_controlsSprite.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 2.0f);

	m_containerShape.setPosition(ScreenSize::s_width * 0.25f, ScreenSize::s_height / 4.0f);
	m_containerShape.setSize({ ScreenSize::s_width * 0.5f, ScreenSize::s_height / 2.0f });

	m_controlsText.setPosition(ScreenSize::s_width * 0.25f + 10.0f, ScreenSize::s_height / 4.0f + 10.0f);
	m_controlsText.setString("Player1 Keyboard Controls:");
	m_controlsText.setFillColor(sf::Color::Black);
}

void HelpScreen::processEvents(sf::Event t_event, GameState& t_gameState)
{
	if (sf::Event::KeyPressed == t_event.type)
	{
		switch (m_controlsDisplay)
		{
		case HelpScreen::Controls::Player1Keyboard:
			m_controlsDisplay = Controls::Player2Keyboard;
			m_controlsSprite.setTextureRect({ 0, 227, 361, 220 });
			m_controlsSprite.setOrigin(m_controlsSprite.getGlobalBounds().width / 2.0f, m_controlsSprite.getGlobalBounds().height / 2.0f);
			m_controlsText.setString("Player2 Keyboard Controls:");
			break;
		case HelpScreen::Controls::Player2Keyboard:
			m_controlsDisplay = Controls::Controller;
			m_controlsSprite.setTextureRect({ 352, 0, 447, 332 });
			m_controlsSprite.setOrigin(m_controlsSprite.getGlobalBounds().width / 2.0f, m_controlsSprite.getGlobalBounds().height / 2.0f);
			m_controlsText.setString("Player Controller Controls:");
			break;
		case HelpScreen::Controls::Controller:
			m_controlsDisplay = Controls::Player1Keyboard;
			m_controlsSprite.setTextureRect({ 0, 0, 340, 220 });
			m_controlsSprite.setOrigin(m_controlsSprite.getGlobalBounds().width / 2.0f, m_controlsSprite.getGlobalBounds().height / 2.0f);
			m_controlsText.setString("Player1 Keyboard Controls:");
			break;
		}
	}

	if (m_backButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::MenuScreen;
	}
}
