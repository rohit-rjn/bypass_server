#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/poll.h>
#include<pthread.h>

#define PORT 3854
#define BACKLOG 10

int sfd;
char port[20];
void read_func()
{
	while(1)
	{
		char buf[1000];
		int len=recv(sfd,buf,1000,0);
		buf[len]='\0';
		printf("You receive message\n%s\n",buf);	
	}
}
void write_func()
{
	while(1)
	{
		char buf[1000];
		printf("Type Message here\n");
		gets(buf);
		send(sfd,buf,strlen(buf),0);
	}
}

void runner(void *arg)
{	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int sfd2,newsfd2;
	socklen_t sin_size2;
	struct sockaddr_in my_address2,client_address2;   
	
	if((sfd2=socket(PF_INET,SOCK_STREAM,0))>0)
	{
		my_address2.sin_family=AF_INET;
		my_address2.sin_port=htons(atoi(port));
		my_address2.sin_addr.s_addr=INADDR_ANY;
		memset(&(my_address2.sin_zero),'\0',8);
		if(bind(sfd2,(struct sockaddr *)&my_address2,sizeof(struct sockaddr))!=-1)
		{
			if(listen(sfd2,BACKLOG)!=-1)
			{
					sin_size2=sizeof(struct sockaddr);
					while(1)
					{
						if((newsfd2=accept(sfd2,(struct sockaddr *)&client_address2,&sin_size2))>=0)
						{
							char name[20];
							printf("request accepted by group1\n");
	
							int l=recv(newsfd2,name,20,0);	
							name[l]='\0';
							printf("Hello %s\n",name);
						}	
					}
			}else perror("listen");	
		}else perror("bind");
	}else perror("socket");	
}

int main()
{
pthread_t read,write;
pthread_t runner_thread;
socklen_t sin_size;
struct sockaddr_in my_address,their_address;

	if((sfd=socket(PF_INET,SOCK_STREAM,0))>0)
	{
		their_address.sin_port=htons(PORT);
		their_address.sin_family=AF_INET;
		their_address.sin_addr.s_addr=inet_addr("127.0.0.1");
		memset(&(my_address.sin_zero),'\0',8);
	
		sin_size=sizeof(struct sockaddr);
		if(connect(sfd,(struct sockaddr *)&their_address,sin_size)>=0)
		{
			char name[20];
			printf("Enter Name here\n");
			gets(name);
			send(sfd,name,strlen(name),0);

			char number[20];
			printf("Enter Number here\n");
			gets(name);
			send(sfd,name,strlen(name),0);
			
			printf("Enter Port here\n");
			gets(port);
			send(sfd,port,strlen(port),0);

			pthread_create(&runner_thread,NULL,(void *)&runner,NULL);		
			while(1);
		}
	}else perror("socket");
	
	return 0;
}


