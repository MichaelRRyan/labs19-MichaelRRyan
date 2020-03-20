#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GUIButton.h"
#include "ScreenSize.h"
#include "GameState.h"
#include "AssetManager.h"

class ModeSelectScreen
{
public:
	ModeSelectScreen(sf::Font const& t_font);
	void draw(sf::RenderWindow& t_window);
	void setup();
	void processEvents(sf::Event t_event, GameState& t_gameState, int t_numberOfPlayers);
	void updateLocked(int t_numberOfPlayers);

private:
	const sf::Vector2f m_TARGET_PRACTICE_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height / 2.0f) - (GUIButton::s_HEIGHT) };
	const sf::Vector2f m_COLLECTION_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height / 2.0f) };
	const sf::Vector2f m_VERSUS_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height / 2.0f) + (GUIButton::s_HEIGHT) };
	const sf::Vector2f m_BACK_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height) - (GUIButton::s_HEIGHT * 1.5f) };

	sf::Sprite m_background;

	GUIButton m_targetPracticeButton;
	GUIButton m_collectionButton;
	GUIButton m_versusButton;
	GUIButton m_backButton;

	sf::Text m_modeText;
	sf::Text m_versusLockedText;
};

