#include <iostream>
#include "shared-memory/SHMUsage.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    SHMUsage usage;
    usage.useOne();
    return 0;
}