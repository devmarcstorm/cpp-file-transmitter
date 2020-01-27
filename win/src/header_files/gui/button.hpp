#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "guiObject.hpp"

class Button : public GUIObject
{
public:

	Button(std::string text, sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, int fontsize);

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~Button();

private:

	sf::Color mColor;
	sf::Color mHightlightColor;
};

#endif