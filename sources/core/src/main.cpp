#include <iostream>
#include "Server.h"
#include "Console.h"
#include <set>

#ifdef _WIN32
 #include <windows.h>
 #include <sstream>

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
std::stringstream s;
s << "Fatal: Unhandled exception 0x" << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode
<< std::endl;

MessageBoxA(NULL, s.str().c_str(), "my application", MB_OK | MB_ICONSTOP);
exit(1);

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