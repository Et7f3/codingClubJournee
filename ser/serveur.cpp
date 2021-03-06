//afficher les message reçus
#include <iostream>

//socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
//ecouter et recevoir en même temps
#include <thread>
#include <vector>

#define PORT 5001


using namespace std;

vector<int> clients(0,0);//les autres utilisateurs

int main(){
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == INVALID_SOCKET)
    {
      perror("socket()");
      exit(errno);
    }

  SOCKADDR_IN sin = { 0 };

  sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

  sin.sin_family = AF_INET;

  sin.sin_port = htons(PORT);

  if(bind (sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
      perror("bind()");
      exit(errno);
    }

  if(listen(sock, 5000) == -1)
    {
      perror("listen()");
      exit(errno);
    }
  while(1){
    char buffer[1024];
    int n = 0;

    if((n = recv(sock, buffer, sizeof buffer - 1, 0)) < 0)
      {
	perror("recv()");
	exit(errno);
      }

    buffer[n] = '\0'
      cout<<buffer;
      }
  

  cout<<"fin"<<endl;
}


