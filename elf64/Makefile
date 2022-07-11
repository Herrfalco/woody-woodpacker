NAME	=	elf64
SRCS	=	elf64.c
ASM		=	../utils/asm/quit.o \
			../utils/asm/get_fd_size.o \
			../utils/asm/str_len.o \
			../utils/asm/round_up.o \
			../utils/asm/sat_sub.o
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -no-pie
RM		=	rm -rf
GARB	=	woody

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			make -C ../utils
			$(CC) $(CFLAGS) $^ $(ASM) -o $@

%.o	:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			make -C ../utils fclean
			$(RM) $(NAME) $(GARB)

re		:	fclean all
