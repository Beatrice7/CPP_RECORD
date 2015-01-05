#include "apue.h"
static void sig_alrm(int signo) {
    /* just return */
}
unsigned int mysleep(unsigned int nsecs) {
    // return sleep(nsecs);
    struct sigaction    newact, oldact;
    sigset_t            newmask, oldmask, suspmask, pendingmask;
    unsigned int        unslept, oldalrm, slept;
    int                 isblock = 0, ispending = 0;
    /* set handler, save previous information */
    newact.sa_handler = sig_alrm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);
    /* has alarm? */
    oldalrm = alarm(0);
    /* is block? */
    sigprocmask(0, NULL, &oldmask);
    if (sigismember(&oldmask, SIGALRM)) {
        isblock = 1;
    }
    /* is pending? */
    if (sigpending(&pendingmask) < 0) {
        err_sys("sigpending error\n");
    }
    if (sigismember(&pendingmask, SIGALRM)) { /* handle the pending signal */
        ispending = 1;
        pendingmask = oldmask;
        sigdelset(&pendingmask, SIGALRM);
        sigsuspend(&pendingmask);
    }
    /* block SIGALRM and save current signal mask */
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    /* sleep */
    if (oldalrm && (oldalrm < nsecs) && !isblock) { /* if has set alarm */
        alarm(oldalrm);
    } else {
        alarm(nsecs);
    }
    suspmask = oldmask;
    sigdelset(&suspmask, SIGALRM); /* make sure SIGALRM isn't blocked */
    sigsuspend(&suspmask); /* wait for any signal to be caught */
    /* signal caught */
    unslept = alarm(0);
    sigaction(SIGALRM, &oldact, NULL); /* reset previous action */
    /* reset signal mask, which unblocks SIGALRM */
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    /* cal the slept time and return the new unslept time */
    if (oldalrm && (oldalrm < nsecs) && !isblock) {
        slept = oldalrm - unslept;
    } else {
        slept = nsecs - unslept;
    }
    /* handle previous interaction */
    if ((oldalrm && (slept >= oldalrm)) ||  (isblock && oldalrm) || ispending) {
        kill(getpid(), SIGALRM);
    }
    if (slept < oldalrm) { /* reset previous alarm */
        alarm(oldalrm - slept);
    }
    return(nsecs - slept);
}