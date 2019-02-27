#include <iostream>
#include "Server.h"

int main(int ac, char *args[]) {
    info("detected platform %s", server.config().platform_str().c_str());
    server.reloadConfig();

    return 0;
}