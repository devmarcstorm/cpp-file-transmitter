#ifndef PREMENU_CLIENT_HPP
#define PREMENU_CLIENT_HPP

#include "state.hpp"

class PremenuClient : public State
{
public:

	PremenuClient();

	void enter(Display& display);
	void leave();

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~PremenuClient();

private:

	void createGUI(Display& display);
};

#endif