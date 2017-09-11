
#include <iostream>
#include <stdlib.h>

#include "Subscriber.h"
#include "TournamentManager.h"

int main(int argc, char* argv[]) {
    int k, rows, columns;
    k = 7;
    rows = 2;
    columns = 3;
    if (argc == 4) {
        k = atoi(argv[1]);
        rows = atoi(argv[2]);
        columns = atoi(argv[3]);
    }

    std::cout << "Hello, World!" << std::endl;
    Subscriber subscriber;
    TournamentManager manager(k, rows, columns);
    subscriber.add_manager(&manager);
    for (int i = 0; i < 10; i++) {
        subscriber.subscribe_person(i);
    }
    while (!manager.finished()) {
        subscriber.do_job();
        manager.do_job();
    }
    return 0;
}