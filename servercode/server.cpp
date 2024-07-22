#include "netutils.h"
#include "colors.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

std::vector<int> clients;
std::mutex client_mutex;

void broadcast(const std::string& msg, int clientsocket) noexcept {
    std::lock_guard<std::mutex> lock(client_mutex);
    for (int client : clients) {
        if (client != clientsocket) {
            if (send(client, msg.c_str(), msg.size(), 0) == -1) {
                std::cerr << "Error sending to client " << client << ". Error: " << strerror(errno) << std::endl;
            }
        }
    }
}

static void rclient(int clientsocket) noexcept 
{
  std::lock_guard<std::mutex> lock(client_mutex);
  clients.erase(std::remove(clients.begin(), clients.end(), clientsocket), clients.end());
  std::cout << RED << "Client disconnected | clients: " << clients.size() << RESET << '\n';
  close(clientsocket);
}
static void chandle(int clientsocket) noexcept
{
  sockaddr_in clientaddr;
  socklen_t clientaddrlen = sizeof(clientaddr);
  getpeername(clientsocket, (sockaddr*)&clientaddr, &clientaddrlen);
 {
  std::lock_guard<std::mutex> lock(client_mutex);
  clients.push_back(clientsocket);
 }
  std::cout << GREEN << "New client connected | clients: " << clients.size() << '\n' << RED << inet_ntoa(clientaddr.sin_addr) << " | " << RESET;

  while (true)
  {
   char buffer[1024] = { 0 };
   int recvbytes = recv(clientsocket, buffer, sizeof(buffer), 0);
   if(recvbytes < 0)
   {
    std::cerr << "Receive failed: " << strerror(errno) << '\n';
    break;
   }
   else if (recvbytes == 0)
   {
	   //std::thread RC(rclient);
     break;
   }
   else
   {
    std::string message(buffer, recvbytes);
    std::cout << message << '\n';
    broadcast(message, clientsocket);
   }
   memset(buffer, 0, sizeof(buffer));
  }
    rclient(clientsocket);


}

int main(const int argc, char** argv) noexcept
{
 if (argc != 2)
 {
  std::cerr << RED << "Usage: " << argv[0] << " PORT\n" << RESET;
  return 1;
 }

 int port = std::stoi(argv[1]);
 std::cout << "listening on " << port << '\n';
 port = htons(port);

  int serversocket = socket(AF_INET, SOCK_STREAM, 0);
  int clientsocket = accept(serversocket, nullptr, nullptr);

 struct sockaddr_in addr;
 memset(&addr, 0, sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_port = port;
 addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt = 1;
  if (setsockopt(serversocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
  {
    std::cerr << "Error setting SO_REUSEADDR: " << strerror(errno) << std::endl;
    close(serversocket);
    return 1;
  }

 if (bind(serversocket, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
 {
  std::cerr << "Bind failed\n";
  return 1;
 }

 if (listen(serversocket, 5) < 0)
 {
  std::cerr << "Listen failed\n";
  return 1;
 }
 for (;;)
 {
  int clientsocket = accept(serversocket, NULL, NULL);

  std::thread HC(chandle, clientsocket);
  HC.detach();
 }
 close(serversocket);

 return 0;
}
