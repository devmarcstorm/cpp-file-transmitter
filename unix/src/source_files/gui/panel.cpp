#include "../../header_files/gui/panel.hpp"

#include "../../header_files/display.hpp"

Panel::Panel(sf::Vector2f size, sf::Vector2f position, sf::Color color)
{
	mShape.setSize(size);
	mShape.setOrigin(mShape.getGlobalBounds().width * 0.5f, mShape.getGlobalBounds().height * 0.5f);
	mShape.setPosition(position);
	mShape.setFillColor(color);
}

Panel::Panel(sf::Vector2f size, sf::Vector2f position, std::string texturePath)
{
	mShape.setSize(size);
	mShape.setOrigin(mShape.getGlobalBounds().width * 0.5f, mShape.getGlobalBounds().height * 0.5f);
	mShape.setPosition(position);

	mTexture.loadFromFile(texturePath);

	mShape.setTexture(&mTexture);
}

void Panel::handleEvents(Display& display)
{

}

void Panel::update(Display& display)
{

}

void Panel::render(Display& display)
{
	display.mpWindow->draw(mShape);
}

Panel::~Panel()
{

}