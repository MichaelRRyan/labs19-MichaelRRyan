#include "ModeSelectScreen.h"

////////////////////////////////////////////////////////////
ModeSelectScreen::ModeSelectScreen(sf::Font const& t_font) :
	m_targetPracticeButton(AssetManager::getTexture("gui"), t_font, "Target Practice", m_TARGET_PRACTICE_BUTTON_POS),
	m_collectionButton(AssetManager::getTexture("gui"), t_font, "Cargo Collection", m_COLLECTION_BUTTON_POS),
	m_versusButton(AssetManager::getTexture("gui"), t_font, "Versus", m_VERSUS_BUTTON_POS),
	m_backButton(AssetManager::getTexture("gui"), t_font, "Back", m_BACK_BUTTON_POS),
	m_background(AssetManager::getTexture("menuBackground"), { 0, 0, ScreenSize::s_width, ScreenSize::s_height })
{
	m_modeText.setFont(t_font);
	m_modeText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 4.0f);
	m_modeText.setString("Choose your game mode:");
	m_modeText.setCharacterSize(50u);
	m_modeText.setFillColor(sf::Color::White);
	m_modeText.setOutlineColor(sf::Color{ 50, 50, 50 });
	m_modeText.setOutlineThickness(5.0f);

	m_versusLockedText.setFont(t_font);
	m_versusLockedText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 1.3f);
	m_versusLockedText.setString("You must have at least 2 players for versus mode");
	m_versusLockedText.setCharacterSize(20u);
	m_versusLockedText.setFillColor(sf::Color{ 200, 200, 200 });
	m_versusLockedText.setOutlineColor(sf::Color{ 50, 50, 50 });
	m_versusLockedText.setOutlineThickness(5.0f);
}

////////////////////////////////////////////////////////////
void ModeSelectScreen::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_background);

	m_targetPracticeButton.draw(t_window);
	m_collectionButton.draw(t_window);
	m_versusButton.draw(t_window);
	m_backButton.draw(t_window);

	if (m_versusButton.getLocked())
	{
		t_window.draw(m_versusLockedText);
	}

	t_window.draw(m_modeText);
}

////////////////////////////////////////////////////////////
void ModeSelectScreen::setup()
{
	m_targetPracticeButton.setup();
	m_collectionButton.setup();
	m_versusButton.setup();
	m_backButton.setup();
	m_modeText.setOrigin(m_modeText.getGlobalBounds().width / 2.0f, m_modeText.getGlobalBounds().height / 2.0f);
	m_versusLockedText.setOrigin(m_versusLockedText.getGlobalBounds().width / 2.0f, m_versusLockedText.getGlobalBounds().height / 2.0f);
}

////////////////////////////////////////////////////////////
void ModeSelectScreen::processEvents(sf::Event t_event, GameState& t_gameState, int t_numberOfPlayers)
{
	if (m_targetPracticeButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::TargetPractice;
	}

	if (m_collectionButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::Collection;
	}

	if (m_versusButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::Versus;
	}

	if (m_backButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::PlayerJoinScreen;
	}
}

////////////////////////////////////////////////////////////
void ModeSelectScreen::updateLocked(int t_numberOfPlayers)
{
	if (t_numberOfPlayers > 1)
	{
		m_versusButton.setLocked(false);
	}
	else
	{
		m_versusButton.setLocked(true);
	}
}
