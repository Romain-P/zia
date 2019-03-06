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

        ModuleEntry(ssizet priority_, std::string const &path_, void *library_, Module::pointer instance_);
        ~ModuleEntry();
    };

    struct SafeModuleContext {
        /* in order removing handlers and then modules to avoid potential invalid memory access */
        ~SafeModuleContext() {
            handlers.clear();
            modules.clear();
        }

        std::unordered_map<module_name, RequestHandler::pointer> handlers;
        std::vector<std::shared_ptr<ModuleEntry>> modules;
    };

    void load(std::string const &path, ssizet priority);
    void loadAll();
    void unload(std::string const &path);
    void unloadAll();
    void configReloaded();
    std::string dumb();

public: /* hooks */
    HookResultType executePipeline(SafeModuleContext const &ctx, std::function<HookResultType(RequestHandler::pointer)> const &hook);
    SafeModuleContext newModuleContext();

private:
    void unload(std::shared_ptr<ModuleEntry> module);
    bool priorityUnavailable(ssizet priority) const;
    std::shared_ptr<ModuleEntry> getModule(std::string const &path) const;

private:
    static bool comparator(std::shared_ptr<Modules::ModuleEntry> const &a, std::shared_ptr<Modules::ModuleEntry> const &b) {
        return a->priority < b->priority;
    }

    std::set<std::shared_ptr<ModuleEntry>, decltype(&Modules::comparator)> _modules = decltype(_modules)(&comparator);
    std::mutex _locker;
};

#endif //ZIA_MODULES_H
