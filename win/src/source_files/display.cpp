#include "../header_files/display.hpp"

GUIManager Display::mGUIManager = GUIManager();
sf::Font Display::mFont = sf::Font();

Display::Display() :
	mpWindow{ new sf::RenderWindow() },
	mpEvent{ new sf::Event() }
{
	mpWindow->create(sf::VideoMode(1280, 720, 32), "MCM File Transmitter V 1.0", sf::Style::Close);
	mpWindow->setFramerateLimit(60);

	mFont.loadFromFile("assets/font/Xolonium-Regular.otf");

	mManager.addState(*this);
}

void Display::run()
{
	while (mpWindow->isOpen())
	{
		handleEvents();
		update();
		render();
	}
}

void Display::handleEvents()
{
	while (mpWindow->pollEvent(*mpEvent))
	{
		mGUIManager.handleEvents(*this);

		mManager.handleEvents(*this);
	}
}

void Display::update()
{
	mGUIManager.update(*this);

	mManager.update(*this);
}

void Display::render()
{
	mpWindow->clear(sf::Color(0, 0, 0));

	mGUIManager.render(*this);

	mManager.render(*this);

	mpWindow->display();
}

Display::~Display()
{

}