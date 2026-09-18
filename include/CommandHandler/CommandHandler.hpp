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

class LETO_CORE_EXPORT CommandHandler
{
protected:
	// Список зарегистрированных команд
	static StaticList<const ConsoleCommand*, 32> commands;
	static std::string line;

	static void HandleCurrentCommand();

public:
	static void RegConsoleCommand(const ConsoleCommand* command);
	static void Callback(char ch);
};

#endif /* INC_COMMANDHANDLER_COMMANDHANDLER_HPP_ */
