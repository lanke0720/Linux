#include <stdio.h>
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
int main(int argc, char *argv[])
{
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock < 0)
    {
        perror("socket");
        return 1;
    }
    if (argc != 3) //·ÀÖ¹ÊäÈë´íÎó
    {
        usag(argv[0]);
        return 1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    
    if (connect(client_sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("connect");
        return 2;
    }

    char buf[1024];
    while(1)
    {
        printf("send# ");
        fflush(stdout);

        ssize_t s = read(0, buf, sizeof(buf)-1);
        if (s < 0)
        {
            perror("read");
            return 3;
        }
        buf[s-1] = 0;
        write(client_sock, buf, s);
    }
    close(client_sock);
    return 0:
    
}
