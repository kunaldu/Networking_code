#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

typedef struct packet
{
	char data[1024];
}Packet;

typedef struct frame
{
	int frame_kind;
	int sq_no;
	int ack;
	Packet packet;
}Frame;

void  main(int argc, char const *argv[])
{
	if (argc !=4)
	{
		printf("Usage: %s <port>\n", argv[0] );
		exit(0);
	}
	int port= atoi(argv[2]);
	int sockfd;
	struct sockaddr_in serverAddr, newAddr;
	char buffer[1024];
	socklen_t addr_size;

	int frame_id=0;
	Frame frame_send;
	Frame frame_recv;
	int ack_recv=1;


	sockfd= socket(AF_INET, SOCK_DGRAM, 0);
	memset(&serverAddr,'\0', sizeof(serverAddr));
	serverAddr.sin_family= AF_INET;
	serverAddr.sin_port= htons(port);
	serverAddr.sin_addr.s_addr= inet_addr(argv[1]);
	char const* const fileName = argv[3]; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r"); /* should check the result */
    //char line[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%s", buffer);

		if (ack_recv=1)
		{
			frame_send.sq_no= frame_id;
			frame_send.frame_kind=1;
			frame_send.ack=0;

			//printf("Enter Data: ");
			//scanf("%s", buffer);
			strcpy(frame_send.packet.data,buffer);
			sendto(sockfd, &frame_send, sizeof(Frame),0, (struct sockaddr*)&serverAddr,sizeof(serverAddr));
			printf("Frame Sent.\n");
		}
		int addr_size=sizeof(serverAddr);
		int f_recv_size= recvfrom(sockfd, &frame_recv, sizeof(Frame),0,(struct sockaddr*)&serverAddr,&addr_size);
		if (f_recv_size>0 && frame_recv.sq_no==0 && frame_recv.ack == frame_id+1)
		{
			printf("ACK Recieved.\n");
			ack_recv=1;
		}else{
			printf("ACK Not Recieved.\n");
			ack_recv=0;
		}
		frame_id++; 
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);
	
	
	close(sockfd);
}