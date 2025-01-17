#ifndef PLAYER_JOIN_SCREEN_H
#define PLAYER_JOIN_SCREEN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ScreenSize.h"
#include "GameState.h"
#include "GUIButton.h"
#include "Tank.h"
#include "GameConfig.h"
#include "XBox360Controller.h"

class PlayerJoinScreen
{
public:
	PlayerJoinScreen(sf::Font const& t_font);
	void processEvents(sf::Event t_event, GameState& t_gameState, Tank t_tanks[], int & t_numberOfPlayers);
	void update(Tank t_tanks[], XBox360Controller t_controllers[], int& t_numberOfPlayers);
	void draw(sf::RenderWindow & t_window, int t_numberOfPlayers);
	void setup();


private:
	const sf::Vector2f m_CONTINUE_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height) - (GUIButton::s_HEIGHT * 2.5f) };
	const sf::Vector2f m_BACK_BUTTON_POS{ (ScreenSize::s_width / 2.0f) - (GUIButton::s_WIDTH / 2.0f), (ScreenSize::s_height) - (GUIButton::s_HEIGHT * 1.5f) };
	const sf::Color m_PLAYER_COLOURS[MAX_PLAYERS]{ sf::Color::Green, sf::Color::Red, sf::Color::Blue, sf::Color::Yellow };

	sf::Sprite m_background;

	sf::RectangleShape m_playerSelectBackground;
	sf::Sprite m_playerSprites[MAX_PLAYERS];
	sf::Sprite m_playerControlSprites[MAX_PLAYERS];

	GUIButton m_continueButton;
	GUIButton m_backButton;
};

#endif // !PLAYER_JOIN_SCREEN_H