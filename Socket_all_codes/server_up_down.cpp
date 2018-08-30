#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fstream>
#include <string>

using namespace std;

void fileWrite(string babaRamRahim){
  ofstream myfile;
  myfile.open ("server/data_server.txt");
  babaRamRahim = babaRamRahim.substr(0, babaRamRahim.size()-2);
  myfile << babaRamRahim;
  myfile.close();

}

string fileRead(){
     string STRING;
     string s;
    ifstream infile;
    infile.open ("data.txt");
        while(getline(infile,STRING)) 
        {
        
        s.append(STRING);
           
        }
    infile.close();
    s.append("*");
   
    
return s;
    
}

int main(){
	int opt;
	cout<<"Enter 1 for upload or  \nEnter 2 for download \n Default case: Download"<<endl;
	cin>>opt;
	if(opt==1){
		 int client, server;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 102400;
    char buffer[bufsize];
    int buff;

    struct sockaddr_in server_addr;
    socklen_t size;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }



    cout << "\n=> Socket server has been created..." << endl;



    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }



    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    listen(client, 1);


    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    while (server > 0) 
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", proceed..." << endl;
        
        cout << "\n=> Enter # to end the connection\n" << endl;



        
        

        string bufs;
        do {
            
            
                recv(server, buffer, bufsize, 0);
                bufs.append(buffer);
              //  cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            //} while (*buffer != '*');
            fileWrite(bufs);
          //  cout<<"caled"<<endl;
        } while (!isExit);


        cout << "\n\n=> Connection terminated . Jai Hind";
        close(server);
        cout << "\n." << endl;
        isExit = false;
        exit(1);
    }

    close(client);

}
else{
	 int client, server;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 102400;
    char buffer[bufsize];
    int buff;

    struct sockaddr_in server_addr;
    socklen_t size;

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }



    cout << "\n=> Socket server has been created..." << endl;



    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }



    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    listen(client, 1);

    string out;
    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    while (server > 0) 
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", proceed..." << endl;
        out = fileRead()   ;
        cout << "\n=> Enter # to end the connection\n" << endl;
        strcpy(buffer, out.c_str());


        
        

        
        do {
            
            
            send(server, buffer, bufsize, 0);
            out = "#";
            strcpy(buffer, out.c_str());
            send(server, buffer, bufsize, 0);
              //  cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            //} while (*buffer != '*');
            
          //  cout<<"caled"<<endl;
        } while (!isExit);


        cout << "\n\n=> Connection terminated . Jai Hind";
        close(server);
        cout << "\n." << endl;
        isExit = false;
        exit(1);
    }

    close(client);

}
}
