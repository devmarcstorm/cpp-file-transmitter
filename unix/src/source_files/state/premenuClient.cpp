#include "../../header_files/state/premenuClient.hpp"

#include "../../header_files/gui/panel.hpp"
#include "../../header_files/gui/button.hpp"
#include "../../header_files/gui/textbox.hpp"

#include "../../header_files/display.hpp"

#include "../header_files/settings.hpp"

#include <iostream>

PremenuClient::PremenuClient()
{

}

void PremenuClient::enter(Display& display)
{
	nextState = "";

	createGUI(display);
}

void PremenuClient::leave()
{
	Display::mGUIManager.clear();
}

void PremenuClient::handleEvents(Display& display)
{
	if (display.mpEvent->type == sf::Event::Closed)
	{
		display.mpWindow->close();
	}
}

void PremenuClient::update(Display& display)
{
	if (Display::mGUIManager.getObject("enter")->mCurrentState == GUIObjectStates::SELECTED)
	{
		Settings::serverIp = Display::mGUIManager.getObject("ip")->mText.getString();

		nextState = "clientMenu";
	}

	if (Display::mGUIManager.getObject("return")->mCurrentState == GUIObjectStates::SELECTED)
	{
		nextState = "mainmenu";
	}
}

void PremenuClient::render(Display& display)
{

}

void PremenuClient::createGUI(Display& display)
{
	Display::mGUIManager.registerObject("bg", std::shared_ptr<GUIObject>(new Panel
	(sf::Vector2f(display.mpWindow->getSize()), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, display.mpWindow->getSize().y * 0.5f), "assets/textures/mcm_ft_bg.jpg")));

	std::vector<char> ipChars = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.'};

	Display::mGUIManager.registerObject("ip", std::shared_ptr<GUIObject>(new Textbox
	("Ip address", sf::Vector2f(500.f, 75.f), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, 260.f), sf::Color(100, 100, 125, 150), Display::mFont, 30, 20, true, ipChars)));

	Display::mGUIManager.registerObject("enter", std::shared_ptr<GUIObject>(new Button
	("Enter server", sf::Vector2f(500.f, 75.f), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, 360.f), sf::Color(100, 125, 100, 150), Display::mFont, 30)));
	Display::mGUIManager.registerObject("return", std::shared_ptr<GUIObject>(new Button
	("Back", sf::Vector2f(500.f, 75.f), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, 460.f), sf::Color(100, 125, 100, 150), Display::mFont, 30)));
}

PremenuClient::~PremenuClient()
{

}