

#include "Server.hpp"

int main ()
{
    try
    {
        Server s;

        s.StartServer();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}