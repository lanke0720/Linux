#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void myhandler(int sec)
{}

int mysleep(int second)
{
    int ret;
    int test = 100;  //这句为了验证pause 干了什么。
    sigset_t sig;  //前两句符合C语言编程规范，把需要定义的变量放前面。
    struct sigaction new, old;  //设置两个处理动作，为后面的注册处理函数而设值的。
    new.sa_handler = myhandler;  //设置结构体（内读到信号时所）调用处理方式（注意指针类型）。
    new.sa_flags = 0;   //默认给0
    sigemptyset(&sig); //初始化信号集，因为不需要屏蔽任何信号。
    new.sa_mask = sig; // （然后把这个不屏蔽任何信号的 ）信号集给这个结构体内

    sigaction(SIGALRM, &new, &old);   //注册 处理函数动作
    alarm(second);      //设置一个闹钟，秒数已经给定，继续跑，在结束发信号SIGALRM
    test = pause();    //跑到这一步，程序就挂起了，就需要等待信号。
    printf("%d\n", test);// -1，如果不给结束的信号，处理动作向下执行。证明这里接受的是SIGALRM
    ret = alarm(0);   /*取消闹钟。//在这个程序里面，取消这个闹钟是可以跑通的，但是如果我们在其他地方加代码呢？比如说在有闹钟的同时，我们在跑其他的代码，闹钟的异常会引起很多问题，为了编写优良，我们是需要加上取消闹钟的。*/
    sigaction(SIGALRM, &old, NULL);//恢复默认的处理动作。
    return ret;  //0 
}
int main()
{
    while(1)
    {
        printf("oh,no,wait me 3s more!\n");
        mysleep(3);
    }
    return 0;
}
