#include <iostream>
#include <vector>
#include <thread>
#include <string>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PORT 5001


//clear&&g++ -pthread client.cpp -o client&&valgrind --leak-check=full ./client
int sock;
using namespace std;
int main(){
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
  struct hostent *hostinfo = NULL;
  sockaddr_in sin = { 0 }; /* initialise la structure avec des 0 */
  const char *hostname = "localhost";

  hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
  if (hostinfo == NULL) /* l'hôte n'existe pas */
    {
      fprintf (stderr, "Unknown host %s.\n", hostname);
      exit(EXIT_FAILURE);
    }

  sin.sin_addr = *(in_addr *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
  sin.sin_port = htons(PORT); /* on utilise htons pour le port */
  sin.sin_family = AF_INET;

  if(connect(sock,(sockaddr *) &sin, sizeof(sockaddr)) == SOCKET_ERROR)
    {
      perror("connect()");
      exit(errno);
    }
  while(1){
    char buffer[1024]={0};
    int n = 0;
    /*
    if((n = recv(sock, buffer, sizeof buffer - 1, 0)) < 0)
      {
	perror("recv()");
	exit(errno);
      }
    */
    // bzero(buffer,256);
   n = read(sock, buffer, sizeof buffer-1);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
    buffer[n] = '\0';
    cout<<buffer;
  }
  return 0;
}
