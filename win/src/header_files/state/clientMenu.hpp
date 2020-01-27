#ifndef CLIENTMENU_HPP
#define CLIENTMENU_HPP

#include "state.hpp"

#include "../client/client.hpp"

class ClientMenu : public State
{
public:

	ClientMenu();

	void enter(Display& display);
	void leave();

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~ClientMenu();

private:

	void createGUI(Display& display);

	Client mClient;

	bool mBroken;
};

#endif