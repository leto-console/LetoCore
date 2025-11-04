/*
 * CommandHandler.cpp
 *
 *  Created on: Dec 28, 2024
 *      Author: Timur
 */

#include "CommandHandler/CommandHandler.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <Data/StaticText.hpp>

StaticList<ConsoleCommand*, 32> CommandHandler::commands;
std::string CommandHandler::line;

void CommandHandler::HandleCurrentCommand()
{
	static StaticList<StaticText32, 16> args;

	int start = -1;
	for (int i = 0; i < (int) line.size(); ++i)
	{
		if (line[i] == ' ' && start != -1)
		{
			args.push_back(line.substr(start, i - start).c_str());
			start = -1;
		}
		else if (line[i] != ' ' && start == -1)
		{
			start = i;
		}
	}

	if (start != -1)
		args.push_back(line.substr(start).c_str());

	line.clear();

	// --------------------------------------------------

	if (args.empty())
		return;

	bool handled = false;
	for (ConsoleCommand* command : commands)
	{
		if (command->CanHandle(args[0]))
		{
			command->Handle(args);
			handled = true;
			break;
		}
	}

	if (!handled)
		printf("command not found\n");

	args.clear();
}

void CommandHandler::RegConsoleCommand(ConsoleCommand* command)
{
	commands.push_back(command);
}

void CommandHandler::Callback(char ch)
{
	if ((ch == '\r' || ch == '\n'))
	{
		HandleCurrentCommand();
		return;
	}
	else if (ch == 0)
	{
		return;
	}
	else if ((ch == '\x08') || (ch == '\x7F'))  // backspace
	{
		line.pop_back();
		return;
	}
	line += ch;
}
