#include "HUD.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 25/03/2020
/// </summary>

////////////////////////////////////////////////////////////
HUD::HUD()
{
}

////////////////////////////////////////////////////////////
void HUD::setupElements(sf::View t_guiView)
{
	// Temporary position variables
	sf::Vector2f screenCenter = t_guiView.getCenter();
	sf::Vector2f screenSize = t_guiView.getSize();
	sf::Vector2f topLeftCorner = screenCenter - screenSize / 2.0f;

	// Set up the background
	m_background.setPosition(topLeftCorner);
	m_background.setSize({ screenSize.x, screenSize.y * 0.1f });

	m_background.setFillColor(sf::Color{ 200, 200, 200, 150 }); // Semi-transparent grey colour

	// Setup the text
	m_cargoRemainingText.setFont(AssetManager::getFont("mainFont"));
	m_cargoRemainingText.setString("Cargo Remaining: 10");
	m_cargoRemainingText.setOrigin(m_cargoRemainingText.getGlobalBounds().width / 2.0f, 0.0f);
	m_cargoRemainingText.setPosition(screenCenter.x, m_background.getSize().y / 3.0f);

	m_cargoRemainingText.setFillColor(sf::Color::Black);
	m_cargoRemainingText.setOutlineColor(sf::Color::White);
	m_cargoRemainingText.setOutlineThickness(2.0f);
}

////////////////////////////////////////////////////////////
void HUD::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_background);
	t_window.draw(m_cargoRemainingText);
}

////////////////////////////////////////////////////////////
void HUD::updateCargoText(int t_cargoRemaining)
{
	m_cargoRemainingText.setString("Cargo Remaining: " + std::to_string(t_cargoRemaining));
	m_cargoRemainingText.setOrigin(m_cargoRemainingText.getGlobalBounds().width / 2.0f, 0.0f);
}

////////////////////////////////////////////////////////////