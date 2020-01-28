#include "../../header_files/state/stateManager.hpp"

#include "../../header_files/state/mainmenu.hpp"
#include "../../header_files/state/premenuClient.hpp"
#include "../../header_files/state/clientMenu.hpp"
#include "../../header_files/state/serverMenu.hpp"

#include <iostream>

StateManager::StateManager()
{

}

void StateManager::addState(Display& display)
{
	mStates.insert(std::pair<std::string, std::shared_ptr<State>>("mainmenu", std::shared_ptr<State>(new Mainmenu)));
	mStates.insert(std::pair<std::string, std::shared_ptr<State>>("premenuClient", std::shared_ptr<State>(new PremenuClient)));
	mStates.insert(std::pair<std::string, std::shared_ptr<State>>("clientMenu", std::shared_ptr<State>(new ClientMenu)));
	mStates.insert(std::pair<std::string, std::shared_ptr<State>>("serverMenu", std::shared_ptr<State>(new ServerMenu)));

	setState("mainmenu", display);
}

void StateManager::setState(std::string name, Display& display)
{
	std::map<std::string, std::shared_ptr<State>>::iterator It;

	It = mStates.find(name);

	if (It != mStates.end())
	{
		if (mpCurrentState != nullptr)
		{
			mpCurrentState->leave();
		}

		mpCurrentState = It->second;

		mpCurrentState->enter(display);
	}
	else
	{
		std::cout << "No state with name: " << name << std::endl;

		std::string error("No state with name: " + name);
	}
}

void StateManager::handleEvents(Display& display)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->handleEvents(display);
	}
}

void StateManager::update(Display& display)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->update(display);
	}
}

void StateManager::render(Display& display)
{
	if (mpCurrentState != nullptr)
	{
		mpCurrentState->render(display);

		if (mpCurrentState->nextState != "")
		{
			setState(mpCurrentState->nextState, display);
		}
	}
}

StateManager::~StateManager()
{

}