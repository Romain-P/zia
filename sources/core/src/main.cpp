#include <iostream>
#include "Server.h"
#include "Console.h"
#include <set>

int main(int ac, char *args[]) {
    info("detected platform %s", server.config().platform_str().c_str());
    server.start();

    Console console;
    console.awaitCommands();
    return 0;
}