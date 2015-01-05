#include "apue.h"
#include <pthread.h>

static pthread_cond_t pwait = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t plock = PTHREAD_MUTEX_INITIALIZER;
static int N;//哲学家数
static int nsecs = 2; //思考的时间，缺省为2s
int *forks;

int isAvailable(int i)
{
    if(forks[i] && forks[(i+1) % N])
        return 1;
    else
        return 0;
}

void takeFork(int i)
{
    forks[i] = 0;
    forks[(i+1) % N] = 0;
}
void putFork(int i)
{
    forks[i] = 1;
    forks[(i+1) % N] = 1;
}

void thinking(int i, int nsecs)
{
    printf("philosopher %d is thinking\n", i);
    sleep(nsecs);
}
void eating(int i, int nsecs)
{
    printf("philosopher %d is eating\n", i);
    sleep(nsecs);
}

void *philosopher(void *arg)
{
   // pthread_detach(pthread_self());
    int i = (int)arg;
    while(1)
    {
        thinking(i, nsecs);
        pthread_mutex_lock(&plock);
        while(!isAvailable(i)){
            pthread_cond_wait(&pwait, &plock);
        }
        takeFork(i);
        pthread_mutex_unlock(&plock);
        eating(i, nsecs);
        putFork(i);
        pthread_cond_broadcast(&pwait);
    }
}


int main(int argc, const char *argv[])
{
    int err;
    pthread_t tid[20];

    if((argc != 2 && argc != 4) || (argc == 4 && (strcmp(argv[2], "-t") != 0))){
        err_quit("usage: philosopher_th <N> [-t <time>]");
    }

    N = atoi(argv[1]);
    if(N < 2 || N > 20){
        err_quit("N outranged!");
    }
    if(argc == 4){
        nsecs = atoi(argv[3]);
    }
    //init forks
    forks = (int *)malloc(N * sizeof(int));
    for(int i = 0; i < N; ++i) forks[i] = 1;

    for(int i = 0; i < N; ++i){
        err = pthread_create(&tid[i], NULL, philosopher, (void*)i);
        if(err != 0){
            err_quit("can't create thread\n");
        }
    }

    for(int i = 0; i< N; ++i){
        pthread_join(tid[i], NULL);
    }

    return 0;

}


