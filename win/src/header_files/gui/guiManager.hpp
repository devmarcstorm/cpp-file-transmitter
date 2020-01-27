#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <map>
#include <string>

#include "guiObject.hpp"

class GUIManager
{
public:

	GUIManager();

	void clear();

	void registerObject(std::string name, std::shared_ptr<GUIObject> object);

	std::shared_ptr<GUIObject> getObject(std::string name);

	void handleEvents(Display& display);
	void update(Display& display);
	void render(Display& display);

	~GUIManager();

private:

	std::map<std::string, std::shared_ptr<GUIObject>> mObjects;
	std::map<std::string, std::shared_ptr<GUIObject>>::iterator mObjectsIt;
};

#endif