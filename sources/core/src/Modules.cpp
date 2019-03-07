//
// Created by romain on 3/1/19.
//

#include <memory>
#include "Modules.h"
#include "Server.h"
#include "Http.h"
#include "DynamicLibrary.h"
#include "zia.h"

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
    std::string fullPath = server.config().modulesPath() + file_util::separator() + path + dl::extension();

    library = dl::open(fullPath.c_str());

    if (library == nullptr) {
        errors("library loader failed to load %s (%s)", fullPath.c_str(), dl::error().c_str());
        return;
    }

    Module::factory factory = nullptr;
    Module::pointer module = nullptr;
    try {
        factory = dl::pointer<Module::factory>(library, "factory");
        module = factory();
    } catch (std::exception &e) {
        module = nullptr;
    }

    if (!module) {
        errors("library loader failed when calling the module factory of %s (%s)", path.c_str(), dl::error().c_str());
        dl::close(library);
        return;
    }

    auto modulePtr = std::make_shared<ModuleEntry>(priority, path, library, module);
    _modules.insert(modulePtr);

    try {
        module->onActivate(server.sharedConfig());
        module->onConfigChange(server.sharedConfig());
        info("module %s loaded successfully", module->getName().c_str());
    } catch(std::exception &e) {
        errors("module %s can't start (%s)", module->getName().c_str(), e.what());
        unload(modulePtr);
    }
}

void Modules::loadAll() {
    for (auto &it: server.config().modules())
        load(it.first, it.second);
    info("loaded all known modules");
}

void Modules::unload(std::string const &path) {
    lock_t lock(_locker);

    std::shared_ptr<ModuleEntry> entry;
    if (!(entry = getModule(path)).get()) {
        errors("library loader can't unload the module %s (module not loaded)", path.c_str());
        return;
    }

    unload(entry);
}

void Modules::unloadAll() {
    lock_t lock(_locker);

    std::vector<std::shared_ptr<ModuleEntry>> copy(_modules.begin(), _modules.end()); //avoid concurrent exceptions
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
        infos += it->instance->getName();
        if (first) first = false;
    }
    infos += "\n";
    return infos;
}

void Modules::unload(std::shared_ptr<ModuleEntry> entry) {
    std::string moduleName = entry->instance->getName();
    _modules.erase(entry);
    info("module %s unloaded successfully", moduleName.c_str());
}

void Modules::configReloaded() {
    lock_t lock(_locker);

    for (auto &module: _modules) {
        try {
            module->instance->onConfigChange(server.sharedConfig());
        } catch(std::exception &e) {
            errors("module %s can't load the config (%s)", module->instance->getName().c_str(), e.what());
            unload(module);
        }
    }
}

bool Modules::priorityUnavailable(ssizet priority) const {
    for (auto &module: _modules)
        if (module->priority == priority)
            return true;
    return false;
}

std::shared_ptr<Modules::ModuleEntry> Modules::getModule(std::string const &path) const {
    for (auto &module: _modules)
        if (module->path == path)
            return module;
    return std::shared_ptr<ModuleEntry>(nullptr);
}

HookResultType Modules::executePipeline(SafeModuleContext const &ctx, std::function<HookResultType(RequestHandler::pointer)> const &hook) {
    Module::pointer module;
    HookResultType result;
    RequestHandler::pointer handler;

    for (auto &entry: ctx.modules) {
        module = entry->instance;

        auto it = ctx.handlers.find(module->getName());
        if (it == ctx.handlers.end())
            return HookResult::Declined; /* skip modules added during a request */

        RequestHandler::pointer requestHandler = it->second;

        try {
            result = hook(requestHandler);
        } catch(std::exception &e) {
            errors("errors encountered on hook of module %s (%s)", module->getName().c_str(), e.what());
            result = http::code::internal_error;
        }

        if (result != HookResult::Declined)
            return result;
    }
    return http::code::resource_not_found;
}

Modules::SafeModuleContext Modules::newModuleContext() {
    lock_t lock(_locker);
    std::unordered_map<module_name, RequestHandler::pointer> handlers;
    std::vector<std::shared_ptr<ModuleEntry>> safeCopy;

    for (auto &entry: _modules) {
        try {
            handlers[entry->instance->getName()] = entry->instance->newRequestHandler();
            safeCopy.push_back(entry);
        } catch (std::exception &e) {
            errors("error while calling request handler factory of module %s (%s)", entry->instance->getName().c_str(), e.what());
            unload(entry);
        }
    }

    return {std::move(handlers), std::move(safeCopy)};
}

Modules::ModuleEntry::~ModuleEntry() {
    try {
        instance->onDeactivate();
        auto recycler = dl::pointer<Module::recycler>(library, "recycler");
        recycler(instance);
    } catch (std::exception &e) {
        errors("library loader failed when calling the module recycler of %s (%s)", path.c_str(), dl::error().c_str());
    }
    dl::close(library);
}

Modules::ModuleEntry::ModuleEntry(ssizet priority_, std::string const &path_, void *library_, Module::pointer instance_)
        : priority(priority_), path(path_), library(library_), instance(instance_) {}
