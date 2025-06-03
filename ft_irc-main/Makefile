NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address

SRCS = main.cpp Server.cpp Client.cpp tools.cpp channel.cpp Join.cpp Pass.cpp Quit.cpp User.cpp \
		Nick.cpp Privmsg.cpp kick.cpp topic.cpp Mode.cpp Invite.cpp
OBJS = $(SRCS:.cpp=.o)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
