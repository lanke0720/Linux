#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void myhandler(int sec)
{}

int mysleep(int second)
{
    int ret;
    int test = 100;  //���Ϊ����֤pause ����ʲô��
    sigset_t sig;  //ǰ�������C���Ա�̹淶������Ҫ����ı�����ǰ�档
    struct sigaction new, old;  //����������������Ϊ�����ע�ᴦ��������ֵ�ġ�
    new.sa_handler = myhandler;  //���ýṹ�壨�ڶ����ź�ʱ�������ô���ʽ��ע��ָ�����ͣ���
    new.sa_flags = 0;   //Ĭ�ϸ�0
    sigemptyset(&sig); //��ʼ���źż�����Ϊ����Ҫ�����κ��źš�
    new.sa_mask = sig; // ��Ȼ�������������κ��źŵ� ���źż�������ṹ����

    sigaction(SIGALRM, &new, &old);   //ע�� ����������
    alarm(second);      //����һ�����ӣ������Ѿ������������ܣ��ڽ������ź�SIGALRM
    test = pause();    //�ܵ���һ��������͹����ˣ�����Ҫ�ȴ��źš�
    printf("%d\n", test);// -1����������������źţ�����������ִ�С�֤��������ܵ���SIGALRM
    ret = alarm(0);   /*ȡ�����ӡ�//������������棬ȡ����������ǿ�����ͨ�ģ�������������������ط��Ӵ����أ�����˵�������ӵ�ͬʱ���������������Ĵ��룬���ӵ��쳣������ܶ����⣬Ϊ�˱�д��������������Ҫ����ȡ�����ӵġ�*/
    sigaction(SIGALRM, &old, NULL);//�ָ�Ĭ�ϵĴ�������
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
