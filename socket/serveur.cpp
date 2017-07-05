#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <mutex>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PORT 5001

using namespace std;
vector<int> clients(0);
int sock=INVALID_SOCKET;
std::mutex mtx;           // mutex for critical section
int server();
//clear&&g++ -pthread serveur.cpp -o serveur&&valgrind --leak-check=full ./serveur
int main(){
  try{
    thread s(server);
    string command;
  
    while(1){
      cin>>command;
      if(command=="quit"){
	for(int i=0;i<clients.size();){
	  cout<<"bye :"<<clients.back()<<endl;
	  close(clients.back());
	  clients.pop_back();
	}
	close(sock);
	s.detach();
	break;
      }
      if(command=="send")
	{
	  string data,message;
	  while(data!="\x1B")
	    {
	      cin>>data;
	      message+=data;
	    }
	  cout<<"ok on envoie"<<endl;
	  for(int i=0;i<clients.size();i++){
	    /* if(send(clients.at(i), message.c_str(), message.size(), 0) < 0)
	       {
	       perror("send()");
	       exit(errno);
	       }*/
	    /* Send message to the server */
	    // critical section (exclusive access to std::cout signaled by locking mtx):
	    mtx.lock();  //mutex
	    int n = write(clients.at(i),
			  message.c_str(), message.size());
	    mtx.unlock();
	    std::cout << n << std::endl;
			      if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			      }
	      cout<<endl<<endl<<"à"<<clients.at(i)<<":"<<message<<endl<<endl;
	  }
	}
      
    }
      //s.join();
  }catch(const std::exception& e){
      cerr<<e.what();
    }
    return 0;
}
  int server(){
    struct protoent *pe = NULL;

    pe = getprotobyname("TCP");
    if (!pe)
      return perror("proto()"), 1;
    sock = socket(AF_INET, SOCK_STREAM, reinterpret_cast<int>(pe->p_proto));
    if(sock == INVALID_SOCKET)
      {
	perror("socket()");
	exit(errno);
      }

    sockaddr_in sin = { 0 };

    sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

    sin.sin_family = AF_INET;

    sin.sin_port = htons(PORT);

    if(bind (sock, (sockaddr *) &sin, sizeof sin) == SOCKET_ERROR)
      {
	perror("bind()");
	exit(errno);
      }

    while(1){
      if(listen(sock, 5) == SOCKET_ERROR)
	{
	  perror("listen()");
	  exit(errno);
	}

      sockaddr_in csin = { 0 };
      int csock;

      socklen_t sinsize = sizeof csin;

      csock = accept(sock, (sockaddr *)&csin, &sinsize);
   
      if(csock == INVALID_SOCKET)
	{
	  perror("accept()");
	  exit(errno);
	}
      cout<<"nouveau client id:"<<csock<<endl;
      // critical section (exclusive access to std::cout signaled by locking mtx):
      mtx.lock(); //mutex
      clients.push_back(csock);
      mtx.unlock();  
      dprintf(clients.back(), "this is a test\n");

    }
    return 0;
  }
