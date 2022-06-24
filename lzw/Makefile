NAME	=	lzw_test
SRCS	=	lzw.c \
			unlzw.c \
			dico.c \
			main_test.c \
			../data_rw/data_rw.c \

ASM		=	new_entry.o \
			not_in_dico.o \
			../utils/asm/get_fd_size.o \
			../utils/asm/quit.o \
			../utils/asm/sat_sub.o

OBJS	=	$(SRCS:.c=.o)

CC	=	gcc

CFLAGS	=	-Wall -Wextra -Werror

RM	=	rm -rf

test	:	all

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		make -C ../utils
		make -C asm
		$(CC) $(CFLAGS) $^ -o $@

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		make -C asm fclean
		$(RM) $(NAME)

re	:	fclean all
