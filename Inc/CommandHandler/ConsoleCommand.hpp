/*
 * ConsoleCommand.hpp
 *
 *  Created on: Dec 03, 2025
 *      Author: Timur
 */

#ifndef INC_COMMANDHANDLER_CONSOLECOMMAND_HPP_
#define INC_COMMANDHANDLER_CONSOLECOMMAND_HPP_

#include "LibrariesExport.h"

#include <Data/StaticText.hpp>
#include <Data/StaticListView.hpp>

class LIBRARIES_EXPORT ConsoleCommand
{
protected:
    StaticText32 name;	// Наименование команды в формате CLI (без пробелов)
    StaticText32 descr;	// Описание команды
public:
    ConsoleCommand(const StaticText32& name, const StaticText32& descr)
        : name{ name }, descr{ descr }
    {
    }
    virtual ~ConsoleCommand() = default;

    bool CanHandle(const StaticText32& command) const { return command == name; }
    StaticText32 GetName() const { return name; }
    StaticText32 GetDescription() const { return descr; }

    // Аргументы вместе с названием команды
    virtual void Handle(const StaticListView<StaticText32>& args) = 0;
};

#endif /* INC_COMMANDHANDLER_COMMANDHANDLER_HPP_ */
