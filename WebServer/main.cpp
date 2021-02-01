#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <unistd.h>
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>

const int BUFFER_SIZE = 256;


std::pair<int, bool> countAndCorrectSymbolsClientData(std::string &&incomeData);

int main(int argc, char *argv[])
{
  int masterSocket, socketAccept;
  try
  {
    if (argc != 2)
    {
      std::cerr << "Invalid Port" << std::endl;
      return -1;
    }

    const int port = atoi(argv[1]);

    if (port <= 0)
    {
      std::cerr << "Invalid Port" << std::endl;
      return -1;
    }

    masterSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (masterSocket == 0)
    {
      std::cerr << "Fatal error, socket incorrect" << std::endl;
      return -1;
    }

    struct sockaddr_in ServerAddress{};

    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = htons(INADDR_ANY);
    ServerAddress.sin_port = htons(port);

    if (bind(masterSocket, reinterpret_cast<struct sockaddr *>(&ServerAddress), sizeof(ServerAddress)) < 0)
    {
      std::cerr << "Fatal error, bind fail" << std::endl;
      close(masterSocket);
      return -1;
    }

    std::cout << "Listen on port: " << port << '\n';

    if (listen(masterSocket, 1) < 0)
    {
      std::cerr << "Fatal error, listen fail" << std::endl;
      close(masterSocket);
      return -1;
    }

    socklen_t addrLen = sizeof(ServerAddress);

    std::cout << "Waiting for connections ..." << '\n';

    char buffer[BUFFER_SIZE];

    socketAccept = accept(masterSocket, reinterpret_cast<struct sockaddr *>(&ServerAddress), &addrLen);

    if (socketAccept < 0)
    {
      std::cerr << "Fatal error, accept fail" << std::endl;
      close(masterSocket);
      return -1;
    }

    strcpy(buffer, "Server connect success!");
    send(socketAccept, buffer, BUFFER_SIZE, 0);
    std::cout << "=> Connected to client" << std::endl;

    while (socketAccept > 0)
    {
      strcpy(buffer, "\0");
      int ret = recv(socketAccept, buffer, BUFFER_SIZE, 0);
      if (ret == 0) //client disconnect and connect again
      {
        socketAccept = accept(masterSocket, reinterpret_cast<struct sockaddr *>(&ServerAddress), &addrLen);
        strcpy(buffer, "Server connect success!");
        send(socketAccept, buffer, BUFFER_SIZE, 0);
        continue;
      }
      if (ret < 0)
      {
        std::cerr << "Fatal error, recv fail" << std::endl;
        close(socketAccept);
        close(masterSocket);
        return -1;
      }
      if (strcmp(buffer, "\0") != 0)
      {
        auto pair = countAndCorrectSymbolsClientData(std::string(buffer));
        std::cout << "==============\n";
        std::cout << "Data: " << buffer << '\n';
        std::cout << "Sum of elements: " << pair.first << '\n';
        pair.second ? std::cout << "Data is correct!\n" : std::cout << "Data is incorrect!\n";
        std::cout << "==============\n";
      }
    }
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
    close(socketAccept);
    close(masterSocket);
    return -1;
  }
  return 0;
}

std::pair<int, bool> countAndCorrectSymbolsClientData(std::string &&incomeData)
{
  std::pair<int, bool> pair;
  pair.first = incomeData.size();
  int num = std::stoi(incomeData);
  if (num > 2 && num % 32 == 0)
  {
    pair.second = true;
  } else
  {
    pair.second = false;
  }
  return pair;
}
