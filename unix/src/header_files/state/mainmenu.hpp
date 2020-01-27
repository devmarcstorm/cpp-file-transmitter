#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "state.hpp"

class Mainmenu : public State
{
public:

	Mainmenu();

	void enter(Display& display);
	void leave();

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~Mainmenu();

private:

	void createGUI(Display& display);
};

#endif