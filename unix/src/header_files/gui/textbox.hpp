#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "guiObject.hpp"

class Textbox : public GUIObject
{
public:

	Textbox(std::string text, sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, int fontsize, int maxChars, bool centered, std::vector<char> allowedChars);

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~Textbox();

private:

	sf::Color mColor;
	sf::Color mHightlightColor;

	sf::Vector2f mPosition;

	std::vector<char> mAllowedChars;

	int mMax;

	bool mCentered;
};

#endif