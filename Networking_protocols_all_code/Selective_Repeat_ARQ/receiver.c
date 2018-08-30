//selective repeat receiver
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
  struct sockaddr_in sndadd,recvadd;
  int sock1,sock2,k,len,l;
  char buf[100];
  sock1 = socket(AF_INET,SOCK_STREAM,0);
  if(argc!=2)
    {
      printf("Enter server port number as command line argument\n");
      exit(0);
    }
  recvadd.sin_port = atoi(argv[1]);
  recvadd.sin_addr.s_addr = 0;
  bind(sock1,(struct sockaddr*)&recvadd,sizeof(recvadd));
  listen(sock1,1);
  len = sizeof(sndadd);
  printf("Waiting for sender..\n\n");
  sock2 = accept(sock1,(struct sockaddr*)&sndadd,&len);
  {
    int n = 8,i=0,wsize,wbegin = 0,rec[10],j=10,pno,flag = 1;
    wsize = n/2;
    packet p;

    for(i=0; i<n; i++)
      rec[i] = 0;//Flag array for marking the received packets
    while(1)
      {
	recv(sock2,&p,sizeof(p),0);
	//sleep(3);
	if(isinsidewindow(wbegin,wsize,p.no) && (p.no!=2 || !flag ))//Packet no 2 is considered lost for the first time
	  {
	    j--;
	    printf("Packet no. %d with data \"%s\" received\n",p.no,p.data);
	    pno = p.no;
	    rec[p.no] = 1;
	    if(p.no == wbegin)
	      while(rec[wbegin])
		{
		  rec[wbegin] = 0;
		  wbegin = (wbegin+1)%n;
		}
	  }
	  else
	    {
	      printf("Packet no. %d with data \"%s\" rejected\n",p.no,p.data);
	      if(p.no == 2)
		flag = 0;
	    }
	printf("\tAcknowledgement send for packet no %d\n",pno);
	send(sock2,&pno,sizeof(int),0);
      }
  }
}
int isinsidewindow(int wbegin,int wsize,int no)
{
  int i;
  for(i=0;i<wsize;i++)
    {
      if((wbegin+i)%(2*wsize) == no)
	return 1;
    }
  return 0;
}







  
