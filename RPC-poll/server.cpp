#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <poll.h>
#include "parallel-scheduler.h"

struct arg {
  pollfd* poll_fd;
};

int server_socket;
pthread_mutex_t mutex;

void handler(void* args) {
  pollfd* ptr = ((arg*)args)->poll_fd;
  int client_socket = ptr->fd;
  delete (arg*)args;

  char buffer[101];
    int rs = recv(client_socket, buffer, 101, 0);
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
      if (std::string(buffer) == "exit") {
        close(client_socket);
        ptr->fd = -1;
        return;
      }
      std::string command[3];
      int index = 0;
      for (int i = 0; buffer[i] != '\0'; ++i) {
        if (buffer[i] == ' ') {
            ++index;
            continue;
        }
        command[index].push_back(buffer[i]);
      }
      double num1 = std::stod(command[1]);
      double num2 = std::stod(command[2]);
      double answer;
      if (command[0] == "ADD") {
        answer = num1 + num2;
      }
      else if (command[0] == "SUB") {
        answer = num1 - num2;
      }
      else if (command[0] == "MUL") {
        answer = num1 * num2;
      }
      else if (command[0] == "DIV") {
        answer = num1 / num2;
      }
      std::string result = std::to_string(answer);
      send(client_socket, result.c_str(), result.size(), 0);
      ptr->events = POLLIN;
  }

}

struct sockaddr_in server_address;

int main() {
  pthread_mutex_init(&mutex, 0); 
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
      perror("socket creation error");
      exit(errno);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);

  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
      perror("bind failed");
      exit(errno);
  }

  if (listen(server_socket, 10) < 0) {
    perror("listen failed");
    exit(errno);
  }
  std::cout << "Waiting for connection\n";

  struct pollfd poll_fds[11];
  poll_fds[0] = {server_socket, POLLIN, 0};
  int clients_count = 1;
  ThreadPool pool(5);

  while(true) {
    int client_socket;
    struct sockaddr_in client_address;
    unsigned int client_addr_len = sizeof(client_address);

    poll(poll_fds, clients_count, -1);

    if (poll_fds[0].revents & POLLIN) {
      poll_fds[0].revents = 0;
      if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) {
        perror("accept failed");
        exit(errno);
      }
      std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << "\n";
      poll_fds[clients_count++] = {client_socket, POLLIN, 0};
      continue;
    }

    for (int i = 1; i < clients_count; ++i) {
      if (poll_fds[i].revents & POLLIN) {
        poll_fds[i].revents = 0;
        poll_fds[i].events = 0;
        arg* ptr = new arg();
        ptr->poll_fd = &poll_fds[i];
        pool.add_task(handler, ptr);
      }
    }
  }
  
  pthread_mutex_destroy(&mutex);
  close(server_socket);
  return 0;
}