#ifndef HUD_H
#define HUD_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 25/03/2020
/// </summary>

#include <SFML/Graphics.hpp>
#include <AssetManager.h>

class HUD
{
public:

	HUD();

	void setupElements(sf::View t_guiView);

	void render(sf::RenderWindow & t_window);

	void updateCargoText(int t_cargoRemaining);

private:

	sf::RectangleShape m_background;
	sf::Text m_cargoRemainingText;

};

#endif // !HUD_H