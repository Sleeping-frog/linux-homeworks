#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>


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
  server_address.sin_addr.s_addr = inet_addr("146.190.62.39");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(80);

  // connect
  int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  if(connected == -1){
    perror("connection failed");
    exit(errno);
  }

  // send message to server
  std::string mess = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
  int sent = send(client_socket, mess.c_str(), mess.size(), 0);
  if(sent == -1){
    exit(errno);
  }

  int fd = open("./httpforever.html", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    perror("open failed");
    exit(errno);
  }

  char buffer[1001];
  while (true) {
    int rs = recv(client_socket, buffer, 1000, 0);
    if (rs == -1) {
      perror("server socket connection error");
      exit(errno);
    }

    if (rs > 0) {
      buffer[rs] = '\0';
      if (write(fd, buffer, 1000) < 0) {
        perror("write error");
        exit(errno);
      }
    }
    else if (rs == 0)
      break;
  }
  close(fd);
  close(client_socket);
  return 0;
}