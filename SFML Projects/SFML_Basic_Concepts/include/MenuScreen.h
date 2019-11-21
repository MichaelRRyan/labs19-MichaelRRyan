#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GUIButton.h"
#include "ScreenSize.h"
#include "GameState.h"


class MenuScreen
{
public:
	MenuScreen(sf::Texture const& t_texture, sf::Font const& t_font);
	void draw(sf::RenderWindow& t_window);
	void setup();
	void processEvents(sf::Event t_event, GameState & t_gameState, sf::RenderWindow &t_window);

private:
	const sf::Vector2f m_PLAY_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height / 2.0f) - (GUIButton::s_HEIGHT / 2.0f) };
	const sf::Vector2f m_OPTIONS_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height / 2.0f) + (GUIButton::s_HEIGHT) };
	const sf::Vector2f m_EXIT_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height / 2.0f) + (GUIButton::s_HEIGHT * 2.5f) };

	sf::Texture const& m_texture;
	GUIButton m_playButton;
	GUIButton m_optionsButton;
	GUIButton m_exitButton;
};

#endif // !MENU_SCREEN_H