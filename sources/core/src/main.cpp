#include <iostream>
#include "Server.h"
#include "Console.h"
#include <set>

#ifdef _WIN32
 #include <windows.h>
 #include <sstream>
 #incldue <string>

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
    std::stringstream s;
    s << "Fatal: Unhandled exception 0x" << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode
    << std::endl;

    std::string message;
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID != 0) {
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        message = std::string(messageBuffer, size);
        LocalFree(messageBuffer);
    }

    MessageBoxA(NULL, (s.str() + " : " + message).c_str(), "zia runtime error", MB_OK | MB_ICONSTOP);

    return EXCEPTION_CONTINUE_SEARCH;
}
#endif


int main(int ac, char *args[]) {
#ifdef _WIN32
    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
#endif
    info("detected platform %s", server.config().platform_str().c_str());
    server.start();

    Console console;
    console.awaitCommands();
    return 0;
}