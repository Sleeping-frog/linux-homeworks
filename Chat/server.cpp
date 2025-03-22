#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unordered_map>

struct arg {
  int client_socket;
};

int server_socket;
pthread_mutex_t mutex;

std::unordered_map<std::string, int> users;

void sendMess(std::string mess, std::string name) {
  std::cout << mess << std::endl;
  for (auto client : users) {
    if (client.first != name) {
      pthread_mutex_lock(&mutex);
      send(client.second, mess.c_str(), mess.size(), 0);
      pthread_mutex_unlock(&mutex);
    }
  }
}

void* handler(void* args) {
  int client_socket = ((arg*)args)->client_socket;
  delete (arg*)args;

  char buffer[1001];
  int rs = recv(client_socket, buffer, 1000, 0);
  if (rs == -1) {
    perror("client socket connection error");
    close(client_socket);
    close(server_socket);
    exit(errno);
  }
  buffer[rs] = '\0';
  std::string name(buffer); 
  users.insert({name, client_socket});
  sendMess(name + " connected!", "");
  while (true) {
    // Receive message from client
    int rs = recv(client_socket, buffer, 1000, 0);
    if (rs == -1) {
      perror("client socket connection error");
      close(client_socket);
      close(server_socket);
      exit(errno);
    }

    if (rs > 0) {
      buffer[rs] = '\0';
      if (std::string(buffer) == "/exit") {
        sendMess(name + " exited", name);
        break;
      }
      if (std::string(buffer) == "/list") {
        std::string list("Online users: ");
        for (auto user : users) {
          list.append(user.first + ", ");
        }
        list.pop_back();
        list.pop_back();
        sendMess(name + ": " + buffer + "\0", name);
        sendMess(list, "");
        continue;
      }
      std::string mess = name + ": " + buffer;
      sendMess(mess, name);
    }
  }

  close(client_socket);
  users.erase(name);
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
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
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
