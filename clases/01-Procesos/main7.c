#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NUMBER_OF_CHILDS 10
#define WAIT_TIME 10

long int rndom() {
    srand(getpid());
    long int result = rand() % WAIT_TIME;
    return result;
}

int main() {
    setbuf(stdout, NULL); //Disable stdout buffer

    int childs = NUMBER_OF_CHILDS;
    printf("Spawning %d process\n" , childs);
    
    for (int i = 0; i < childs; i++) {
        int pid = fork();

        if (pid == 0) {
            printf("Started process with id: %d\n" , getpid());
            unsigned int s = rndom();
            sleep(s);
            printf("Exited %d after -> %d\n",getpid(),s);
            exit(s);
        }

    }

    for (int i = 0; i < childs; i++) {
        unsigned int a;
        int pid = wait(&a);
        printf("Kiled %d after %d (Exit status: %d)\n",pid,WEXITSTATUS(a),a);
    }

    return 0;
}
