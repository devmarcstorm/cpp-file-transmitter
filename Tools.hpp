#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>
#include <sstream>

namespace Tools
{
	static std::vector<std::string> Splitting(std::string &string, char delim)
	{
		std::vector<std::string> Parts;

		std::stringstream SS(string);

		std::string Item;

		while (std::getline(SS, Item, delim))
		{
			Parts.push_back(Item);
		}

		return Parts;
	}
}

#endif