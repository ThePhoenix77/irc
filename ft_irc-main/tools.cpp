#include "Server.hpp"


std::string& modifie_string(std::string& str, int x)
{
	if (x == 0)
	{
		for (int g = 0; str[g]; g++)
		{
			if (std::isalpha(str[g]))
				str[g] = std::toupper(str[g]);
		}
		return str;
	}
	if (x == 1)
	{
		for (int g = 0; str[g]; g++)
		{
			if (std::isalpha(str[g]))
				str[g] = std::tolower(str[g]);
		}
		return str;
	}
	return str;
}

std::vector<std::string> split(const std::string &input, char delimiter)
{
	std::vector<std::string> arr;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, delimiter))
	{
		arr.push_back(token);
	}

	return arr;
}
