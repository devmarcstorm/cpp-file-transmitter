#ifndef PANEL_HPP
#define PANEL_HPP

#include "guiObject.hpp"

class Panel : public GUIObject
{
public:

	Panel(sf::Vector2f size, sf::Vector2f position, sf::Color color);
	Panel(sf::Vector2f size, sf::Vector2f position, std::string texturePath);

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~Panel();

private:

	sf::Texture mTexture;
};

#endif