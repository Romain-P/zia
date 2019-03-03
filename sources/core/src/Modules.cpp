//
// Created by romain on 3/1/19.
//

#include "Modules.h"
#include "Server.h"
#include "Http.h"
#include <dlfcn.h>

void Modules::load(std::string const &path, ssizet priority) {
    lock_t lock(_locker);
    if (getModule(path) != nullptr) {
        errors("library loader already loaded module %s", path.c_str());
        return;
    } else if (priorityUnavailable(priority)) {
        errors("library loader failed to load module %s due of the priority %d (already used)", path.c_str(), priority);
        return;
    }

    void *library = nullptr;
    std::string fullPath = server.config().modulesPath() + file_util::separator() + path;

    library = dlopen(fullPath.c_str(), RTLD_LAZY);

    if (library == nullptr) {
        errors("library loader failed to load %s (%s)", fullPath.c_str(), dlerror());
        return;
    }

    Module::factory factory = nullptr;
    Module::pointer module = nullptr;
    try {
        factory = (Module::factory) dlsym(library, "factory");
        module = factory();
    } catch (std::exception &e) {
        module = nullptr;
    }

    if (!module) {
        errors("library loader failed when calling the module factory of %s (%s)", path.c_str(), dlerror());
        dlclose(library);
        return;
    }

    _modules.insert({priority, path, library, module});
    info("module %s loaded successfully", module->getName().c_str());
}

void Modules::loadAll() {
    for (auto &it: server.config().modules())
        load(it.first, it.second);
    info("loaded all known modules");
}

void Modules::unload(std::string const &path) {
    lock_t lock(_locker);

    ModuleEntry const *entry;
    if (!(entry = getModule(path))) {
        errors("library loader can't unload the module %s (module not loaded)", path.c_str());
        return;
    }

    unload(*entry);
}

void Modules::unloadAll() {
    lock_t lock(_locker);

    std::set<ModuleEntry> copy(_modules); //avoid concurrent exceptions
    for (auto &entry: copy)
        unload(entry);
    info("unloaded all modules");
}

std::string Modules::dumb() {
    lock_t lock(_locker);

    if (_modules.empty())
        return "[modules] no active module found";

    std::string infos("[modules] active modules: ");

    bool first = true;
    for (auto &it: _modules) {
        if (!first)
            infos += ", ";
        infos += it.instance->getName();
        if (first) first = false;
    }
    infos += "\n";
    return infos;
}

void Modules::unload(const Modules::ModuleEntry &entry) {
    Module::recycler recycler = nullptr;
    std::string moduleName = entry.instance->getName();
    try {
        recycler = (Module::recycler) dlsym(entry.library, "recycler");
        recycler(entry.instance);
    } catch (std::exception &e) {
        errors("library loader failed when calling the module recycler of %s (%s)", entry.path.c_str(), dlerror());
        recycler = nullptr;
    }

    _modules.erase(entry);
    info("module %s unloaded %s", moduleName.c_str(), recycler ? "successfully" : "with possible memory leak.");
}

bool Modules::priorityUnavailable(ssizet priority) const {
    for (auto &module: _modules)
        if (module.priority == priority)
            return true;
    return false;
}

Modules::ModuleEntry const *Modules::getModule(std::string const &path) const {
    for (auto &module: _modules)
        if (module.path == path)
            return &module;
    return nullptr;
}

HookResultType Modules::executePipeline(std::function<HookResultType(Module::pointer)> hook) {
    lock_t lock(_locker);
    Module::pointer module;
    HookResultType result;
    RequestHandler::pointer handler;

    for (auto &entry: _modules) {
        module = entry.instance;
        try {
            result = hook(module);
        } catch(std::exception &e) {
            errors("errors encountered on hook of module %s (%s)", module->getName().c_str(), e.what());
            result = http::code::internal_error;
        }

        if (result != HookResult::Declined)
            return result;
    }
    return http::code::resource_not_found;
}

std::unordered_map<module_name, RequestHandler::pointer> Modules::moduleHandlersFactory() {
    lock_t lock(_locker);
    std::unordered_map<module_name, RequestHandler::pointer> handlers;

    for (auto &entry: _modules) {
        try {
            handlers[entry.instance->getName()] = entry.instance->newRequestHandler();
        } catch (std::exception &e) {
            errors("error while calling request handler factory of module %s (%s)", entry.instance->getName().c_str(), e.what());
            unload(entry);
        }
    }

    return handlers;
}
