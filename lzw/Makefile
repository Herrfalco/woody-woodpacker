NAME	=	lzw
SRCS	=	lzw.c \
		../data_rw/data_rw.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wextra -Werror
RM	=	rm -rf
GARB	=	test_file

test	:	all

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $(CFLAGS) $^ -o $@

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS) $(GARB)

fclean	:	clean
		$(RM) $(NAME)

re	:	fclean all
