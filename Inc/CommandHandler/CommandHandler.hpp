/**
 * @file CommandHandler.hpp
 * @date Dec 28, 2024
 * @author Rakhimov T.
 */

#ifndef INC_COMMANDHANDLER_COMMANDHANDLER_HPP_
#define INC_COMMANDHANDLER_COMMANDHANDLER_HPP_

#include "ConsoleCommand.hpp"
#include "LibrariesExport.h"

#include <string>
#include <Data/StaticList.hpp>

class LIBRARIES_EXPORT CommandHandler
{
protected:
	// Список зарегистрированных команд
	static StaticList<ConsoleCommand*, 32> commands;
	static std::string line;

	static void HandleCurrentCommand();

public:
	static void RegConsoleCommand(ConsoleCommand* command);
	static void Callback(char ch);
};

#endif /* INC_COMMANDHANDLER_COMMANDHANDLER_HPP_ */
