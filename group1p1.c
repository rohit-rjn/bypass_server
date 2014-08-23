
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/poll.h>

#define MYPORT 3490
#define BACKLOG 10

struct client_desc
{
    int sock_fd;
};
struct message
{
    char sender[1000];
    char recver[1000];
    char msg[1000];
}mesg, mesg1;

struct pollfd sfds[1000];
pthread_t handler[1000];

int main(int argc, char* argv[])
{

    int client_count = 0;
    int sockfd, sockfd2, newfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr, their_addr2;
    struct hostent *h;
    socklen_t sin_size;
    int yes = 1;


    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error\n");
    }
    if((sockfd2 = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error\n");
    }
    their_addr.sin_family = AF_INET;
    int port = atoi(argv[2]);
    their_addr.sin_port = htons(port);
    inet_aton(argv[1], &(their_addr.sin_addr));
    memset(&(their_addr.sin_zero), '\0', 8);

    my_addr.sin_family = AF_INET;
    printf("hell\n");
    my_addr.sin_port = htons(4000);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    //my_addr.sin_addr.s_addr = inet_addr(argv[1]);
    memset(&(my_addr.sin_zero), '\0', 8);
    printf("socket error %s\n", argv[1]);
//    if(bind(sockfd2, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
//    {
//        perror("Bind error\n");
//    }
    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Bind error\n");
    }
//    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
//    {
//        perror("setsockopt");
//        exit(1);
//    }
//
    if(listen(sockfd, BACKLOG) == 1)
    {
        perror("Listen error\n");
    }
    printf("arg %s %d\n", argv[1], ntohs(my_addr.sin_port));
    sin_size = sizeof(struct sockaddr_in);
    while(connect(sockfd, (struct sockaddr *)&their_addr, sin_size) == 0)//bypass-server connected
    {
        char buf[1000];
        int len = recv(sockfd, buf, 1000, 0);
        buf[len] = '\0';
        printf("%s", buf);
        buf[0] = '\0';
        char num[100];
        scanf("%s", num);
        send(sockfd, num, strlen(num), 0);
        sleep(5);
        //close(sockfd);
        int newfd;
        while((newfd = accept(sockfd, (struct sockaddr *)&their_addr2, &sin_size)) > 0)
        {
            printf("Connected to %s : %d\n", inet_ntoa(their_addr2.sin_addr), their_addr2.sin_port);
            printf("Now we communicate\n");
        }
        close(newfd);
    }
}

