//
// Created by romain on 3/1/19.
//

#ifndef ZIA_CONSOLE_H
#define ZIA_CONSOLE_H


#include <unordered_map>
#include <functional>

class Console {
public:
    void awaitCommands();

private:
    static const std::unordered_map<std::string, bool(Console::*)(std::vector<std::string>)> commands;

    bool onExit(std::vector<std::string> = std::vector<std::string>());
    bool onConfig(std::vector<std::string>);
    bool onStart(std::vector<std::string>);
    bool onStop(std::vector<std::string>);
    bool onRestart(std::vector<std::string>);
    bool onLoad(std::vector<std::string>);
    bool onUnload(std::vector<std::string>);
    bool onUnloadAll(std::vector<std::string>);
    bool onHelp(std::vector<std::string>);
};


#endif //ZIA_CONSOLE_H
