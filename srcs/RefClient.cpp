

#include "../headers/Server.hpp"

int main ()
{
    try
    {
        int clientsock = socket(AF_INET, SOCK_STREAM, 0);
        if (clientsock == -1)
            throw (std::runtime_error("Client's socket failed !"));
        
        sockaddr_in caddr;
        
        caddr.sin_port = htons(4040);
        caddr.sin_family = AF_INET;
        caddr.sin_addr.s_addr =  inet_addr("127.0.0.1");;
    
        socklen_t len = sizeof(caddr);

        if (connect(clientsock, reinterpret_cast<sockaddr *>(&caddr), len) == -1)
            throw (std::runtime_error("Client's connect failed !"));

        char buffer[BUFSIZE];

        while (true)
        {
            std::memset(buffer, 0, sizeof(buffer));
            if (read(0, buffer, BUFSIZE) < 0)
                throw (std::runtime_error("Client's read failed !"));

            write(clientsock, buffer, BUFSIZE - 1);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
