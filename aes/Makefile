SRCS	=	aes.c \
			aes_fd.c
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -no-pie
RM		=	rm -rf

all		:	$(OBJS)
			make -C asm

%.o		:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			make -C asm fclean

re		:	fclean all
