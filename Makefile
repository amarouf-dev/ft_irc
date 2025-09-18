NAME = ircserv
CNAME = Client
RM = rm -rf
CPP = c++
CPPFlags = -Wextra -Wall -Werror -std=c++98

HDIR = headers
SRCDIR = srcs
CMD_DIR = cmds

HDR = $(HDIR)/Client.hpp  $(HDIR)/Server.hpp $(HDIR)/Channel.hpp \


CSRC = $(SRCDIR)/RefClient.cpp \

SRC = $(SRCDIR)/Client.cpp  $(SRCDIR)/main.cpp $(SRCDIR)/Server.cpp $(SRCDIR)/Channel.cpp \
	$(CMD_DIR)/join.cpp $(CMD_DIR)/nick.cpp $(CMD_DIR)/pass.cpp $(CMD_DIR)/user.cpp $(CMD_DIR)/kick.cpp $(CMD_DIR)/invite.cpp $(CMD_DIR)/topic.cpp \

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

