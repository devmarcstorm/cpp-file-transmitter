#include "../../header_files/gui/textArea.hpp"

#include "../../header_files/display.hpp"

TextArea::TextArea(sf::Vector2f size, sf::Vector2f position, sf::Color color, sf::Font& font, int fontsize, int maxChars) :
	mFont(font),
	mPosition{ position },
	mFontsize{ fontsize },
	mMax{ maxChars }
{
	mShape.setSize(size);
	mShape.setOrigin(mShape.getGlobalBounds().width * 0.5f, mShape.getGlobalBounds().height * 0.5f);
	mShape.setPosition(position);
	mShape.setFillColor(color);
	mShape.setOutlineThickness(-5.f);
	mShape.setOutlineColor(sf::Color(100, 100, 100));
}

void TextArea::clear()
{
	mMutex.lock();

	mTexts.clear();

	mMutex.unlock();
}

void TextArea::addLine(std::string line, sf::Color color)
{
	if (line != "")
	{
		mMutex.lock();

		if (line == "!clear" || line == "!c")
		{
			clear();
		}
		else
		{
			sf::Text newLine;

			newLine.setFont(mFont);
			newLine.setString(line);
			newLine.setCharacterSize(mFontsize);

			float xPos = mPosition.x - (mShape.getSize().x / 2) + 15;
			float yPos = mPosition.y + (mShape.getSize().y / 2) - mFontsize - 15;

			newLine.setPosition(xPos - mText.getGlobalBounds().width * 0.5f, yPos);

			std::vector<sf::Text>::iterator It;

			for (It = mTexts.begin(); It != mTexts.end(); )
			{
				It->setPosition(It->getPosition().x, It->getPosition().y - (mFontsize * 1.25f));

				if (It->getPosition().y < mPosition.y - (mShape.getSize().y / 2))
				{
					It = mTexts.erase(It);
				}
				else
				{
					++It;
				}
			}

			newLine.setFillColor(color);

			mTexts.push_back(newLine);
		}

		mMutex.unlock();
	}
}

void TextArea::handleEvents(Display& display)
{

}

void TextArea::update(Display& display)
{

}

void TextArea::render(Display& display)
{
	display.mpWindow->draw(mShape);

	mMutex.lock();

	for (mTextsIt = mTexts.begin(); mTextsIt != mTexts.end(); ++mTextsIt)
	{
		display.mpWindow->draw((*mTextsIt));
	}

	mMutex.unlock();
}

TextArea::~TextArea()
{

}