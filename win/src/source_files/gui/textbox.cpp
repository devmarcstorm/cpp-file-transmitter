#include "../../header_files/gui/textbox.hpp"

#include "../../header_files/display.hpp"

Textbox::Textbox(std::string text, sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, int fontsize, int maxChars, bool centered, std::vector<char>& allowedChars) :
	mColor{ color },
	mHightlightColor{ color + sf::Color(50, 50, 50) },
	mPosition{ position },
	mAllowedChars{ allowedChars },
	mMax{ maxChars },
	mCentered{ centered }
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

	if (mCentered)
	{
		mText.setPosition(position.x - mText.getGlobalBounds().width * 0.5f, position.y - mText.getGlobalBounds().height / 1.25f);
	}
	else
	{
		mText.setPosition((position.x - mShape.getSize().x / 2) + 15, position.y - mText.getGlobalBounds().height / 1.25f);
	}

	mCurrentState = GUIObjectStates::NORMAL;
}

void Textbox::handleEvents(Display& display)
{
	if (mCurrentState != GUIObjectStates::SELECTED)
	{
		if (mCurrentState == GUIObjectStates::TEXT_ENTERED)
		{
			mCurrentState = GUIObjectStates::SELECTED;
		}
		else
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
						mText.setString("");

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
	}
	else
	{
		// deselecting
		if (sf::Mouse::getPosition(*display.mpWindow).x < mShape.getPosition().x - (mShape.getSize().x * 0.5f) || sf::Mouse::getPosition(*display.mpWindow).x > mShape.getPosition().x + (mShape.getSize().x * 0.5f) ||
			sf::Mouse::getPosition(*display.mpWindow).y < mShape.getPosition().y - (mShape.getSize().y * 0.5f) || sf::Mouse::getPosition(*display.mpWindow).y > mShape.getPosition().y + (mShape.getSize().y * 0.5f))
		{
			if (display.mpEvent->type == sf::Event::MouseButtonReleased && display.mpEvent->mouseButton.button == sf::Mouse::Left)
			{
				mCurrentState = GUIObjectStates::NORMAL;
			}
		}

		// text manipulation
		if (display.mpEvent->type == display.mpEvent->TextEntered)
		{
			std::string Input{ "" };

			if (display.mpEvent->text.unicode == 13)
			{
				mCurrentState = GUIObjectStates::TEXT_ENTERED;
			}
			else if (display.mpEvent->text.unicode == 8)
			{
				Input = mText.getString();

				if (Input.size() >= 1)
				{
					Input.resize(Input.size() - 1);

					mText.setString(Input);

					if (mCentered)
					{
						mText.setPosition(mPosition.x - mText.getGlobalBounds().width / 2, mText.getPosition().y);
					}
				}
			}
			else
			{
				if (mText.getString().getSize() < mMax)
				{
					if (!mAllowedChars.empty())
					{
						for (int i = 0; i < mAllowedChars.size(); i++)
						{
							if (display.mpEvent->text.unicode == mAllowedChars.at(i))
							{
								Input += display.mpEvent->text.unicode;
							}
						}
					}
					else
					{
						Input += display.mpEvent->text.unicode;
					}

					mText.setString(mText.getString() + Input);

					if (mCentered)
					{
						mText.setPosition(mPosition.x - mText.getGlobalBounds().width / 2, mText.getPosition().y);
					}
				}
			}
		}
	}
}

void Textbox::update(Display& display)
{

}

void Textbox::render(Display& display)
{
	display.mpWindow->draw(mShape);
	display.mpWindow->draw(mText);
}

Textbox::~Textbox()
{

}