NAME	=	data_rw_test
SRCS	=	data_rw.c \
		data_rw_test.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wextra -Werror
RM	=	rm -rf
GARB	=	test_file

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
