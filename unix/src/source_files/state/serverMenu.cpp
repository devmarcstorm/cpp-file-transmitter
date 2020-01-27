#include "../../header_files/state/serverMenu.hpp"

#include "../../header_files/gui/panel.hpp"
#include "../../header_files/gui/button.hpp"
#include "../../header_files/gui/textbox.hpp"
#include "../../header_files/gui/textArea.hpp"

#include "../../header_files/display.hpp"

#include "../header_files/settings.hpp"

#include <iostream>

ServerMenu::ServerMenu() :
	mServer{ &mClients },
	mBrokenClient{ false },
	mBrokenServer{ false }
{

}

void ServerMenu::enter(Display& display)
{
	nextState = "";

	createGUI(display);

    if (mServer.Start() != -1)
    {
		std::promise<void>().swap(mExitSignal);

		std::future<void> futureObj = mExitSignal.get_future();

		std::thread newServer(mServer, std::move(futureObj));

        newServer.detach();

		mBrokenServer = false;

		std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Wait for listener to set up before try to connect

		if (mClient.start("127.0.0.1") == -1)
		{
			mBrokenClient = true;
		}
		else
		{
			mBrokenClient = false;
		}
    }
	else
	{
		mBrokenServer = true;
	}
}

void ServerMenu::leave()
{
	Display::mGUIManager.clear();

	if (mBrokenServer == false)
	{
		if (mBrokenClient == false)
		{
			mClient.close();
		}

		mExitSignal.set_value();
	}
}

void ServerMenu::handleEvents(Display& display)
{
	if (display.mpEvent->type == sf::Event::Closed)
	{
		if (mBrokenServer == false)
		{
			if (mBrokenClient == false)
			{
				mClient.close();
			}

			mExitSignal.set_value();
		}

		display.mpWindow->close();
	}

	if (Display::mGUIManager.getObject("input")->mCurrentState == GUIObjectStates::TEXT_ENTERED)
	{
		if (Display::mGUIManager.getObject("input")->mText.getString().substring(0, 1) == "-")
		{
			std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine(Display::mGUIManager.getObject("input")->mText.getString(), sf::Color(100, 100, 100));
		}
		else
		{
			std::dynamic_pointer_cast<TextArea>(Display::mGUIManager.getObject("console"))->addLine(Display::mGUIManager.getObject("input")->mText.getString(), sf::Color::Cyan);
		}

		if (mBrokenClient == false)
		{
			mClient.sender(Display::mGUIManager.getObject("input")->mText.getString());
		}

		Display::mGUIManager.getObject("input")->mText.setString("");
	}
}

void ServerMenu::update(Display& display)
{
	if (Display::mGUIManager.getObject("quit")->mCurrentState == GUIObjectStates::SELECTED)
	{
		nextState = "mainmenu";
	}
}

void ServerMenu::render(Display& display)
{

}

void ServerMenu::createGUI(Display& display)
{
	Display::mGUIManager.registerObject("bg", std::shared_ptr<GUIObject>(new Panel
	(sf::Vector2f(display.mpWindow->getSize()), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, display.mpWindow->getSize().y * 0.5f), "assets/textures/mcm_ft_bg.jpg")));

	Display::mGUIManager.registerObject("console", std::shared_ptr<GUIObject>(new TextArea
	(sf::Vector2f(display.mpWindow->getSize().x * 0.95f, 620.f), sf::Vector2f(display.mpWindow->getSize().x * 0.5f, 320.f), sf::Color(50, 50, 50, 150), Display::mFont, 15, 50)));

	Display::mGUIManager.registerObject("input", std::shared_ptr<GUIObject>(new Textbox
	("Message", sf::Vector2f(1000.f, 50.f), sf::Vector2f(532.f, 675.f), sf::Color(100, 100, 125, 150), Display::mFont, 15, 50, false, std::vector<char>())));

	Display::mGUIManager.registerObject("quit", std::shared_ptr<GUIObject>(new Button
	("Quit", sf::Vector2f(200.f, 50.f), sf::Vector2f(1148.f, 675.f), sf::Color(100, 125, 100, 150), Display::mFont, 20)));
}

ServerMenu::~ServerMenu()
{

}