#include "apue.h"
#include "lock.h"
#define N 5

static char* forks[N] = {"fork0", "fork1", "fork2", "fork3", "fork4"};
static int nsecs = 2;

void takeFork(int i)
{
    if(i == N-1)
    {
        lock(forks[0]);//right first
        lock(forks[i]);
    }else{
        lock(forks[i]);//left first
        lock(forks[i+1]);
    }
}

void putFork(int i)
{
    if(i == N-1){
        unlock(forks[0]);
        unlock(forks[i]);
    }else{
        unlock(forks[i]);
        unlock(forks[i+1]);
    }
}

void thinking(int i)
{
    printf("philosopher %d is thinking...\n", i);
    sleep(nsecs);
}

void eating(int i)
{
    printf("philosopher %d is eating...\n", i);
    sleep(nsecs);
}

void philosopher(int i)
{
    while(1){
        thinking(i);
        takeFork(i);
        eating(i);
        putFork(i);
    }
}

int main(int argc, const char *argv[])
{
    int i;
    pid_t pid;

    for(i = 0; i < N; i++){
        initlock(forks[i]);
    }

    if(argc == 3 && strcmp(argv[1], "-t") == 0)
    {
        nsecs = atoi(argv[2]);
    }else if(argc != 1){
        err_quit("usage: philosopher [-t <time>]");
    }

    for(i = 0; i < N; ++i){
        pid = fork();
        if(pid == 0)
        {
            philosopher(i);

        }else if (pid < 0){
            err_quit("fork error");
        }
    }

    wait(NULL);
    return 0;
}












