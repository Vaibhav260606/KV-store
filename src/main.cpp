#include "../include/hashtable.h"
#include "../include/server.h"

#include <iostream>

int main()
{
    Server server(6379);

    server.start();

    return 0;
}