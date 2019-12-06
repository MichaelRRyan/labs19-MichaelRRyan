#include "MenuScreen.h"

MenuScreen::MenuScreen(sf::Texture const& t_guiSheet, sf::Texture const& t_background, sf::Font const& t_font) :
	m_playButton{ t_guiSheet, t_font, "PLAY", m_PLAY_BUTTON_POS },
	m_optionsButton{ t_guiSheet, t_font, "OPTIONS", m_OPTIONS_BUTTON_POS },
	m_exitButton{ t_guiSheet, t_font, "EXIT", m_EXIT_BUTTON_POS },
	m_background(t_background, { 0, 0, ScreenSize::s_width, ScreenSize::s_height })
{
	m_titleText.setFont(t_font);
	m_titleText.setPosition(ScreenSize::s_width / 2.0f, ScreenSize::s_height / 4.0f);
	m_titleText.setString("BATTLE TANKS");
	m_titleText.setCharacterSize(100u);
	m_titleText.setFillColor(sf::Color::Black);
	m_titleText.setOutlineColor(sf::Color::White);
	m_titleText.setOutlineThickness(10.0f);

	m_partSys.setTexture(t_guiSheet);
	m_partSys.addTextureRect({ 0, 470, 15, 15 });
	m_partSys.addTextureRect({ 15, 470, 9, 9 });
	m_partSys.addTextureRect({ 30, 470, 11, 11 });
	m_partSys.addTextureRect({ 45, 470, 5, 5 });

	thor::UniversalEmitter m_emitter;
	m_emitter.setEmissionRate(120);
	m_emitter.setParticleLifetime(sf::seconds(0.30f));
	m_emitter.setParticlePosition(sf::Vector2f{ 100.0f, 300.0f });   // Emit particles in given circle
	m_emitter.setParticleVelocity(thor::Distributions::deflect({500.0f, 0.0f}, 45.f)); // Emit towards direction with deviation of 15°
	m_emitter.setParticleTextureIndex(thor::Distributions::uniform(0, 3));

	thor::ColorGradient colourGradient;
	colourGradient[0.0f] = sf::Color::Red;
	colourGradient[0.8f] = sf::Color::Blue;
	colourGradient[1.0f] = sf::Color::Cyan;

	thor::ColorAnimation colourAnimation(colourGradient);

	thor::FadeAnimation fader(0.0f, 0.8f);
	m_partSys.addAffector(thor::AnimationAffector(fader));
	m_partSys.addAffector(thor::AnimationAffector(colourAnimation));
	
	float emmitTime = 1.5f;
	m_partSys.addEmitter(m_emitter, sf::seconds(emmitTime));
	
}

void MenuScreen::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_background);

	m_playButton.draw(t_window);
	m_optionsButton.draw(t_window);
	m_exitButton.draw(t_window);

	t_window.draw(m_titleText);

	t_window.draw(m_partSys);
}

void MenuScreen::setup()
{
	m_playButton.setup();
	m_optionsButton.setup();
	m_exitButton.setup();
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2.0f, m_titleText.getGlobalBounds().height / 2.0f);
}

void MenuScreen::processEvents(sf::Event t_event, GameState& t_gameState, sf::RenderWindow& t_window)
{
	if (m_playButton.processMouseEvents(t_event))
	{
		t_gameState = GameState::PlayerJoinScreen;
	}

	if (m_optionsButton.processMouseEvents(t_event))
	{
		std::cout << "Options clicked" << std::endl;
	}

	if (m_exitButton.processMouseEvents(t_event))
	{
		t_window.close();
	}
}

void MenuScreen::update(double t_dt)
{
	m_partSys.update(m_partSysClock.restart());
}
