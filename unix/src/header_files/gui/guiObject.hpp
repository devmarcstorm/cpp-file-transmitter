#ifndef GUI_OBJECT_HPP
#define GUI_OBJECT_HPP

#include <SFML/Graphics.hpp>

class Display;

enum class GUIObjectStates
{
	NORMAL,
	HOVER,
	CLICKED,
	SELECTED,
	TEXT_ENTERED
};

class GUIObject
{
public:

	GUIObject() {};

	virtual void handleEvents(Display& display) = 0;
	virtual void update(Display& display) = 0;
	virtual void render(Display& display) = 0;

	~GUIObject() {};

	GUIObjectStates mCurrentState;

	sf::RectangleShape mShape;

	sf::Text mText;
};

#endif