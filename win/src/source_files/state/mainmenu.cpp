#include "../../header_files/state/mainmenu.hpp"

#include "../../header_files/gui/panel.hpp"
#include "../../header_files/gui/button.hpp"

#include "../../header_files/display.hpp"

#include <iostream>

Mainmenu::Mainmenu()
{

}

void Mainmenu::enter(Display& display)
{
	nextState = "";

	createGUI(display);
}

void Mainmenu::leave()
{
	Display::mGUIManager.clear();
}

void Mainmenu::handleEvents(Display& display)
{
	if (display.mpEvent->type == sf::Event::Closed)
	{
		display.mpWindow->close();
	}
}

void Mainmenu::update(Display& display)
{
	if (Display::mGUIManager.getObject("host")->mCurrentState == GUIObjectStates::SELECTED)
	{
		nextState = "serverMenu";
	}

	if (Display::mGUIManager.getObject("enter")->mCurrentState == GUIObjectStates::SELECTED)
	{
		nextState = "premenuClient";
	}

	if (Display::mGUIManager.getObject("quit")->mCurrentState == GUIObjectStates::SELECTED)
	{
		display.mpWindow->close();
	}
}

void Mainmenu::render(Display& display)
{

}

void Mainmenu::createGUI(Display& display)
{
	Display::mGUIManager.registerObject("bg", std::shared_ptr<GUIObject>(new Panel
	(sf::Vector2f(display.mpWindow->getSize()), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, display.mpWindow->getSize().y * 0.5f), "assets/textures/mcm_ft_bg.jpg")));

	Display::mGUIManager.registerObject("host", std::shared_ptr<GUIObject>(new Button
	("Host server", sf::Vector2f(500.f, 75.f), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, 260.f), sf::Color(100, 125, 100, 150), Display::mFont, 30)));
	Display::mGUIManager.registerObject("enter", std::shared_ptr<GUIObject>(new Button
	("Enter server", sf::Vector2f(500.f, 75.f), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, 360.f), sf::Color(100, 125, 100, 150), Display::mFont, 30)));
	Display::mGUIManager.registerObject("quit", std::shared_ptr<GUIObject>(new Button
	("Quit", sf::Vector2f(500.f, 75.f), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, 460.f), sf::Color(100, 125, 100, 150), Display::mFont, 30)));
}

Mainmenu::~Mainmenu()
{

}