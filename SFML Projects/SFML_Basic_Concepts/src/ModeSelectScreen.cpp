#include "ModeSelectScreen.h"

ModeSelectScreen::ModeSelectScreen(sf::Texture const& t_guiSheet, sf::Texture const& t_background, sf::Font const& t_font) :
	m_gameMode1Button(t_guiSheet, t_font, "Target Practice", m_TARGET_PRACTICE_BUTTON_POS),
	m_gameMode2Button(t_guiSheet, t_font, "Versus", m_VERSUS_BUTTON_POS),
	m_backButton(t_guiSheet, t_font, "Back", m_BACK_BUTTON_POS),
	m_background(t_background, { 0, 0, ScreenSize::s_width, ScreenSize::s_height })
{
	m_modeText.setFont(t_font);
	m_modeText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 4.0f);
	m_modeText.setString("Choose your game mode:");
	m_modeText.setCharacterSize(50u);
	m_modeText.setFillColor(sf::Color::White);
	m_modeText.setOutlineColor(sf::Color{ 50, 50, 50 });
	m_modeText.setOutlineThickness(5.0f);
}

void ModeSelectScreen::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_background);

	m_gameMode1Button.draw(t_window);
	m_gameMode2Button.draw(t_window);
	m_backButton.draw(t_window);

	t_window.draw(m_modeText);
}

void ModeSelectScreen::setup()
{
	m_gameMode1Button.setup();
	m_gameMode2Button.setup();
	m_backButton.setup();
	m_modeText.setOrigin(m_modeText.getGlobalBounds().width / 2.0f, m_modeText.getGlobalBounds().height / 2.0f);
}

void ModeSelectScreen::processEvents(sf::Event t_event, GameState& t_gameState)
{
	if (m_gameMode1Button.processMouseEvents(t_event))
	{
		t_gameState = GameState::TargetPractice;
	}

	if (m_gameMode2Button.processMouseEvents(t_event))
	{
		t_gameState = GameState::Versus;
	}

	if (m_backButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::MenuScreen;
	}
}
