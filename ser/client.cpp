//afficher les message reçus
#include <iostream>

//socket
#include <netdb.h>
#include <netinet/in.h>
//ecouter et recevoir en même temps
#include <thread>
#include <vector>

#include <unistd.h>

using namespace std;

vector<int> clients(0,0);//les autres utilisateurs

int recevoir(){
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
  
  cout<<"J'arrette de recevoir"<<endl;
}

int envoyer(string msg){
  cout<<"Je doit envoyer "<<msg<<endl;
}


int main(){
  thread rec(recevoir);
  rec.join();
  for (int i=0;i<clients.size();i++)
    std::cout << clients.at(i) << ' ';
  string message="";
  while(message!=string("\x1B")){
    cin>>message;
    if(message=="\x1B"){
      cout<<"je me déconnecte"<<endl;
      envoyer("bye");
      exit(EXIT_SUCCESS);
    }else{
      int r=envoyer(message);
    }
  }
  return 0;
}
