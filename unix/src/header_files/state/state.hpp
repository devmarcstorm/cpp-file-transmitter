#ifndef STATE_HPP
#define STATE_HPP

#include <string>

#include <SFML/Graphics.hpp>

class Display;

class State
{
public:

	State() {};

	virtual void enter(Display& display) = 0;
	virtual void leave() = 0;

	virtual void handleEvents(Display& display) = 0;
	virtual void update(Display& display) = 0;
	virtual void render(Display& display) = 0;

	~State() {};

	std::string nextState;
};

#endif