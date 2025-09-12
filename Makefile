NAME = ircserv
CNAME = claint
RM = rm -rf
CPP = c++
CPPFlags = -Wextra -Wall -Werror -std=c++98

HDIR = headers
SRCDIR = srcs

HDR = $(HDIR)/Claint.hpp  $(HDIR)/Server.hpp \


CSRC = $(SRCDIR)/RefClaint.cpp \

SRC = $(SRCDIR)/Claint.cpp  $(SRCDIR)/main.cpp $(SRCDIR)/Server.cpp \

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

