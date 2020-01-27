#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SFML/Graphics.hpp>

#include "state/stateManager.hpp"

#include "../header_files/gui/guiManager.hpp"

class Display
{
public:

	Display();

	void run();

	void handleEvents();
	void update();
	void render();

	~Display();

	static GUIManager mGUIManager;
	static sf::Font mFont;

	sf::RenderWindow* mpWindow;

	sf::Event* mpEvent;

private:

	StateManager mManager;
};

#endif