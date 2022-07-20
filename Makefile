NAME	=	elf64
SRCS	=	elf64.c \
			utils.c
SCODE	=	shell_code.asm
OSCODE	= 	$(SCODE:.asm=.o)
OBJS	=	$(SRCS:.c=.o)
CC		=	gcc
AS		=	nasm
AFLAGS	=	-felf64
CFLAGS	=	-no-pie -Wall -Wextra #-Werror
RM		=	rm -rf
GARB	=	woody

all		:	$(NAME)

$(NAME)	:	$(OBJS) shell
			$(CC) $(CFLAGS) $(OBJS) $(OSCODE) -o $@

shell	:	$(SCODE)
			$(AS) $(AFLAGS) $^

%.o		:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

clean	:
			$(RM) $(OBJS) $(OSCODE)

fclean	:	clean
			$(RM) $(NAME) $(GARB)

re		:	fclean all
