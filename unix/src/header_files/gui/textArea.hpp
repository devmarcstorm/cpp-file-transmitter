#ifndef TEXT_AREA_HPP
#define TEXT_AREA_HPP

#include <mutex>

#include "guiObject.hpp"

class TextArea : public GUIObject
{
public:

	TextArea(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, int fontsize, int maxChars);

	void clear();

	void addLine(std::string line, sf::Color color);

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~TextArea();

private:

	sf::Font& mFont;

	std::vector<sf::Text> mTexts;
	std::vector<sf::Text>::iterator mTextsIt;

	sf::Vector2f mPosition;

	int mFontsize;

	int mMax;

	std::mutex mMutex;
};

#endif