#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <map>
#include <memory>

#include "state.hpp"

class StateManager
{
public:

	StateManager();

	void addState(Display& display);
	void setState(std::string name, Display& display);

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~StateManager();

private:

	std::map<std::string, std::shared_ptr<State>> mStates;

	std::shared_ptr<State> mpCurrentState;
};

#endif