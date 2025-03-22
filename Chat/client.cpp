#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <pthread.h>

struct arg {
  int socket;
};

void* handler(void* args) {
  int socket = ((arg*)args)->socket;
  delete (arg*)args;

  char buffer[1001];
  while (true) {
    int rs = recv(socket, buffer, 1000, 0);
    if (rs == -1) {
      perror("server socket connection error");
      exit(errno);
    }

    if (rs > 0) {
      buffer[rs] = '\0';
      std::cout << buffer << std::endl;
    }
  }
  return nullptr;
}

int main(){

  // server address 
  struct sockaddr_in server_address;

  // create a socket
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
        perror("socket creation error");
        exit(errno);
  }

  // fill the server ip and port address 
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);

  // connect
  int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  if(connected == -1){
    perror("connection failed");
    exit(errno);
  }
  
  arg* ptr = new arg();
  ptr->socket = client_socket;
  pthread_t tid = 0;
  pthread_create(&tid, 0, handler, (void*)ptr);

  // send message to server
  std::string mess;
  std::cout << "Enter your name:" << std::endl;
  std::getline(std::cin, mess);
  int sent = send(client_socket, mess.c_str(), mess.size(), 0);
  if(sent == -1){
    exit(errno);
  }
  do {
    std::getline(std::cin, mess);
    int sent = send(client_socket, mess.c_str(), mess.size(), 0);
    if(sent == -1){
      exit(errno);
    }
  } while (mess != "/exit");

  pthread_cancel(tid);
  close(client_socket);
  return 0;
}
