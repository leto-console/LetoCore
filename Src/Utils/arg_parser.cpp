#include <Utils/arg_parser.hpp>

ArgParser::ArgParser(int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			if ((i + 1) < argc &&
				argv[i + 1][0] != '-')
			{
				key_value[argv[i]] = argv[i + 1];
				i++;
				continue;
			}
			key_value[argv[i]] = "";
		}
	}
}

bool ArgParser::FindArg(const std::string& arg, std::string* value)
{
	auto it = key_value.find(arg);
	bool founded = (it != key_value.end());
	if (founded && value)
		*value = it->second;
	return founded;
}

std::string ArgParser::FindValue(const std::string& arg, const std::string& default_value)
{
	std::string value;
	if (FindArg(arg, &value))
		return value;
	return default_value;
}
