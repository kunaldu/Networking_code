//GobackN sender
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct pkt
{
  int no;
  char data[20];
}packet;
int main(int argc,char* argv[])
{
  struct sockaddr_in server,client;
  int sock1,sock2,k,len,l;
  char buf[100];
  sock1 = socket(AF_INET,SOCK_STREAM,0);
  if(argc!=2)
    {
      printf("Enter port number as command line argument\n");
      exit(0);
    }
  server.sin_port = atoi(argv[1]);
  server.sin_addr.s_addr = INADDR_ANY;
  k = bind(sock1,(struct sockaddr*)&server,sizeof(server));
  if(k == -1)
    printf("Binding error\n");
  listen(sock1,5);
  len = sizeof(client);
  sock2 = accept(sock1,(struct sockaddr*)&client,&len);
  if(sock2 == -1)
    printf("Accept error\n");
  {
    int wsize,wbegin = 0,time=0,ackno,i=0,n;
    packet p;
    recv(sock2,&n,sizeof(int),0);//No. of packet nos is received
    wsize = n-1;//window size for GBN can be maximum of n-1
    while(1)//can be a finite loop for the maximum no. of packets to be send
      {	
	while(1)// to accept a permissible packet from user
	  {
	    if(recv(sock2,&ackno,sizeof(int),MSG_DONTWAIT)>0)//Non blocking recv for acknowledgement
	      {
		if(isinsidewindow(wbegin,wsize,ackno))
		  {
		    printf("Acknowledgement for packet no %d received\n",ackno);
		    wbegin = ackno;
		    time=0;  
		  }
	      }
	    time++;    
	    if(time>2*wsize)//A randomn timeout
	      {
		printf("Timedout \nEnter the packets from no %d\n",wbegin);
		time=0;
	      }
	    printf("Enter the next packet no and data: ");
	    scanf("%d%s",&p.no,p.data);
	    if(p.no>wsize)
	      printf("Invalid packet no!!!\n"); 
	    else if (!isinsidewindow(wbegin,wsize,p.no))
	      {
		printf("Packet no. outside window limit\nCannot send this packet at the moment\n");
	      }
	    else
	      break;
	  }
	send(sock2,&p,sizeof(p),0);
      }
  }
  
}
int isinsidewindow(int wbegin,int wsize,int no)
{
  int i;
  for(i=0;i<wsize;i++)
      if((i+wbegin)%(wsize+1) == no)
	return 1;
  return 0;
}
