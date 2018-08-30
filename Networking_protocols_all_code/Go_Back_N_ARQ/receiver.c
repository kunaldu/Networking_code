//GoBackN receiver
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct pack
  {
    int no;
    char data[20];
  }packet;
int main(int argc,char* argv[])
  {
    struct sockaddr_in server;
    int sock1,k,len,l;
    char buf[100];
    sock1 = socket(AF_INET,SOCK_STREAM,0);
    if(argc!=2)
      {
        printf("Enter server port number as command line argument\n");
        exit(0);
      }
    server.sin_family = AF_INET;
    server.sin_port = atoi(argv[1]);
    server.sin_addr.s_addr = INADDR_ANY;
    k  = connect(sock1,(struct sockaddr*)&server,sizeof(server));
    if(k == -1)
      printf("Connect error\n");
    {
      int n,nextpktno=0,i=0;
      packet p;
      printf("Enter the size of the sequence number space [If frames are numbered from 0 to N enter N+1]:\n");
      scanf("%d",&n);
      send(sock1,&n,sizeof(int),0);
      while(1)
        {
  	sleep(1);//To slow down the receiver
  	recv(sock1,&p,sizeof(p),0);
  	if(p.no == nextpktno)
  	  {
  	    printf("Packet no. %d with data \"%s\" received\n",p.no,p.data);
  	    if(nextpktno == n-1)
  	      nextpktno = 0;
  	    else
  	      nextpktno++;
  	  }
  	else
  	    printf("Packet no. %d with data \"%s\" rejected\n",p.no,p.data);
  	if(++i%(n-1) == 0)//Ack being send 
  	  {
  	    printf("Acknowledgement send for  packet no %d\n",nextpktno-1);
  	    send(sock1,&p.no,sizeof(int),0);
  	  }
        }
    }
  }