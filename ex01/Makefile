CXX				= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRCS			= main.cpp \
						RPN.cpp

OBJS			=	$(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

INCS			=	$(shell find $(INCS_DIR) -type f -name "*.hpp")

SRCS_DIR	= ./srcs/
OBJS_DIR	= ./objs/
INCS_DIR	= ./incs/

NAME			= RPN

all:				$(NAME)

$(NAME):		$(OBJS)
						$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

${OBJS_DIR}%.o:	${SRCS_DIR}%.cpp $(INCS)
								@mkdir -p ${OBJS_DIR}
								$(CXX) $(CXXFLAGS) -I $(INCS_DIR) -c $< -o $@

clean:
						rm -rf ${OBJS_DIR}

fclean:			clean
						rm -rf $(NAME)

re:					fclean all

.PHONY:			all clean fclean re
