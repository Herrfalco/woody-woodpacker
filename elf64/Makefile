NAME	=	elf64
SRCS	=	elf64.c
UTILS	=	../utils/asm/quit.o \
			../utils/asm/get_fd_size.o \
			../utils/asm/str_len.o \
			../utils/asm/round_up.o \
			../utils/asm/sat_sub.o
SCODE	=	shell_code.asm
OSCODE	= 	$(SCODE:.asm=.o)
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
AS		=	nasm
AFLAGS	=	-felf64
CFLAGS	=	-Wall -Wextra -Werror -no-pie
RM		=	rm -rf
GARB	=	woody

all		:	$(NAME)

$(NAME)	:	$(OBJS) shell
			make -C ../utils
			$(CC) $(CFLAGS) $(OBJS) $(OSCODE) $(UTILS) -o $@

shell	:	$(SCODE)
			$(AS) $(AFLAGS) $^

%.o		:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS) $(OSCODE)

fclean	:	clean
			make -C ../utils fclean
			$(RM) $(NAME) $(GARB)

re		:	fclean all
