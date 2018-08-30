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


int fla;
using namespace std;

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


void fileWrite(string babaRamRahim){
  ofstream myfile;
  myfile.open ("clienti/data_server.txt");
  babaRamRahim = babaRamRahim.substr(0, babaRamRahim.size()-2);
  myfile << babaRamRahim;
  myfile.close();
    
}



int main(){
	int opt;
	cout<<"Enter 1 for upload or  \nEnter 2 for download \n Default case: Download"<<endl;
	cin>>opt;
	if(opt==1){

		    string out;
    int client;
    int portNum = 1500; 
    bool isExit = false;
    int bufsize = 1024000;
    char buffer[bufsize];
    char* ip = "192.168.1.73";
    int opt;

    struct sockaddr_in server_addr;

    fla =1;
    client = socket(AF_INET, SOCK_STREAM, 0);

    
    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }



    cout << "\n=> Socket client has been created..." << endl;
    


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);






    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;



    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, proceed...";

    cout << "\n\n=> Enter # to end the connection\n" << endl;
    out = fileRead()   ;

    strcpy(buffer, out.c_str());
   

    
    
    

    do {
        
        do {
                        
            send(client, buffer, bufsize, 0);
            out = "#";
            strcpy(buffer, out.c_str());
            send(client, buffer, bufsize, 0);
            
            if (*buffer == '#') {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);

      
        cout << "Check file 'data_server.txt' "<<endl;
        do {
            recv(client, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }

        } while (*buffer != 42);
        cout << endl;

    } while (!isExit);

   

    cout << "\n=> Connection terminated.\nJai Hind...\n";

    close(client);

	}


	else{
		  string out;
    int client;
    int portNum = 1500; 
    bool isExit = false;
    int bufsize = 1024000;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
    int opt;

    struct sockaddr_in server_addr;

    fla =1;
    client = socket(AF_INET, SOCK_STREAM, 0);

    
    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }



    cout << "\n=> Socket client has been created..." << endl;
    


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);






    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;



    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, proceed...";

    cout << "\n\n=> Enter # to end the connection\n" << endl;
    
    strcpy(buffer, out.c_str());
   

    
    
    

      
        cout << "Check file 'data_server.txt' "<<endl;

    string bufs;
        do {
            
            
                recv(client, buffer, bufsize, 0);
                bufs.append(buffer);
                isExit = true;
              //  cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            //} while (*buffer != '*');
            //cout<<bufs<<endl;
          //  cout<<"caled"<<endl;
        } while (!isExit);
fileWrite(bufs);

   

    cout << "\n=> Connection terminated.\nJai Hind...\n";

    close(client);

	}
}