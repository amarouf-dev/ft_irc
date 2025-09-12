NAME = ircserv
CNAME = claint
RM = rm -rf
CPP = c++
CPPFlags = -Wextra -Wall -Werror -std=c++98

SRC = Claint.cpp  main.cpp  Server.cpp \

CSRC = RefClaint.cpp \

HDR = Claint.hpp  Server.hpp \

OSRC = $(SRC:.cpp=.o)
COSRC = $(CSRC:.cpp=.o)

all:$(NAME) $(CNAME)

$(NAME): $(OSRC)
	$(CPP) $(OSRC) -o $(NAME)

$(CNAME): $(COSRC)
	$(CPP) $(COSRC) -o $(CNAME)

%.o:%.cpp
	$(CPP) $(CPPFlags) -c $< -o $@

clean:
	$(RM) $(OSRC) $(COSRC)

fclean:
	$(RM) $(OSRC) $(COSRC)  $(NAME) $(CNAME)

re: fclean all

.SECENDARY:

.PHONY: all clean fclean re

