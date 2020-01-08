#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>
#include <sstream>

namespace Tools
{
template <typename Out>
static void Splitting(const std::string &string, char delim, Out result)
{
	std::stringstream SS(string);

	std::string Item;

	while (std::getline(SS, Item, delim))
	{
		*(result++) = Item;
	}
}

static std::vector<std::string> Split(const std::string &string, char delim)
{
	std::vector<std::string> Elements;

	Splitting(string, delim, std::back_inserter(Elements));

	return Elements;
}
} // namespace Tools

#endif