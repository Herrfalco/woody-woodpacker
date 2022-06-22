NAME	=	aes_test
SRCS	=	aes.c \
			aes_fd.c \
			aes_utils.c \
			../test_utils.c \
			../utils.c
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -no-pie
RM		=	rm -rf

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			make -C asm

%.o	:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME)
			make -C asm fclean

re		:	fclean all
