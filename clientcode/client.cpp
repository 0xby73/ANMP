#include "netutils.h"
#include "joinmsg.h"
#include "ui.h"
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

std::vector<std::string> messages;
std::string username;

static void sendmsg(int clientsocket, WINDOW* input_win, WINDOW* chat_win) noexcept
{
  //std::vector<std::string> messages;

 while(run)
  {
    wrefresh(input_win);
    box(input_win);
    mvwprintw(input_win, 1, 1, "%s- ", username.c_str());
    wrefresh(input_win);

  
    std::string msg = getinp(input_win);

    /*
    box(input_win);
    wrefresh(input_win);

    std::string msg;

    std::cout << username << "- ";    
    std::getline(std::cin, msg);
    */
    if (msg.empty())
    {
      continue;
    }

    // I'm going to make the help command in its own header file in the future since it could get long
    if (msg == "!help")
    {
      messages.emplace_back("\n---------- HELP MENU ----------");
      messages.emplace_back("!help - Displays the help menu");
      messages.emplace_back("!quit - gracefully disconnects the client");
      update(chat_win, messages, std::max(0, static_cast<int>(messages.size()) - (LINES - height - 1)));
      continue;
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
   
    messages.emplace_back(msg);
    update(chat_win, messages, std::max(0, static_cast<int>(messages.size()) - (LINES - height - 1)));
    
    werase(input_win);
    box(input_win);
    mvwprintw(input_win, 1, 1, "%s- ", username.c_str());
    wrefresh(input_win);
  }
  return;
}

static void recvmsg(int clientsocket, WINDOW* chat_win) noexcept
{
  char buffer[1024] = {0};
  //std::vector<std::string> messages;

  while (run)
  {
      memset(buffer, 0, sizeof(buffer));
      const int recvbytes = recv(clientsocket, buffer, sizeof(buffer) - 1, 0);
      if (recvbytes > 0)
      {
        buffer[recvbytes] = '\0';
        messages.emplace_back(buffer);
        update(chat_win, messages, std::max(0, static_cast<int>(messages.size()) - (LINES - height - 1)));
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

  // --ncurses--

  startui();

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  WINDOW* chat_win = newwin(max_y - height - 1, max_x, 0, 0);
  WINDOW* input_win = newwin(height, max_x, max_y - height, 0);

  scrollok(chat_win, TRUE);
  box(input_win);
  
  // --ncurses--

  std::thread recvthread(static_cast<void(*)(int, WINDOW*)>(recvmsg), clientsocket, chat_win);
  //std::thread sendthread(static_cast<(int, WINDOW*)(int, WINDOW*)>(sendmsg), clientsocket, input_win, chat_win);
  std::thread sendthread(static_cast<void(*)(int, WINDOW*, WINDOW*)>(sendmsg), clientsocket, input_win, chat_win);

  recvthread.join();
  sendthread.join();
}
