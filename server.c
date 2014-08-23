#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<pthread.h>
#include <sys/poll.h>


#define MYPORT 3490
#define BACKLOG 10

struct client_desc
{
    int sfd;
    char IPaddr[1000];
    char name[1000];
} client[1000];

struct message
{
    char sender[1000];
    char recver[1000];
    char msg[1000];
} mesg;

struct group2info
{
    char IPaddr[100];
    int port;
} g2i[100];

int numClients = 0;
struct pollfd sfds[100];
pthread_t handler;
int fd_max;

fd_set original, read_fds;
//
//void func(void* s)
//{
//    struct timeval tv;
//    tv.tv_sec = 1;
//    tv.tv_usec = 0;
//    char snd[1000];
//    for( ; ; )
//    {
//        read_fds = original;
//        select(fd_max+1, &read_fds, NULL, NULL, &tv);
//        int i=0;
//        for(i=0; i<fd_max+1; i++)
//        {
//            if(FD_ISSET(i, &read_fds))
//            {
//                //strcpy(snd, client[i].name);
//                //strcat(snd, " sent :  ");
//                int len = recv(i, &mesg, sizeof(mesg), 0);
//                printf("Got message  %s", mesg.msg);
//                int j=0;
//                //if(strcmp(mesg.recver, ":all")==0)
//                //{
//                for(j=0; j<fd_max+1; j++)
//                {
//                    if(FD_ISSET(j, &original) && j!=i)
//                    {
//                        send(j, &mesg, sizeof(mesg), 0);
//                    }
//                }
//                //}
////                else
////                {
////                    for(j=0; j<numClients; j++)
////                    {
////                        if(strcmp(client[j].name, mesg.recver)==0)//message is meant for a specific receiver..
////                        {
////                            send(sfds[j].fd, &mesg, sizeof(mesg), 0);
////                            break;
////                        }
////                    }
////                }
//            }
//        }
//    }
//}
int PORT = 3490;

int main()
{
    int i;
    for(i=1; i<=5; i++)
    {
        strcpy(g2i[i].IPaddr, "127.0.0.1");
        g2i[i].port = PORT + i;
    }
    char hostname[1000];
    gethostname(hostname, 1000);
    printf("%s\n", hostname);
    int sockfd1, sockfd2, newfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    int yes = 1;

    if((sockfd1 = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error\n");
    }
    if((sockfd2 = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error\n");
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);
    if(bind(sockfd1, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Bind error\n");
    }
    if (setsockopt(sockfd1,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    if(listen(sockfd1, BACKLOG) == 1)
    {
        perror("Listen error\n");
    }
    sin_size = sizeof(struct sockaddr_in);
   // pthread_create(&handler, NULL, (void*)&func, NULL);

    while((newfd = accept( sockfd1, (struct sockaddr *)&their_addr, &sin_size)) > 0)
    {
        sin_size = sizeof(struct sockaddr_in);
        printf("Server: got connection from %s %d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));
        if(send(newfd, "Top of the morning, from your bypass-server !, please send group-2 process number..\n", strlen("Top of the morning, from your server !, please send your screen-name..\n"), 0) == -1)
            perror("Send error\n");

        char buf[5];
        int len = recv(newfd, buf, 100, 0);

        if(len > 0)
        {
            //close(newfd);
            buf[len] = '\0';
            struct sockaddr_in addr;
            int g2num = atoi(buf);
            close(newfd);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(g2i[g2num].port);
//            addr.sin_addr.s_addr = inet_addr(g2i[g2num].IPaddr);
            inet_aton(g2i[g2num].IPaddr, &(addr.sin_addr));
            memset(&(addr.sin_zero), '\0', 8);
            size_t s = sizeof(addr);

            if((connect(sockfd2, (struct sockaddr *)&addr, s)) ==0)
            {printf("sending %d\n", ntohs(addr.sin_port));
                char buf[1000];
                strcpy(buf, inet_ntoa(their_addr.sin_addr));
                printf("sending %s\n", buf);
                send(sockfd2, buf, strlen(buf), 0);
                sleep(1);
                int p = (their_addr.sin_port);
                send(sockfd2, &p, sizeof(int), 0);
                //close(sockfd2);
            }
        }
    }
}

