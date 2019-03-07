//
// Created by romain on 3/4/19.
//

#ifndef ZIA_DYNAMICLIBRARY_H
#define ZIA_DYNAMICLIBRARY_H

#ifdef _WIN32
 #include "windows.h"
#else
#include <dlfcn.h>
#endif

namespace dl {
    inline void *open(char const *path) {
#ifdef _WIN32
        return (void *) LoadLibrary(path);
#else
        return dlopen(path, RTLD_LAZY);
#endif
    }

    inline bool close(void *module) {
#ifdef _WIN32
        return (bool) FreeLibrary((HMODULE) module);
#else
        return (bool) dlclose(module);
#endif
    }

    template<typename T>
    inline T pointer(void *module, char const *name) {
#ifdef _WIN32
        return (T) GetProcAddress((HMODULE) module, name);
#else
        return (T) dlsym(module, name);
#endif
    }

    inline std::string error() {
#ifdef _WIN32
        DWORD errorMessageID = ::GetLastError();
        if(errorMessageID == 0)
            return std::string();

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);

        return message;
#else
        return std::string(dlerror());
#endif
    }

    inline char const *extension() {
#ifdef _WIN32
        return ".dll";
#else
        return ".so";
#endif
    }
}

#endif //ZIA_DYNAMICLIBRARY_H
