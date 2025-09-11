NAME = irc
RM = rm -rf
CPP = c++
CPPFlags = -Wextra -Wall -Werror -std=c++98

SRC = Claint.cpp  main.cpp  Server.cpp \

HDR = Claint.hpp  Server.hpp \

OSRC = $(SRC:.cpp=.o)

all:$(NAME)

$(NAME): $(OSRC)
	$(CPP) $(OSRC) -o $(NAME)

%.o:%.cpp
	$(CPP) $(CPPFlags) -c $< -o $@

clean:
	$(RM) $(OSRC)

fclean:
	$(RM) $(OSRC) $(NAME)

re: fclean all

.SECENDARY:

.PHONY: all clean fclean re

