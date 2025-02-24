#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

struct arg {
  int client_socket;
};

int server_socket;
pthread_mutex_t mutex;

void* handler(void* args) {
  int client_socket = ((arg*)args)->client_socket;
  delete (arg*)args;

  char buffer[1001];
  do {
    // Receive message from client
    int rs = recv(client_socket, buffer, 1000, 0);
    if (rs == -1) {
      perror("client socket connection error");
      close(client_socket);
      close(server_socket);
      exit(errno);
    }

    if (rs > 0) {
      pthread_mutex_lock(&mutex);
      std::cout << "Got message:\n";
      buffer[rs] = '\0';
      std::cout << buffer << "\n";
      pthread_mutex_unlock(&mutex);
    }
  } while (std::string(buffer) != "exit");

  close(client_socket);
  return nullptr;

}

struct sockaddr_in server_address;

int main() {
  pthread_mutex_init(&mutex, 0); 
  // create a socket  
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
      perror("socket creation error");
      exit(errno);
  }

  // create an endpoint
  
  // internet protocol = AF_INET
  server_address.sin_family = AF_INET;
  // accept or any address (bind the socket to all available interfaces)
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  // port
  server_address.sin_port = htons(8888);

  // Bind server_socket to server_address
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
      perror("bind failed");
      exit(errno);
  }

  // Listen for incoming connections
  if (listen(server_socket, 10) < 0) {
    perror("listen failed");
    exit(errno);
  }
  std::cout << "Waiting for connection\n";

  while(true) {
    int client_socket;
    struct sockaddr_in client_address;
    unsigned int client_addr_len = sizeof(client_address);

    // Accept incoming connection
    if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
      perror("accept failed");
      exit(errno);
    }

    std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";

    arg* ptr = new arg();
    ptr->client_socket = client_socket;
    pthread_t tid = 0;
    pthread_create(&tid, 0, handler, (void*)ptr);
    pthread_detach(tid);
  }
  
  // close
  pthread_mutex_destroy(&mutex);
  close(server_socket);
  return 0;
}
