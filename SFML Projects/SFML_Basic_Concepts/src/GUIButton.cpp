#include "GUIButton.h"

GUIButton::GUIButton(sf::Texture const& t_texture, sf::Font const & t_font, std::string t_textString, sf::Vector2f t_position) :
	m_texture{ t_texture }
{
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect({ 0, 0, static_cast<int>(s_WIDTH), static_cast<int>(s_HEIGHT) });
	m_sprite.setPosition(t_position);

	m_text.setPosition(t_position.x + s_WIDTH / 2.0f, t_position.y + s_HEIGHT / 2.0f);
	m_text.setString(t_textString);
	m_text.setFont(t_font);
}

void GUIButton::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_sprite);
	t_window.draw(m_text);
}

void GUIButton::setup()
{
	m_text.setOrigin(m_text.getGlobalBounds().width / 2.0f, m_text.getGlobalBounds().height / 2.0f);
	m_text.setFillColor(sf::Color::Black);
}

bool GUIButton::processMouseEvents(sf::Event t_event)
{
	if (sf::Event::MouseMoved)
	{
		m_sprite.setTextureRect({ 0, 0, static_cast<int>(s_WIDTH), static_cast<int>(s_HEIGHT) });

		// Check the mouse pointer against the button x bounds
		if (t_event.mouseMove.x > m_sprite.getPosition().x
			&& t_event.mouseMove.x < m_sprite.getPosition().x + m_sprite.getGlobalBounds().width)
		{
			// Check the mouse pointer against the button y bounds
			if (t_event.mouseMove.y > m_sprite.getPosition().y
				&& t_event.mouseMove.y < m_sprite.getPosition().y + m_sprite.getGlobalBounds().height)
			{
				m_sprite.setTextureRect({ 0, static_cast<int>(s_HEIGHT) + 1, static_cast<int>(s_WIDTH), static_cast<int>(s_HEIGHT) });
			}
		}
	}

	if (sf::Event::MouseButtonPressed == t_event.type)
	{
		// Check if the button was pressed
		if (sf::Mouse::Left == t_event.mouseButton.button)
		{
			// Check the mouse pointer against the button x bounds
			if (t_event.mouseButton.x > m_sprite.getPosition().x
				&& t_event.mouseButton.x < m_sprite.getPosition().x + m_sprite.getGlobalBounds().width)
			{
				// Check the mouse pointer against the button y bounds
				if (t_event.mouseButton.y > m_sprite.getPosition().y
					&& t_event.mouseButton.y < m_sprite.getPosition().y + m_sprite.getGlobalBounds().height)
				{
					return true;
				}
			}
		}
	}
	return false;
}