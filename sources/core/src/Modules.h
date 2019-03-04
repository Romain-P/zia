//
// Created by romain on 3/1/19.
//

#ifndef ZIA_MODULES_H
#define ZIA_MODULES_H

#include <set>
#include "zia.h"

using module_name = std::string;

class Modules {
public:
    struct ModuleEntry {
        ssizet priority;
        std::string path;
        void *library;
        Module::pointer instance;

        //std::set auto sorting
        bool operator<(ModuleEntry const &other) const { return priority < other.priority; }
    };

    void load(std::string const &path, ssizet priority);
    void loadAll();
    void unload(std::string const &path);
    void unloadAll();
    void configReloaded();
    std::string dumb();

public: /* hooks */
    HookResultType executePipeline(std::function<HookResultType(Module::pointer)> hook);
    std::unordered_map<module_name, RequestHandler::pointer> moduleHandlersFactory();

private:
    void unload(ModuleEntry const &module);
    bool priorityUnavailable(ssizet priority) const;
    ModuleEntry const *getModule(std::string const &path) const;

private:
    std::set<ModuleEntry> _modules;
    std::mutex _locker;
};


#endif //ZIA_MODULES_H
