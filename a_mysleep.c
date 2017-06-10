
#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void myhandler(int sec)
{}

int mysleep(int second)
{
    int ret;
    struct sigaction new, old;
    sigset_t newmask, oldmask, smask;

    new.sa_handler = myhandler;
    sigemptyset(&new.sa_mask);
    new.sa_flags = 0;
    sigaction(SIGALRM, &new, &old);

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    alarm(second);
    smask = oldmask;
    sigdelset(&smask, SIGALRM);
    sigsuspend(&smask);
    ret = alarm(0);
    sigaction(SIGALRM, &old, NULL);
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    return ret;

}

int main()
{
    while(1)
    {
        mysleep(3);
        printf("oh,no,wait me 3s more!\n");
    }
    return 0;
}
