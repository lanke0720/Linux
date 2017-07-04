#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

static void usag(const char* argv)
{
    printf("please Enter : %s [IP] [PORT]\n", argv);
}
//1,creat socket
int startup(const char* ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(2);
    }
    //2,creat && fill client space
    struct sockaddr_in local;  //breause: AF_INET
    local.sin_family = AF_INET;
    local.sin_port = htons(port);//16 bit port
    local.sin_addr.s_addr = inet_addr(ip);

    printf("sockfd:%d\n", sockfd);
    //3,bind socket
    if (bind(sockfd, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
        perror("bind");
        exit(3);
    }
    //4,set listen
    if ((listen(sockfd, 5)) < 0)
    {
        perror("listen");
        exit(4);
    }
    return sockfd;
}

int main(int argc, char *argv[])
{
    if (argc != 3) //·ÀÖ¹ÊäÈë´íÎó
    {
        usag(argv[0]);
        return 1;
    }

    int listen_sock = startup(argv[1], atoi(argv[2]));


    //signal pro set accept 
    while(1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);//B:accept() third num is out&&in
        int new_sock = accept(listen_sock, (struct sockaddr*)&client,\
                             &len);//get client info
        if (new_sock < 0)
        {
            perror("accept");
            continue;
        }
        //print client info
        printf("get a new client, ip is %s, port is %d\n",\
              inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        
        //set cache
        while(1)
        {
            char buf[1024];
            ssize_t s = read(new_sock, buf, sizeof(buf)-1);
            if(s > 0)
            {
                buf[s] = 0;
                printf("client# %s\n", buf);
            }
            else if(s == 0)
            {
                printf("client is quit!\n");
                break;
            }
            else
            {
                perror("read");
                break;
            }
        }
        close(new_sock);
    }
    return 0;
}
