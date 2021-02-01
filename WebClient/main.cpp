#include <iostream>
#include <functional>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libc.h>
#include "SingleThreadPool.hpp"
#include "GlobalVar.hpp"
#include "InputProcessing.hpp"


int main(int argc, char *argv[])
{
  int client;
  try
  {
    if (argc != 3)
    {
      std::cerr << "Invalid Port or Ip" << std::endl;
      return -1;
    }
    const int port = atoi(argv[1]);
    const char *serverIp = argv[2];

    client = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ServerAddress{};
    if (client < 0)
    {
      std::cerr << "Fatal error, socket incorrect" << std::endl;
      return -1;
    }

    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = htons(INADDR_ANY);
    inet_pton(AF_INET, serverIp, &ServerAddress.sin_addr);
    ServerAddress.sin_port = htons(port);

    if (connect(client, reinterpret_cast<const struct sockaddr *> (&ServerAddress), sizeof(ServerAddress)) == 0)
    {
      std::cout << "Connection to server: " << inet_ntoa(ServerAddress.sin_addr) << "\n" << "With port: " << port
                << '\n';
    } else
    {
      std::cerr << "Fatal error: server connect!" << std::endl;
      close(client);
      return -1;
    }

    char buffer[clientVar::BUFFER_SIZE];
    if (recv(client, buffer, clientVar::BUFFER_SIZE, 0) < 0)
    {
      std::cerr << "Fatal error, recv fail" << std::endl;
      close(client);
      return -1;
    }

    while (client > 0)
    {
      std::cout << "Enter message: ";
      std::cin.getline(buffer, clientVar::BUFFER_SIZE);
      std::string str(buffer);
      std::shared_ptr<SingleThreadPool::future_t<std::string>> string = clientVar::FirstThreadPool.execute<std::string>(
              InputProcessing::firstStageProcessingInput, str);
      std::shared_ptr<SingleThreadPool::future_t<long>> future = clientVar::SecondThreadPool.execute<long>(
              InputProcessing::secondStageProcessingInput, string);
      while (!future->isReady)
      {
        //waiting
      }
      auto exceptionPtr = getPtr();
      if (exceptionPtr)
      {
        std::rethrow_exception(exceptionPtr);
      }
      std::cout << "Data transfer to server:" << std::to_string(future->data).c_str() << '\n';
      if (send(client, std::to_string(future->data).c_str(), clientVar::BUFFER_SIZE, 0) < 0)
      {
        std::cerr << "Fatal error, send fail" << std::endl;
        return -1;
      }
    }
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
    close(client);
    return -1;
  }
  return 0;
}