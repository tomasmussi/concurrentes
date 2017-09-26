

#include <stdlib.h>

#include "Subscriber.h"

/*
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

    Subscriber subscriber;
    TournamentManager manager(k, rows, columns);
    subscriber.add_manager(&manager);
    for (int i = 0; i < 10; i++) {
        subscriber.subscribe_person(i);
    }
    while (manager.do_job()) {
        subscriber.do_job();
    }
    manager.get_recorder()->list_all_matches();
    return 0;
}*/

/*
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int f1() {
    sleep(3);
    std::cout << "f1" << std::endl;
    return 1;
}
int f2() {
    sleep(2);
    std::cout << "f2" << std::endl;
    return 2;
}
int f3() {
    sleep(2);
    std::cout << "f3" << std::endl;
    return 3;
}
typedef int (*f)();


int main(int argc, char* argv[]) {
    f arr[3] = {&f1, &f2, &f3};
    arr[0]();
    bool father = true;
    int hijo = 0;
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            father = false;
            std::cout << "hijo " << i << std::endl;
            hijo = arr[i]();
            break;
        } else {
            std::cout << "padre, sigo de largo" << std::endl;
        }
    }
    if (father) {
        for (int i = 0; i < 3; i++) {
            int status;
            pid_t t = wait(&status);
            std::cout << "[" << t << "] termino con: " << WEXITSTATUS(status) << std::endl;
        }
        std::cout << "fin wating children" << std::endl;
    }
    return hijo;
}*/


#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "MainSIGIntHandler.h"
#include "SignalHandler.h"

#include "PeopleRegisterWorker.h"
#include "BeachManagerWorker.h"
#include "SIGIntHandler.h"

#define N_WORKERS 2

int main(int argc, char* argv[]) {
    // MainSIGIntHandler sigint_handler;
    SIGIntHandler sigint_handler;
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );

    WorkerProcess* arr[N_WORKERS] = {new PeopleRegisterWorker(), new BeachManagerWorker()};
    bool is_father = true;
    int son_process = 0;
    for (int i = 0; i < N_WORKERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            is_father = false;
            std::cout << "hijo " << i << std::endl;
            son_process = arr[i]->loop();
            break;
        } else {
            // sigint_handler.add_pid_notification(pid);
            std::cout << "padre, despache: ["<< pid << "]" << std::endl;
        }
    }
    if (is_father) {
        for (int i = 0; i < N_WORKERS; i++) {
            int status;
            std::cout << "llegue al wait (bloqueado)"<< std::endl;
            pid_t t = wait(&status);
            std::cout << "[" << t << "] termino con: " << WEXITSTATUS(status) << std::endl;
        }
        std::cout << "fin waiting children" << std::endl;
    }
    for (int i = 0; i < N_WORKERS; i++) {
        delete arr[i];
    }
    return son_process;
}