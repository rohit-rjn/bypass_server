#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/poll.h>


#define PORT 1235
#define BACKLOG 10

struct information
{
int fd;
char name[20];
}info[100];
int n=0;
pthread_t runner_thread;

void runner(void *arg)
{
char *num=(char *)arg;
int newport=atoi(num);

printf("Port received %d Now connecting to group1 user\n",newport);	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
socklen_t sin_size2;
int sfd2;
struct sockaddr_in my_address2,their_address2;

	if((sfd2=socket(PF_INET,SOCK_STREAM,0))>0)
	{
		their_address2.sin_port=htons(newport);
		their_address2.sin_family=AF_INET;
		their_address2.sin_addr.s_addr=inet_addr("127.0.0.1");
		memset(&(my_address2.sin_zero),'\0',8);
	
		sin_size2=sizeof(struct sockaddr);
		if(connect(sfd2,(struct sockaddr *)&their_address2,sin_size2)>=0)
		{
			char name[20];
			printf("Enter Your Name here\n");
			gets(name);
			send(sfd2,name,strlen(name),0);
		}
	}else perror("socket");
}


int main()
{
	int sfd,newsfd,nfds=0;
	socklen_t sin_size;
	struct sockaddr_in my_address,client_address;   
	
	if((sfd=socket(PF_INET,SOCK_STREAM,0))>0)
	{
		my_address.sin_family=AF_INET;
		my_address.sin_port=htons(PORT);
		my_address.sin_addr.s_addr=INADDR_ANY;
		memset(&(my_address.sin_zero),'\0',8);
		if(bind(sfd,(struct sockaddr *)&my_address,sizeof(struct sockaddr))!=-1)
		{
			if(listen(sfd,BACKLOG)!=-1)
			{
					sin_size=sizeof(struct sockaddr);
					while(1)
					{
						if((newsfd=accept(sfd,(struct sockaddr *)&client_address,&sin_size))>=0)
						{
							char newport[20];
							printf("request accepted by group2 Please send the Port\n");
	
							int l=recv(newsfd,newport,20,0);	
							newport[l]='\0';
							printf("Received Port %s\n",newport);
							pthread_create(&runner_thread,NULL,(void *)&runner,(void *)newport);		
							
						}	
					}
			}else perror("listen");	
		}else perror("bind");
	}else perror("socket");	
	
	return 0;
}
