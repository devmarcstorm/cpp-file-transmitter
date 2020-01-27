#include "../../header_files/gui/guiManager.hpp"

#include <iostream>

GUIManager::GUIManager()
{

}


void GUIManager::clear()
{
	mObjects.clear();
}

void GUIManager::registerObject(std::string name, std::shared_ptr<GUIObject> object)
{
	mObjects.insert(std::pair<std::string, std::shared_ptr<GUIObject>>(name, object));
}

std::shared_ptr<GUIObject> GUIManager::getObject(std::string name)
{
	std::map<std::string, std::shared_ptr<GUIObject>>::iterator It;

	It = mObjects.find(name);

	if (It != mObjects.end())
	{
		return It->second;
	}
	else
	{
		std::cout << "No guiObject with name: " << name << std::endl;

		std::string error("No guiObject with name: " + name);

		throw std::exception(error.c_str());
	}
}

void GUIManager::handleEvents(Display& display)
{
	for (mObjectsIt = mObjects.begin(); mObjectsIt != mObjects.end(); ++mObjectsIt)
	{
		mObjectsIt->second->handleEvents(display);
	}
}

void GUIManager::update(Display& display)
{
	for (mObjectsIt = mObjects.begin(); mObjectsIt != mObjects.end(); ++mObjectsIt)
	{
		mObjectsIt->second->update(display);
	}
}

void GUIManager::render(Display& display)
{
	for (mObjectsIt = mObjects.begin(); mObjectsIt != mObjects.end(); ++mObjectsIt)
	{
		mObjectsIt->second->render(display);
	}
}

GUIManager::~GUIManager()
{

}