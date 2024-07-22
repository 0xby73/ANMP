#include "netutils.h"
#include "joinmsg.h"
#include "colors.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <cstring>
#include <limits>
#include <atomic>
#include <errno.h>

std::mutex mtx;
std::atomic<bool> run(true);

std::string username;

static void sendmsg(int clientsocket) noexcept
{

 while(run)
  {
    std::string msg;

    std::cout << username << "- ";    
    std::getline(std::cin, msg);
    
    if (msg.empty())
    {
      continue;
    }

    // I'm going to make the help command in its own header file in the future since it could get long
    if (msg == "!help")
    {
      std::cout << "\n---------- HELP MENU ---------- \n"; // I'm also probably going to turn this into ascii later on
      std::cout << BLUE << "!help - Displays the help menu\n" << RESET;
      std::cout << BLUE << "!quit - gracefully disconnects the client <optimal way to quit>\n\n" << RESET;
    }

    if (msg == "!quit")
    {
      msg = ORANGE + username + " has disconnected.";
      send(clientsocket, msg.c_str(), msg.length(), 0);
      close(clientsocket);
      run = false;
      break;
    } 
    
    msg = username + ": " + msg;

    send(clientsocket, msg.c_str(), msg.length(), 0);
    
  }
  return;
}

static void recvmsg(int clientsocket) noexcept
{
  char buffer[1024] = {0};

  while (run)
  {
      memset(buffer, 0, sizeof(buffer));
      const int recvbytes = recv(clientsocket, buffer, sizeof(buffer) - 1, 0);
      if (recvbytes > 0)
      {
        buffer[recvbytes] = '\0';
        std::cout << buffer << '\n';
      }
      else if (recvbytes == 0)
      {
        std::cout << "Server has shutdown.\n";
        run = false;
        break;
      }
      else
      {
        std::cerr << "Error in recv(). Error code: " << errno << ", Error message: " << strerror(errno) << std::endl;
        run = false;
        break;
      }
    }
  return;
}


static void pickusername() noexcept
{
  std::cout << "Enter a username: ";
  std::cin >> username;

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(const int argc, char** argv)
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " IP " << "PORT\n" << RESET;
    return 1;
  }
  
  std::string ip = argv[1];
  int port = std::stoi(argv[2]);
 
  system("clear");

  header();

  pickusername();
  
  int clientsocket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
  {
    std::cerr << "Invalid IP Address\n";
    return 1;
  }

  
  if (connect(clientsocket, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
  {
    std::cerr << "Connection failed\n";
    return 1;
  }

  std::string joined = username + " has joined. ";
  send(clientsocket, joined.c_str(), joined.length(), 0);

  std::thread recvthread(static_cast<void(*)(int)>(recvmsg), clientsocket);
  std::thread sendthread(static_cast<void(*)(int)>(sendmsg), clientsocket);

  recvthread.join();
  sendthread.join();

}
