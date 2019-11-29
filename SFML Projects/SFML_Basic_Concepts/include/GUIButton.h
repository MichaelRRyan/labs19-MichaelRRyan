#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <SFML/Graphics.hpp>

class GUIButton
{
public:
	GUIButton(sf::Texture const& t_texture, sf::Font const& t_font, std::string t_textString, sf::Vector2f t_position);
	void draw(sf::RenderWindow& t_window);
	void setup();
	bool processMouseEvents(sf::Event t_event);

	static constexpr float s_WIDTH{ 500.0f };
	static constexpr float s_HEIGHT{ 100.0f };

private:
	sf::Sprite m_sprite;
	sf::Text m_text;
};

#endif // !GUI_BUTTON_H