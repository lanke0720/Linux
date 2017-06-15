#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
void creat_daemon()
{
    umask(0);
    if (fork() > 0)
    {
        exit(-1);
    }
    setsid();
    chdir("/");
    close(0);
    close(1);
    close(2);
    signal(SIGCHLD, SIG_IGN);
}

int main()
{
    creat_daemon();
    //daemon(0,0);
    while(1);
    return 0;
}
