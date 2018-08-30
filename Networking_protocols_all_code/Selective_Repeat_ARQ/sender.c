//selective repeat sender

#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct pkt
{
  int no;
  char data[20];
}packet;
int main(int argc,char* argv[])
{
  struct sockaddr_in recvadd;
  int sock1,k,len,l;
  char buf[100];
  sock1 = socket(AF_INET,SOCK_STREAM,0);
  if(argc!=2)
    {
      printf("Enter port number as command line argument\n");
      exit(0);
    }
  recvadd.sin_family = AF_INET;
  recvadd.sin_port = atoi(argv[1]);
  recvadd.sin_addr.s_addr = INADDR_ANY;
  connect(sock1,(struct sockaddr*)&recvadd,sizeof(recvadd));
  {
    int wsize,wbegin = 0,time=0,ackno,i=0,n = 6,j=0,ack[10],flag = 1;
    packet p;
    for(i=0; i<n; i++)
      ack[i] = 0;//Flag array for acknowledgements received
    wsize = n/2;//window size for SR can be maximum of n/2
    while(1)
      {	
	if(recv(sock1,&ackno,sizeof(int),MSG_DONTWAIT)>0)//Non blocking recv for acknowledgement
	  {
	    printf("\tAcknowledgement for packet no %d received\n",ackno);
	    if(isinsidewindow(wbegin,wsize,ackno))
	      ack[ackno] = 1;
	    if(ackno == wbegin)
	      {
		while(ack[wbegin])
		  {
		    ack[wbegin] = 0;
		    wbegin = (wbegin+1)%n;
		  }
		flag = 1;
		time = 0;
	      }
	  }
	time++;  
	  
	if(time > 1.5*wsize)//A timeout choosen
	  {
	    printf("Timedout \nResending the timed out packet %d\n",wbegin);
	    p.no = wbegin;
	    time = 0;
	  }
	else 
	  {
	    p.no = (j++)%n;
	  }
	strcpy(p.data,"data");
	if (!isinsidewindow(wbegin,wsize,p.no))
	  {
	    if(flag)
	      {
		printf("Packet no. %d outside window limit\nCannot send this packet at the moment\n",p.no);
		flag = 0;
	      }
	    j--;
	  }
	else 
	  {
	    printf("Sending packet no %d\n",p.no);
	    send(sock1,&p,sizeof(p),0);
	  }
      }
  }
  
}
int isinsidewindow(int wbegin, int wsize, int no)
{
  int i;
  for(i = 0;i< wsize;i++)
    {
      if((wbegin+i)%(2*wsize) == no)
	return 1;
    }
  return 0;
}
