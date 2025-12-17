

#include "../headers/Server.hpp"

void joke(int bot, const std::string &channel) {
    std::string str[6] = {
        "Why do programmers prefer dark mode? Because light attracts bugs.",
        "A SQL query walks into a bar, walks up to two tables and asks: Can I join you?",
        "How many programmers does it take to change a light bulb? None, that’s a hardware problem.",
        "Why do Java developers wear glasses? Because they don’t C#.",
        "There are 10 types of people in the world: Those who understand binary and those who don’t.",
        "Debugging: Being the detective in a crime movie where you are also the murderer."
    };

    int r = std::rand() % 6;
    std::string msg = "PRIVMSG " + channel + " :" + str[r] + "\r\n";
    write(bot, msg.c_str(), msg.size());
}

int main() {
    try {
        int bot = socket(AF_INET, SOCK_STREAM, 0);
        if (bot == -1)
            throw std::runtime_error("Bot socket failed!");

        sockaddr_in caddr;
        caddr.sin_port = htons(4040);
        caddr.sin_family = AF_INET;
        caddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(bot, (sockaddr*)&caddr, sizeof(caddr)) == -1)
            throw std::runtime_error("Bot connect failed!");

        write(bot, "NICK jokebot\r\n", 14);
        write(bot, "USER jokebot 0 * :Joke Bot\r\n", 28);
        write(bot, "JOIN #school\r\n", 14);

        char buffer[1024];
        std::string channel = "#school";

        while (true) {
            std::memset(buffer, 0, sizeof(buffer));
            int n = read(bot, buffer, sizeof(buffer)-1);
            if (n <= 0) continue;

            std::string buff(buffer);
            std::cout << buff;

            if (buff.find("!JOKE") != std::string::npos) {
                joke(bot, channel);
            }
        }
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}