
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080

void sendHtmlPage(int clientSocket){

  std::string htmlPage = "<html><body><h1>Spectral Malmar</h1></body></html>";
  std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +std::to_string(htmlPage.length()) + "\r\n\r\n" + htmlPage;
  send(clientSocket, response.c_str(), response.length(), 0);
}

void handleRequest(int clientSocket){

  std::string data = "Spectral Analysis";
  std::string webpage ="Spectral analysis. <html><head><title>Spectral Webpage</title></head><body><h1>Spectral Project</h1><p>Basic tool for spectral analysis</p></body></html>";
  std::string response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/html; charset=utf-8\r\n";
  response += "Connection: close";
  response += "Content-Length: " + std::to_string(webpage.length()) + "\r\n";
  response += "\r\n";
  response += webpage;


  std::cout << "writing to client : " << std::endl << response << std::endl;
  write(clientSocket, response.c_str(), strlen(response.c_str()));  

  close(clientSocket);
}

int main(){

  
  int serverSocket, clientSocket;

  struct sockaddr_in serverAddress, clientAddress;

  socklen_t clientAddressLength = sizeof(clientAddress);

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if(serverSocket == -1){

    perror("Failed to create socket");

    return 1;
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);

  if(bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == 1){

    perror("Failed to bind socket");
    return 1;
  }

  if(listen(serverSocket, 5) == -1){
    
    perror("Failed to listen for connections");
    
    return 1;
  } 
  
  std::cout << "Server running on http://localhost;" << PORT << std::endl;

  while(true){

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if(clientSocket == -1){
  
      perror("Failed to accept connection");
      continue;
    }

    sendHtmlPage(clientSocket);
  }

  close(serverSocket);
  
  return 0;
}
