#ifndef HELP_SCREEN_H
#define HELP_SCREEN_H

#include <iostream>
#include "GUIButton.h"
#include "ScreenSize.h"
#include "GameState.h"
#include "AssetManager.h"

class HelpScreen
{
public:
	HelpScreen(sf::Font const& t_font);
	void draw(sf::RenderWindow& t_window);
	void setup();
	void processEvents(sf::Event t_event, GameState& t_gameState);

private:
	enum class Controls
	{
		Player1Keyboard,
		Player2Keyboard,
		Controller
	};

	const sf::Vector2f m_BACK_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height) - (GUIButton::s_HEIGHT * 1.5f) };

	sf::Texture m_controlsTexture;

	sf::Sprite m_controlsSprite;
	sf::Sprite m_backgroundSprite;

	sf::RectangleShape m_containerShape;

	sf::Text m_controlsText;

	Controls m_controlsDisplay; // Current set of controls displayed on screen

	GUIButton m_backButton;
};

#endif // !HELP_SCREEN_H