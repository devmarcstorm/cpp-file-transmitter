#include "../../header_files/gui/button.hpp"

#include "../../header_files/display.hpp"

Button::Button(std::string text, sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, int fontsize) :
	mColor{ color },
	mHightlightColor{ color + sf::Color(50, 50, 50) }
{
	mShape.setSize(size);
	mShape.setOrigin(mShape.getGlobalBounds().width * 0.5f, mShape.getGlobalBounds().height * 0.5f);
	mShape.setPosition(position);
	mShape.setFillColor(color);
	mShape.setOutlineThickness(-3.f);
	mShape.setOutlineColor(mHightlightColor);

	mText.setFont(font);
	mText.setString(text);
	mText.setCharacterSize(fontsize);
	mText.setFillColor(sf::Color(225, 225, 225));
	mText.setPosition(position.x - mText.getGlobalBounds().width * 0.5f, position.y - mText.getGlobalBounds().height / 1.25f);

	mCurrentState = GUIObjectStates::NORMAL;
}

void Button::handleEvents(Display& display)
{
	if (sf::Mouse::getPosition(*display.mpWindow).x > mShape.getPosition().x - (mShape.getSize().x * 0.5f) && sf::Mouse::getPosition(*display.mpWindow).x < mShape.getPosition().x + (mShape.getSize().x * 0.5f))
	{
		if (sf::Mouse::getPosition(*display.mpWindow).y > mShape.getPosition().y - (mShape.getSize().y * 0.5f) && sf::Mouse::getPosition(*display.mpWindow).y < mShape.getPosition().y + (mShape.getSize().y * 0.5f))
		{
			mShape.setFillColor(mHightlightColor);

			if (mCurrentState == GUIObjectStates::NORMAL)
			{
				mCurrentState = GUIObjectStates::HOVER;
			}

			if (display.mpEvent->type == sf::Event::MouseButtonPressed && display.mpEvent->mouseButton.button == sf::Mouse::Left)
			{
				mCurrentState = GUIObjectStates::CLICKED;
			}

			if (mCurrentState == GUIObjectStates::CLICKED && display.mpEvent->type == sf::Event::MouseButtonReleased && display.mpEvent->mouseButton.button == sf::Mouse::Left)
			{
				mCurrentState = GUIObjectStates::SELECTED;
			}
		}
		else
		{
			mShape.setFillColor(mColor);

			mCurrentState = GUIObjectStates::NORMAL;
		}
	}
	else
	{
		mShape.setFillColor(mColor);

		mCurrentState = GUIObjectStates::NORMAL;
	}
}

void Button::update(Display& display)
{

}

void Button::render(Display& display)
{
	display.mpWindow->draw(mShape);
	display.mpWindow->draw(mText);
}

Button::~Button()
{

}