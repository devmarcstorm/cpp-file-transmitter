#ifndef SERVERMENU_HPP
#define SERVERMENU_HPP

#include "state.hpp"

#include "../server/serverThread.hpp"
#include "../client/client.hpp"

class ServerMenu : public State
{
public:

	ServerMenu();

	void enter(Display& display);
	void leave();

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~ServerMenu();

private:

	void createGUI(Display& display);

	bool mBrokenServer;
	bool mBrokenClient;

	ServerThread mServer;
	Client mClient;

	std::promise<void> mExitSignal;

	std::map<std::string, int> mClients;
};

#endif