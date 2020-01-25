#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <iostream>

namespace Tools
{
	static std::vector<std::string> split(std::string& string, char delim)
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

	static std::vector<std::string> readDirectory(std::string path)
	{
		std::vector<std::string> pathParts{ split(path, '/') };

		std::vector<std::string> files;

		if (std::filesystem::exists(path))
		{
			for (const auto entry : std::filesystem::recursive_directory_iterator(path))
			{
				std::string file{ entry.path().string() };

				std::replace(file.begin(), file.end(), '\\', '/');

				files.push_back(file);
			}
		}

		return files;
	}

	static std::string concatenate(std::vector<std::string> parts, int count)
	{
		std::string path{ "" };

		if (count > 1)
		{
			for (int i{ 0 }; i < count; i++)
			{
				if (i > 0)
				{
					path += "/" + parts.at(i);
				}
				else
				{
					path += parts.at(i);
				}
			}
		}
		else
		{
			path = parts.at(0);
		}

		return path;
	}
}
#endif